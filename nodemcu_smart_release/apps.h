#ifndef apps_h
#define apps_h


BLYNK_WRITE(V1)
{
  soglia = param.asDouble();
  Serial.print("V1 Temp: ");
  Serial.println(soglia);
};


#endif
