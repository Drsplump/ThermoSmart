#ifndef reading_h
#define reading_h

class reading {
  private:
  
  public:

  reading();
};

reading::reading(){
   for(int i=0;i<24;i++){
    EEPROM.begin(512);
    vcav[i]=EEPROM.read(i);
   }
   
  
};
#endif
