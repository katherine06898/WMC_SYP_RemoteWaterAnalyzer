# Senior Year Project - Remote Water Analyzer for Water Management Canada
## Description
  Water Management Canada deals with the monitor and service of HVAC systems in commercial, multi-residential, and industrial buildings. Currently all monitoring is done by running periodic manual tests. This method leaves room for optimization as labour is expensive and there is a large period between subsequent tests. In an increasingly connected world, there is opportunity to replace the manual testing with autonomous IOT based sensors.  Spurred by the repercussions of the global pandemic, WMC came to Dalhousie University and Group 13 in the Electrical and Computer Engineering Department seeking the design of a prototype which could replace their manual checks in a cost-effective manor.  The device would be connected to an existing piping system using T-pipe fitting and monitor various water qualities using sensors. The readings specified to the group were pH, electroconductivity, and water flowrate. The device must connect to an external dashboard and upload the data continuously. The following is group 13’s proposed solution to WMC.
  
## User Instructions
  The remote water analyzer serves as a standalone device capable of monitoring the electroconductivity (mS/cm^2), pH, and flowrate of a water pipe system.  For WMC the device will monitor HVAC systems and upload data over wi-fi to a dashboard.  
The first iteration of the device leaves room for improvement over some areas:
-	LoRaWAN connection methods
-	Dynamic input for parameters (Implemented but not tested for ThingID and ThingKey)
-	Automatic warning
-	Proprietary Probe Design
-	Customized dashboard
-	Temperature and other sensor expansions
-	Hardware battery level notification system
-	Extra low power RTL clock
-	Hardware sensor failure systems
-	Increased isolation in the pH electrical readings from flowrate readings

