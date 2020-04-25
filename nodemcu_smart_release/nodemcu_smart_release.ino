/*          Modified version for Nodemcu1.0 (esp 12e) of the Arduino programmable thermostat

                                 CHANGELOG:

 * graphic modification of riders and addition of time position indication

 * added indication with icons if time is off / on

 * support rtc DS3231

 * modified temperature in steps of 0.5 degrees C

 * added heating program saving on EEPROM

 * added time setting menu

 * changes hysteresis to +/- 0.5 degrees C

 * DS18b20 digital temperature probe used with resolution set to 9 bit
  in my case I had to carry out a calibration of -1 C in the "void temperature ()" section* web server for setting wifi credentials: before giving voltage, hold down and keep it as such
  for a few seconds the PLUS button, at this point an access point is generated
  "TermoSmart" with the settings address at 192.168.4.1,
  it is also possible to cancel the previously saved settings with the same procedure as above but using
  the SET key or by activating the line of code "wifiManager.resetSettings ();"
  
* - * - * - * - * - * - * - * - * - * - * - * - * - * - * - * - * - * - * - * - * - * - * - * - * - * - * - * - * - * - * - * - * - * - * - * - * - * - * - * - * - * - * - * - * - * - * - * - * - * - * - * - * - *

  BLYNK app connection:
  temperature probe connected on V2
  power light connected on V4
  thermostat connected on V1
  Activation forcing connected to D0
  temperature graph connected on data from V2
  you can replicate the app with the QRcode attached and, later, by entering it in the space provided
  on the related line: "char auth [] =" **************************** ";" the AUTH code issued by Blynk


used libraries:

   https://github.com/adafruit/RTClib
   https://github.com/johnrickman/LiquidCrystal_I2C
   https://github.com/PaulStoffregen/OneWire
   https://github.com/milesburton/Arduino-Temperature-Control-Library
   https://github.com/tzapu/WiFiManager
   https://github.com/blynkkk/blynk-library

   the following line must be added as an additional URL in the settings: http://arduino.esp8266.com/stable/package_esp8266com_index.json

   I hope I have not left anything out.
   
   seivasan79@gmail.com
*/

#include "RTClib.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>            //D1=SCL     D2=SDA
#include <OneWire.h>
#include <DallasTemperature.h>
OneWire oneWire_temp(2);                  //D4
DallasTemperature temp(&oneWire_temp);
#include <EEPROM.h>
RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2);

boolean vcav[24];

#include "reading.h"
#include "scanner.h"
#include "write.h"

int lettura = 0;
double gradiC;
long trTemp = 0, trOra = 0, trDisp = 0;
int opz = -1, inc_opz = 0, incH = 0, mod_incH = 0, flagH = 0, flagT = 0, t_vecchia = 0, stato = 0, timer = 0;
int t_luce = 30;                  // tempo luminosità
double unit = 00.50;

#define SET       13        //D5
#define PIU       12        //D6
#define MENO      14        //D7
#define rele      15        //D8

char buffer[10];
unsigned long timeSet = 0;
int  setModeTime = 5000;
#include "setmode.h"
#include "caratteri.h"

int EEaddress = 32;
float floatingpoint_variable;
double soglia;

/////////////////////////---------------------------------------app Blynk

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include "WiFiManager.h"
#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>
char auth[] = "/////// your Blynk AUTH////////";
#include"apps.h"

////////////////////////---------------------------------------fine Blynk


void setup() {

  Serial.begin(115200);
  EEPROM.begin(512);
  rtc.begin();
  Wire.begin();
  lcd.clear();
  lcd.backlight();
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("TermoSmart v0.1");

  WiFiManager wifiManager;

  //wifiManager.resetSettings();

  if (digitalRead(SET) == HIGH) {
    Serial.println("Reset wifi settings");
    wifiManager.resetSettings();
  }

  if (digitalRead(PIU) == HIGH) {
    if (!wifiManager.autoConnect("TermoSmart")) {
      Serial.println("failed to connect and hit timeout");
      ESP.reset();
      delay(1000);
    }

  }

  wifiManager.setAPCallback(configModeCallback);

  Serial.println("connected...yeey :)");

  WiFi.printDiag(Serial);
  Blynk.config(auth);

  lcd.createChar(0, spento);
  lcd.createChar(1, on_pari);
  lcd.createChar(2, on_dispari);
  lcd.createChar(3, on_due);
  lcd.createChar(4, segna_pari);
  lcd.createChar(5, segna_dispari);
  lcd.createChar(6, orologio);
  lcd.createChar(7, clessidra);
  temp.begin();
  temp.setResolution(9);
  lcd.backlight();
  delay(2000);

  reading();

  pinMode(SET, INPUT);                                  //menu
  pinMode(PIU, INPUT);                                  //+
  pinMode(MENO, INPUT);                                 //-
  pinMode(rele, OUTPUT);
  digitalWrite(rele, LOW);

  EEaddress = 32;
  floatingpoint_variable;
  soglia = EEPROM.get(EEaddress, floatingpoint_variable);

  lcd.clear();
}

