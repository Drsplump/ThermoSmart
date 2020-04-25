# ThermoSmart
ThermoSmart is a project that creates a chronothermostat based on Nodemcu1.0 (esp 12e)
and initially developed for arduino.
Blynk is used to add a smart device control system
* graphic modification of riders and addition of time position indication

# Features


 * support rtc DS3231

 * heating program saving on EEPROM

 * time setting menu

 * DS18b20 digital temperature probe used with resolution set to 9 bit
  in my case I had to carry out a calibration of -1 C in the "void temperature ()" section
  
 * web server for setting wifi  credentials: 
  before power up, hold down and keep it as such
  for a few seconds the PLUS button, at this point an A.P. named "TermoSmart" is generated 
  with the settings address at 192.168.4.1,
  it is also possible to cancel the previously saved settings with the same procedure as above but using
  the SET key or by activating the line of code "wifiManager.resetSettings ();"
  
  # Blynk settings
 BLYNK app connection:
  temperature probe connected on V2
  power light connected on V4
  thermostat connected on V1
  Activation forcing connected to D0
  temperature graph connected on data from V2
  you can replicate the app with the QRcode attached and, later, by entering it in the space provided
  on the related line: "char auth [] =" **************************** ";" the AUTH code issued by Blynk
 
  

