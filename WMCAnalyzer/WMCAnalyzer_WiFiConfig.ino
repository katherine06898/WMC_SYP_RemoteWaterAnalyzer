#include <PubSubClient.h>
#include <SPI.h>
#include <Adafruit_SleepyDog.h>
#include <uFire_EC.h>
#include "defines.h"
#include "Credentials.h"
#include "dynamicParams.h"
uFire_EC ec;

#define pH_pin A6 // Analog pin for pH
#define conn_LED 13 // define pin as 13
#define dis_LED  7 // define as pin 7

/* Initialize parameters for sensors */
// Flow Rate Sensor
byte PWM_pin = 2;
volatile float waterflow;
volatile float freq;
float tmp = 0.00;
float Q;

// pH sensor
float pH;
float voltage = 0.00; // voltage used in pH

// Conductivity sensor
float cond;

// Low power mode
int sleep_time = 4; //will sleep for 16 seconds* sleep time

/*initialize parameters for MQTT connection*/
const char* mqttServer = mqttSERVER;
const char* mqttUsername = mqttUSERNAME;
const char* mqttPassword = thingKEY;
const char* mqttDeviceId = thingID;

// Initialize topics, 'resource' specified on tingg.io need to have the same name
char FRTopic[] = "FR";
char ECTopic[] = "EC";
char pHTopic[] = "pH";

// Set up WiFi client for Wi-Fi and MQTT connection
WiFiClient wifiClient;
PubSubClient client(wifiClient);
long lastMsg = 0;

/* Check Wi-Fi connection */
void heartBeatPrint(void)
{
  static int num = 1;

  if (WiFi.status() == WL_CONNECTED){
    Serial.print("WiFi Connected");        // Connected to WiFi
    connect_Led();                         // LED on device will be on
    setup_sensors();                       // Initialize sensors
    client.setServer(mqttServer, 1883);    // MQTT port
    client.setCallback(callback);
    reconnect();                           // Connect to MQTT server
    update_and_sleep();                    // Update value and enter low power mode
  }else{
    delay(500);
    Serial.print(".");                     // WiFi not connected
    disconnect_Led();                      // LED on device not on
  }
  if (num == 80)
  {
    Serial.println();
    num = 1;
  }
  else if (num++ % 10 == 0)
  {
    Serial.print(" ");
  }
}

void check_status()
{
  static unsigned long checkstatus_timeout = 0;

  //KH
#define HEARTBEAT_INTERVAL    20000L
  // Print hearbeat every HEARTBEAT_INTERVAL (20) seconds.
  if ((millis() > checkstatus_timeout) || (checkstatus_timeout == 0))
  {
    heartBeatPrint();
    checkstatus_timeout = millis() + HEARTBEAT_INTERVAL;
  }
}

// declare wifi manager
WiFiManager_NINA_Lite* WiFiManager_NINA;

#if USING_CUSTOMS_STYLE
const char NewCustomsStyle[] /*PROGMEM*/ = "<style>div,input{padding:5px;font-size:1em;}input{width:95%;}body{text-align: center;}\
button{background-color:blue;color:white;line-height:2.4rem;font-size:1.2rem;width:100%;}fieldset{border-radius:0.3rem;margin:0px;}</style>";
#endif

/* Setup for Wi-Fi Manager */
void setup()
{
  // Debug console
  Serial.begin(115200);
  //while (!Serial); //remove comment for troubleshooting

  delay(200);

  Serial.print(F("\nStarting SAMD_WiFiNINA on ")); Serial.println(BOARD_TYPE);
  Serial.println(WIFIMANAGER_NINA_LITE_VERSION);

  WiFiManager_NINA = new WiFiManager_NINA_Lite();

  // Optional to change default AP IP(192.168.4.1) and channel(10)
  //WiFiManager_NINA->setConfigPortalIP(IPAddress(192, 168, 120, 1));
  WiFiManager_NINA->setConfigPortalChannel(0);

#if USING_CUSTOMS_STYLE
  WiFiManager_NINA->setCustomsStyle(NewCustomsStyle);
#endif

#if USING_CUSTOMS_HEAD_ELEMENT
  WiFiManager_NINA->setCustomsHeadElement("<style>html{filter: invert(10%);}</style>");
#endif

#if USING_CORS_FEATURE  
  WiFiManager_NINA->setCORSHeader("Your Access-Control-Allow-Origin");
#endif

  // Set customized DHCP HostName
  WiFiManager_NINA->begin(HOST_NAME);
  //Or use default Hostname "SAMD-WiFiNINA-XXXXXX"
  //WiFiManager_NINA->begin();
  
}

