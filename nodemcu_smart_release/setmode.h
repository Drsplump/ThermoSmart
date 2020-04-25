#ifndef setmode_h
#define setmode_h

void setMode( DateTime now ) {

  boolean setMode = true;
  int setModeLevel = 0;

  int _day = now.day();
  int _month = now.month();
  int _year = now.year();
  int _hour = now.hour();
  int _min = now.minute();
  int _sec = now.second();

  lcd.clear();
  lcd.setCursor(0, 0);
  delay( 1000 );
  timeSet = millis();

  while ( setMode ) {
    if ( digitalRead(SET) == HIGH || digitalRead(PIU) == HIGH || digitalRead(MENO) == HIGH ) {
      timeSet = millis();
    }

    lcd.setCursor(0, 0);

    
    // Set Ora
    if ( setModeLevel == 0 ) {
      if ( digitalRead(PIU) == HIGH && _hour < 23) {
        _hour++;
      }
      if ( digitalRead(MENO) == HIGH && _hour > 0) {
        _hour--;
      }

      sprintf(buffer,  "%s: %02d", "Ora", _hour);
    }
    // Set Minuto
    if ( setModeLevel == 1) {
      if ( digitalRead(PIU) == HIGH && _min < 59) {
        _min++;
      }
      if ( digitalRead(MENO) == HIGH && _min > 0) {
        _min--;
      }

      sprintf(buffer,  "%s: %02d", "Minuti", _min);
    }
    

    lcd.print( buffer );
    if ( digitalRead(SET) == HIGH ) {
      lcd.clear();
      setModeLevel++;
    }

    if (setModeLevel == 2) {
      rtc.adjust(DateTime(_year, _month, _day, _hour, _min, _sec));
      opz = -1;
      break;

    }
 
    delay(200);
  }

}

#endif
