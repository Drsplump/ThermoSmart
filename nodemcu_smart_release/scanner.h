#ifndef scanner_h
#define scanner_h

void scanner() { //aggiorna i cavalieri

  int i = 0;
  for (i = 0; i < 12; i++) {
    if (vcav[i * 2] == 0 && vcav[i * 2 + 1] == 0) {

      lcd.setCursor(i + 2, 1);
      lcd.write(byte(0));
    }

    if (vcav[i * 2] == 1 && vcav[i * 2 + 1] == 0) {
      lcd.setCursor(i + 2, 1);
      lcd.write(byte(1));
    }

    if (vcav[i * 2] == 0 && vcav[i * 2 + 1] == 1) {
      lcd.setCursor(i + 2, 1); 
      lcd.write(byte(2));
    }

    if (vcav[i * 2] == 1 && vcav[i * 2 + 1] == 1) {
      lcd.setCursor(i + 2, 1); 
      lcd.write(byte(3));
    }
  }

}






#endif scanner_h