### Installation and Operation Instructions
#### *Configuring the Dashboard*
1.	Go to tingg.io, click **Sign Up for Free** and register an account 
 ![image](https://user-images.githubusercontent.com/36083529/117103713-6b88b180-adad-11eb-8bdb-8b1a1d27079d.png)
 
2.	After registration, log into the console and go to THINGS -> **Connect a Thing** 
 ![image](https://user-images.githubusercontent.com/36083529/117103736-75121980-adad-11eb-9965-11c943761bf0.png)
 
3.	Specify these parameters in the pop-up widget and click **Connect**. Here try to name your thing a specific but generic name first such as WMC_device, since this name will be used as the Thing Type name, which makes it easy when you create other new Things that will have the same configurations.
 ![image](https://user-images.githubusercontent.com/36083529/117103825-a1c63100-adad-11eb-9fbb-52c3c5c5b6c7.png)
 
4.	Once the Thing shows up on the board, click on the Thing or **See Details**
 ![image](https://user-images.githubusercontent.com/36083529/117103831-a7bc1200-adad-11eb-8b4e-6a7be863d23f.png)

5.	You will see the page below, click **Configure**
 ![image](https://user-images.githubusercontent.com/36083529/117103865-b9051e80-adad-11eb-9323-f7b733685226.png)

6.	Once the page shown below shows up, 
a.	**Topic Field: pH; Method: Publish data; Type: Number**
Then click **Configure**.
 ![image](https://user-images.githubusercontent.com/36083529/117103878-c15d5980-adad-11eb-986d-ba051eac953a.png)
 
7.	You should see the pH resource you configured show up on the board. Click **+ New Resource** button to configure two more resources in the same way.
a.	**Topic Field: EC; Method: Publish data; Type: Number**
b.	**Topic Field: FR; Method: Publish data; Type: Number**

  ![image](https://user-images.githubusercontent.com/36083529/117104140-32047600-adae-11eb-9421-fe327839acf5.png)
  ![image](https://user-images.githubusercontent.com/36083529/117104150-3761c080-adae-11eb-8b1f-9d50800e510c.png)


8.	Navigate to THINGS tab on the left, go to **Settings**. In **Things Authentication** you will see **ThingID** and **ThingKey**, copy and save them somewhere locally for later use.
  ![image](https://user-images.githubusercontent.com/36083529/117104176-45174600-adae-11eb-8e4d-18e01698a170.png)

 
9.	If you wish to add more Devices (Things), click the **THINGS** -> **+ Connect a Thing**, now you can specify a different name for your other devices and select a Type that was created above to save time on the configuration.
  ![image](https://user-images.githubusercontent.com/36083529/117104188-4b0d2700-adae-11eb-8bc8-bc279156e7c7.png)

10.	Click on the Thing you wish to see data from and explore different options to view your data.
  ![image](https://user-images.githubusercontent.com/36083529/117104210-53656200-adae-11eb-90e7-a1708d26b4db.png)
  ![image](https://user-images.githubusercontent.com/36083529/117104221-57917f80-adae-11eb-9ada-6689daa0168f.png)


#### *Hardware Setup*
1.	The PCB files can be found on GitHub under “PCB” under the file name “Gerbs” and should be downloaded as a zip file. This file contains the Gerber and drill files necessary to manufacture the PCB. 
  ![image](https://user-images.githubusercontent.com/36083529/117104446-b9ea8000-adae-11eb-856c-8bde4ac70a76.png)

2.	Once the PCB is manufactured, soldering of the necessary components can begin. The Bill of materials in this document includes all the components necessary for assembly. 
  ![image](https://user-images.githubusercontent.com/36083529/117104453-bd7e0700-adae-11eb-850b-b9f1912ff96b.png)

3.	After correct assembly the PCB will appear as seen above. It is noteworthy that the Red LED is in the top right corner and the Blue is on the left. 
  ![image](https://user-images.githubusercontent.com/36083529/117104473-c5d64200-adae-11eb-8e54-8a2f6b33cfe4.png)

4.	Each component is soldered. The user can plug in all the necessary attachments accordingly. 


#### *Software Setup*
When setting up a new device:
1.	Download Arduino IDE to PC or create an account on Arduino Create to use their web editor.
2.	Pull the code from  https://github.com/katherine06898/WMC_SYP_RemoteWaterAnalyzer
3.	Connect the device through USB cable to laptop and download the firmware as well as update as instructed.
4.	Open the code in the editor and navigate to Credentials.h
5.	Each‘hing’ on tingg.io is setup per device, therefore before loading the program to new devices, change the ‘ThingID’ and ‘ThingKey’ values (see step 8 of Configuring Dashboard) in the Credentials.h file to match with the ‘Thing’ you wish to see data from.

#### *Hardware Installation*
1.	The device comes stock with ¾” PVC T Connectors and an inline flowrate sensor.  The T connectors and flow rate sensor should be installed on a vertical or horizontal plane in a relatively safe space.
2.	The probes can be screwed into the liquid tight probe adaptors. 
3.	Dependent on space constraints the 3D printed electrical housing can be installed directly on the probes (with probes fitting in the bottom holes) or wall mounted less than the wires’ distance away.  
4.	The electrical casing houses the PCB, sensor drivers, and batteries.  The PCB screws into the frame using 3mm screws and appropriate risers.  The batteries can be stored in the free space to the left of the PCB.
5.	The device is designed to use two 9 volt 4.5Ah batteries but can use any combination of 9 volt supply.
6.	Probe wiring is designed to be modular.  Both sensors can be replaced at the board using their analog cable connection, or at the PCB using the jumper cables.  


#### *Configuring Wi-Fi*
1.	Once the device is installed and plugged in, the LED light on the device is off, suggesting Wi-Fi is not connected.
2.	Go to Settings -> Wi-Fi on the phone and look for WIFININA_51F485, tap connect and input password: MyWIFININA_51F485 and save the connection.
  ![image](https://user-images.githubusercontent.com/36083529/117104569-f4ecb380-adae-11eb-9100-b79aa5271c15.png)

3.	If prompted “This network has no internet access, would you like to connect anyway?”, tap connect anyways.
4.	Once connected go to a browser of your choice and put 192.168.4.1 in the address bar. You should see a web page demonstrated as below. Put in the Wi-Fi SSID that you would like to connect to and enter the respective password. (Note: You only need to put in one set of Wi-Fi SSID and password). Enter a board name of your choice or leave it blank.
  ![image](https://user-images.githubusercontent.com/36083529/117104578-fae29480-adae-11eb-8ecb-ab3f719f6e6b.png)

5.	Click Save, and you should see a prompt on the webpage as shown below. 
  ![image](https://user-images.githubusercontent.com/36083529/117104596-046bfc80-adaf-11eb-92ed-64938c919ca2.png)

6.	Observe if the device blue LED is on, indicating it is connected to Wi-Fi network.

### Troubleshooting
| Error                                    | Suggested Solution           | 
| -----------------------------------------|:----------------------------:|
| Sensors reading -1 or infinite values    | Check sensors for physical failure. If the device still reads an unexpected value replace the probe from the analog cable. Sensors reading other unexpected values	Recalibrate the sensor using a known solution. If still reading unexpected value, replace probe from the analog cable. |
| No reported data    | It is normal to observe slight delays on reported data depending on the network environment the device is place. If the issue continues:
-	Check if battery is dead, if so replace battery.
-	Check connection quality, if failing see connection failure.
-	If none of the above replace microcontroller or run electrical diagnostic tests.   |
| Connection failure | In light of a connection failure, the blue LED on device is turned off. Plug in the device again and reset the network following steps 1-6 in Configuring Wi-Fi section.    |
| Code Troubleshoot  | Code troubleshoots can be done using the Serial Monitor feature in Arduino IDE or the serial monitor in Arduino Create Web Editor |
	 
### Bill of Material
Specific Details in document sent to Geoff Gross, Water Management Canada.
