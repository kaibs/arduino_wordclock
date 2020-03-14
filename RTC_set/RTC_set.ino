#include <Time.h>
#include <DS3232RTC.h>
#include <Wire.h>




void setup()
{
  Serial.begin(9600);
  setSyncProvider(RTC.get);
}

void loop()
{
  setTime(22, 40, 00, 27, 01, 2015);
  RTC.set(now());
  
}