void loop() {

  Blynk.run();
  Blynk.virtualWrite(V2, gradiC);

  reading();

  floatingpoint_variable = soglia;
  EEPROM.put(EEaddress, floatingpoint_variable);
  EEPROM.commit();
  Serial.print("Temperatura memorizzata  : ");
  EEPROM.get(EEaddress, floatingpoint_variable);
  Serial.println(floatingpoint_variable);

  temp.requestTemperatures();
  //casi pressione menu
  if (digitalRead(SET) == HIGH) {

    opz = inc_opz % 4;
    //Serial.println(opz);
    switch (opz) {

      case -1:
        lcd.backlight();
        break;

      case 0:
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Imp. orari");
        lcd.backlight();
        scanner();
        break;

      case 1:
        flagH = 0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Set temperatura");
        lcd.setCursor(0, 1);
        lcd.setCursor(9, 1);
        lcd.print(soglia);
        lcd.backlight();

        break;

      case 2:
        lcd.clear();
        lcd.backlight();
        flagH = 0;
        timer = 0;

        break;

      case 3:
        lcd.clear();
        lcd.backlight();
        flagH = 0;
        timer = 0;
        break;

    }
    delay(500);
    inc_opz++;
    timer = 0;


  }
  if ((opz == 2)) {
    lcd.clear();
    DateTime now = rtc.now();
    setMode( now );

  }

  if ((opz == 3) || (opz == -1)) { //siamo fuori dal menu in visualizzazione normale
    scanner();
    lcd.setCursor(0, 0);
    //sistemiamo ora
    if (millis() - trOra > 1000) {
      DateTime now = rtc.now();
      char ora[10];
      sprintf(ora, "%2d:%02d" , now.hour(), now.minute());
      lcd.setCursor(0, 0);
      lcd.print(ora);
      trOra = millis();
      if (vcav[now.hour()]) {
        lcd.setCursor(0, 1);
        lcd.write(byte(6));
      } else {
        lcd.setCursor(0, 1);
        lcd.write(byte(7));
      }
    }


    //risparmio energetico spegne la retroilluminazione del display
    if (millis() - trDisp > 1000) {
      timer++;
      trDisp = millis();

    }
    if (timer >= t_luce) {
      lcd.noBacklight();
      inc_opz = -1;
    }

    //sistemiamo temperatura

    if (millis() - trTemp > 500) { // aggiorna ogni x000 la temperatura
      lcd.setCursor(10, 0);
      temperatura();
    }
    DateTime now = rtc.now();
    float sogliasup = (soglia + unit);
    float sogliainf = (soglia - unit);
    switch (stato) {
      case 0: //se è spenta

        if (gradiC <= sogliainf  && vcav[now.hour()]) { //se sono sotto soglia && in orario la accendo
          lcd.setCursor(6, 0);
          lcd.print("on  ");
          digitalWrite(rele, HIGH);
          Blynk.virtualWrite(V4, 1023);
          lcd.backlight();
          stato = 1;
          //soglia = soglia +2;

        } else { //altrimenti la tengo spenta
          lcd.setCursor(6, 0);
          lcd.print("off ");
          digitalWrite(rele, LOW);
          Blynk.virtualWrite(V4, 0);
          //lcd.backlight();
          stato = 0;

        }
        break;

      case 1:// se è accesa
        if (gradiC >= sogliasup  || vcav[now.hour()] == 0) { //se sono oltre soglia oppure fuori orario la la spengo
          lcd.setCursor(6, 0);
          lcd.print("off");
          digitalWrite(rele, LOW);
          Blynk.virtualWrite(V4, 0);
          //lcd.backlight();
          stato = 0;
          //soglia = soglia - 2;

        }
        break;

    }


    if (stato == 0) {
      lcd.setCursor(6, 0);
      lcd.print("off");
      digitalWrite(rele, LOW);
      Blynk.virtualWrite(V4, 0);
    }
    else {
      lcd.setCursor(6, 0);
      lcd.print("on");
      digitalWrite(rele, HIGH);
      Blynk.virtualWrite(V4, 1023);
    }



  }

  if (digitalRead(MENO) && opz == 1) //decrementa temperatura alla pressione del tasto -
  {
    soglia = (soglia - unit);
    lcd.setCursor(9, 1);
    lcd.print(soglia);
    delay(200);
    if (gradiC < soglia) stato = 0;
    if (gradiC > soglia) stato = 1;

  }


  if (digitalRead(PIU) && opz == 1)       //aumenta temperatura alla pressione del tasto +
  {
    soglia = (soglia + unit);
    lcd.setCursor(9, 1);
    lcd.print(soglia);
    delay(200);
    if (gradiC < soglia) stato = 0;
    if (gradiC > soglia) stato = 1;

  }


  if (digitalRead(PIU) && opz == 0)         //scorre cavalieri
  { flagH = 1;
    mod_incH = incH % 24;
    lcd.clear();
    scanner();
    lcd.backlight();

    if (incH % 2 == 0) {
      lcd.setCursor(mod_incH / 2 + 2 , 0);
      lcd.write(byte(4));

    }
    if (incH % 2 == 1) {
      lcd.setCursor(mod_incH / 2 + 2 , 0);
      lcd.write(byte(5));
    }

    lcd.setCursor(0, 0);
    lcd.print(mod_incH);
    delay(200);
    incH++;

  }
  if (digitalRead(MENO) && opz == 0 && flagH == 1) {      //setta cavalieri
    vcav[mod_incH] = !vcav[mod_incH];
    writing();
    scanner();
    delay(200);
  }


}

void temperatura() {
  gradiC = (temp.getTempCByIndex(0) - 1);
  lcd.print(gradiC);
  lcd.print("c");
  trTemp = millis();


}

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
}
