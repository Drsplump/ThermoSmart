# ThermoSmart
A simple chronothermostat on a Nodemcu1.0 (esp 12e) initially developed for arduino.
Blynk is used to add a smart device control system

![alt text](https://github.com/Drsplump/ThermoSmart/blob/master/termo_nodemcu_bb.jpg)

# Features

- heating program saved on EEPROM

- time setting menu on LCD
  
- web server for setting wifi  credentials: 
  before power up, hold down and keep it as such
  for a few seconds the PLUS button, at this point an A.P. named "TermoSmart" is generated 
  with the settings address at 192.168.4.1,
  it is also possible to cancel the previously saved settings with the same procedure as above but using
  the SET key or by activating the line of code "wifiManager.resetSettings ();"
  
  
![alt text](https://github.com/Drsplump/ThermoSmart/blob/master/server.png)
  
 
 # Blynk app settings
  
- temperature probe connected on V2
 
- operation led connected on V4

- thermostat connected on V1
 
- Activation forcing connected to D0
 
- temperature graph connected on data from V2
 
  you can replicate the app with the QRcode attached and, later, by entering it in the space provided
  on the related line: "char auth [] =" **************************** ";" the AUTH code issued by Blynk
  
  
  ![alt text](https://github.com/Drsplump/ThermoSmart/blob/master/app%20blynk.jpg)
  
  
  
   *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
   
   tnx to:
   
   https://github.com/adafruit/RTClib
   
   https://github.com/johnrickman/LiquidCrystal_I2C
   
   https://github.com/PaulStoffregen/OneWire
   
   https://github.com/milesburton/Arduino-Temperature-Control-Library
   
   https://github.com/tzapu/WiFiManager
   
   https://github.com/blynkkk/blynk-library

   I hope I have not left anything out.
   
   seivasan79@gmail.com 
   
  