#if USE_DYNAMIC_PARAMETERS
void displayCredentials()
{
  Serial.println(F("\nYour stored Credentials :"));

  for (uint16_t i = 0; i < NUM_MENU_ITEMS; i++)
  {
    Serial.print(myMenuItems[i].displayName);
    Serial.print(F(" = "));
    Serial.println(myMenuItems[i].pdata);
  }
}

void displayCredentialsInLoop()
{
  static bool displayedCredentials = false;

  if (!displayedCredentials)
  {
    for (int i = 0; i < NUM_MENU_ITEMS; i++)
    {
      if (!strlen(myMenuItems[i].pdata))
      {
        break;
      }

      if ( i == (NUM_MENU_ITEMS - 1) )
      {
        displayedCredentials = true;
        displayCredentials();
      }
    }
  }
}

#endif

/* Debug for MQTT connection */
void callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("topic: ");
  Serial.print(topic);
  Serial.print(" message: ");
  for (int i = 0; i < length; i++) 
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

/* Connect/Reconnect to MQTT server*/
void reconnect() 
{
  while (!client.connected()) 
  {
    Serial.print("connecting to mqtt.tingg.io...");
 
    if (client.connect(mqttDeviceId, mqttUsername, mqttPassword)) 
    {
      Serial.println("connected.");
    } else 
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

/* Sensors */
void setup_sensors() 
{    
  pinMode(pH_pin,INPUT);
  pinMode(PWM_pin, INPUT);
  pinMode(conn_LED,OUTPUT);
  pinMode(dis_LED,OUTPUT);

  waterflow = 0.00;

  // Conductivity Sensor setup
  Wire.begin();
  ec.begin();
  ec.reset();
  ec.calibrateProbe(0.8, 21.0);//set to 0.8mS/cm measured at 21.0 deg c as ref point
}

float pH_read(){

 analogReadResolution(12); // change resolution
    // pH calculation
    voltage = analogRead(pH_pin)*(3.3/4095);
    pH = ((-5.6548*(voltage))+ 15.511);

    
    if (isnan(pH)) {
      
      Serial.println("Error reading pH!");
      Serial.print("\n");
    }
    else {
      Serial.print("pH: ");
      Serial.print(pH);
      
      //publish to tingg.io
      client.publish(pHTopic, String(pH).c_str());
    }
    
  return pH;
  
}

float flow_read(){

   // Get Flow Rate event and print its value.
    waterflow = pulseIn(PWM_pin, HIGH);
    
    if (waterflow != 0.0) {
      freq = (1 / (waterflow * 0.000001));
      Q = (freq / 11); // believe this is correct
      
      Serial.print("\n");
      Serial.print("waterFlow:");
      Serial.print(Q);
      Serial.println("   L/min");

      //publish to tingg.io
      client.publish(FRTopic, String(Q).c_str());
     } 
     else {
      Serial.print("\n");
      Serial.println("Error reading Flow Rate!");

      waterflow = 0.00;
      freq =0.00;
      Q =0.00;
      client.publish(FRTopic, String(Q).c_str());
     }

  return Q;
  
}

/* Read values from conductivity sensor */
float cond_read(){

  // Get EC event and print its value.
    tmp = ec.measureEC(21.0);

    if((tmp >= -0.5) && (tmp <= 100.0)){
      Serial.print("\n");
      Serial.println((String) "EC (mS/cm): " + ec.measureEC(21.0));
      cond = tmp;
      
      //publish to tingg.io
      client.publish(ECTopic, String(cond).c_str()); 
    }
return cond;
  
}


/* Turn on LED if network is connected */
void connect_Led(){ 

digitalWrite(conn_LED, HIGH);  // high led high
digitalWrite(dis_LED, LOW); // other lED low

  
}

/* blink LED if disconnected */
void disconnect_Led(){ 

digitalWrite(conn_LED, LOW);  // high led high
digitalWrite(dis_LED, HIGH); // other lED low
delay(500);
digitalWrite(dis_LED, LOW); // other lED low
delay(100);
  
}

/* Run Wi-Fi manager */
void loop()
{
  WiFiManager_NINA->run();
  check_status();

#if USE_DYNAMIC_PARAMETERS
  displayCredentialsInLoop();
#endif 
}

/* Set update time and sleep time */
void update_and_sleep() 
{
  int x = 0;
  if (!client.connected()) 
  {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 3000) //publish new values every 3 seconds
  {

    pH_read();
    flow_read();
    cond_read();
    delay(1000);
  
    //sleep function  
    for (int i = 0; x<=sleep_time; x++){
      int sleepMS = Watchdog.sleep();
    }
  }
}
