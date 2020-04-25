#ifndef writing_h
#define writing_h

class writing {
  private:
  
  public:
  writing();
};

writing::writing(){

  for (int i = 0; i < 24; i++) {
      EEPROM.write(i, vcav[i]);
      EEPROM.commit();
    }
    
};

#endif
