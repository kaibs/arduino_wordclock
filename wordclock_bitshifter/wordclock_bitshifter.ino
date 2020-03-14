
#include <Streaming.h>
#include <Wire.h>
#include <Time.h>
#include <TimeLib.h>
#include <DS3232RTC.h>


//DS3231  rtc(SDA, SCL);
//const int ButtonM = 2;  //Buttons für Zeiteinstellung
//const int ButtonH = 3;
//int buttonStateH = LOW;
//int readingM;
int readingH;
int previousM = LOW;
int previousH = LOW;
//int LDR = A0;           //Fotowiderstand
//int Sensorwert = 0;
//boolean ledValues[110];
//boolean leo = false;
//boolean kai = false;
int oldminute = 0;

int DataPin = 9;   //DS
int LatchPin = 11;  //STCP
int ClockPin = 10; //SHTP

int minuteOne = 4;
int minuteTwo = 5;
int minuteThree = 7;
int minuteFour = 6;
int uhr = 8;
int singleS = 3;

boolean setS = false;

boolean registers[24];


void setup() {

  //-----Pinout initialisieren-----//


  //pinMode(ButtonH, INPUT);
  //pinMode(ButtonM, INPUT);
  pinMode(minuteOne, OUTPUT);
  pinMode(minuteTwo, OUTPUT);
  pinMode(minuteThree, OUTPUT);
  pinMode(minuteFour, OUTPUT);
  pinMode(uhr, OUTPUT);
  pinMode(singleS, OUTPUT);

  pinMode(DataPin, OUTPUT);
  pinMode(LatchPin, OUTPUT);
  pinMode(ClockPin, OUTPUT);

  //-----------Uhr & Bitshift initialisieren--------------//
  
  hourFormat12();
  oldminute = minute();

  clearRegisters(); //reset beim Booten
  writeRegisters();
  

  //setTime(12, 45, 00, 20, 7, 2017);


  Serial.begin(115200);
  setSyncProvider(RTC.get);
  if (timeStatus() != timeSet) //Notification IC2-Kommunikation bei Stream-Beginn
    Serial.println("Unable to sync with the RTC");
  else
    Serial.println("RTC has set the system time");
}

//----------------------------------Funktionen für Bitshift------------------------------//

void clearRegisters() //Funktion definieren
{
  for(int i = 24 - 1; i >= 0; i--)
   {
    registers[i] = LOW;
   }
}

void writeRegisters()  //Funktion definieren
{
  digitalWrite(LatchPin, LOW);
  
  for(int i = 24-1; i >= 0; i--)
  {
    digitalWrite(ClockPin, LOW);
    int val = registers[i];
    digitalWrite(DataPin, val);
    digitalWrite(ClockPin, HIGH);
  }
  
  digitalWrite(LatchPin, HIGH);
}

void setRegisterPin( int index, int value)  //Funktion definieren
{
  registers[index] = value;
}

void loop() {

  //ES IST

  setRegisterPin(0, HIGH);
  setRegisterPin(1, HIGH);

  //-----Zeitsteuereung-----//

  //if( minute() != oldminute){
  //Serial.print(" Changing ");
  //Serial.end();
  //noInterrupts();       //interrups unterbrechen, da ws2812b zeitsensitiv

  //-----Strukturwörter-----//

  //UHR
  if ((minute() < 5))
  {
    digitalWrite(uhr, HIGH);
    setS = false;
    
  }
  else{
    digitalWrite(uhr, LOW);
    setS = true;
    
  }

  //VOR
  if ((minute() > 24) && (minute() < 30) || (minute() > 49))
  {
    setRegisterPin(7, HIGH);
  }
  else{
    setRegisterPin(7, LOW);
  }

  //NACH
  if ((minute() > 4) && (minute() < 25) || (minute() > 34) && (minute() < 45))
  {
    setRegisterPin(9, HIGH);
  }
  else{
    setRegisterPin(9, LOW);
  }
  //-----Zahlen-----//

  //EIN
  if (((minute() < 25) && (hourFormat12() == 1) || (minute() > 24) && (hourFormat12() == 0) || (minute() > 24) && (hourFormat12() == 12)))
  {
    setRegisterPin(14, HIGH);
  }
  else{
    setRegisterPin(14, LOW);
  }

  //S
   if (((minute() < 25) && (hourFormat12() == 1) && setS || (minute() > 24) && (hourFormat12() == 0) && setS || (minute() > 24) && (hourFormat12() == 12) && setS))
  {
    digitalWrite(singleS, LOW);
  }
  else{
    digitalWrite(singleS, HIGH);
  }

  //ZWEI
  if ((minute() < 25) && (hourFormat12() == 2) || (minute() > 24) && (hourFormat12() == 1))
  {
    setRegisterPin(15, HIGH);
  }
  else{
    setRegisterPin(15, LOW);
  }
  
  //DREI
  if ((minute() < 25) && (hourFormat12() == 3) || (minute() > 24) && (hourFormat12() == 2))
  {
    setRegisterPin(16, HIGH);
  }
  else{
    setRegisterPin(16, LOW);
  }

  //VIER
  if ((minute() < 25) && (hourFormat12() == 4) || (minute() > 24) && (hourFormat12() == 3))
  {
    setRegisterPin(17, HIGH);
  }
  else{
    setRegisterPin(17, LOW);
  }

  //ÜNF
  if ((minute() < 25) && (hourFormat12() == 5) || (minute() > 24) && (hourFormat12() == 4))
  {
    setRegisterPin(13, HIGH);
  }
  else{
    setRegisterPin(13, LOW);
  }

  //F
  if ((minute() < 25) && (hourFormat12() == 5) || (minute() > 24) && (hourFormat12() == 4) || (minute() < 25) && (hourFormat12() == 11) || (minute() > 24) && (hourFormat12() == 10))
  {
    setRegisterPin(12, HIGH);
  }
  else{
    
    setRegisterPin(12, LOW);
  }
  

  //SECHS
  if ((minute() < 25) && (hourFormat12() == 6) || (minute() > 24) && (hourFormat12() == 5))
  {
    setRegisterPin(18, HIGH);
  }
  else{
    setRegisterPin(18, LOW);
  }


  //SIEBEN
  if ((minute() < 25) && (hourFormat12() == 7) || (minute() > 24) && (hourFormat12() == 6))
  {
    setRegisterPin(20, HIGH);
  }
  else{
    setRegisterPin(20, LOW);
  }


  //ACHT
  if ((minute() < 25) && (hourFormat12() == 8) || (minute() > 24) && (hourFormat12() == 7))
  {
    setRegisterPin(19, HIGH);
  }
  else{
    setRegisterPin(19, LOW);
  }


  //NEUN
  if ((minute() < 25) && (hourFormat12() == 9) || (minute() > 24) && (hourFormat12() == 8))
  {
    setRegisterPin(21, HIGH);
  }
  else{
    setRegisterPin(21, LOW);
  }

  //ZEHN
  if ((minute() < 25) && (hourFormat12() == 10) || (minute() > 24) && (hourFormat12() == 9))
  {
    setRegisterPin(23, HIGH);
  }
  else{
    setRegisterPin(23, LOW);
  }


  //EL
  if ((minute() < 25) && (hourFormat12() == 11) || (minute() > 24) && (hourFormat12() == 10))
  {
    setRegisterPin(11, HIGH);
  }
  else{
    setRegisterPin(11, LOW);
  }

  //ZWÖLF
  if ((minute() < 25) && (hourFormat12() == 12) || (minute() > 24) && (hourFormat12() == 11))
  {
    setRegisterPin(22, HIGH);
  }
  else{
    setRegisterPin(22, LOW);
  }


  //-----Zeitunterteilung-----//

  //FÜNF
  if ((minute() > 4) && (minute() < 10) || (minute() > 24) && (minute() < 30) || (minute() > 34) && (minute() < 40) || (minute() > 54))
  {
    setRegisterPin(2, HIGH);
  }
  else{
    setRegisterPin(2, LOW);
  }


  //ZEHN
  if ((minute() > 9) && (minute() < 15) || (minute() > 39) && (minute() < 45) || (minute() > 49) && (minute() < 55))
  {
    setRegisterPin(3, HIGH);
  }
  else{
    setRegisterPin(3, LOW);
  }


  //VIERTEL
  if ((minute() > 14) && (minute() < 20) || (minute() > 44) && (minute() < 50)) //Intervall für Dreiviertel ebenfalls eingeschlossen
  {
    setRegisterPin(6, HIGH);
  }
  else{
    setRegisterPin(6, LOW);
  }

  //ZWANZIG
  if ((minute() > 19) && (minute() < 25))
  {
    setRegisterPin(4, HIGH);
  }
  else{
    setRegisterPin(4, LOW);
  }


  //HALB
  if ((minute() > 24) && (minute() < 45))
  {
    setRegisterPin(10, HIGH);
  }
  else{
    setRegisterPin(10, LOW);
  }


  //DREI(VIERTEL)
  if ((minute() > 44) && (minute() < 50))
  {
    setRegisterPin(5, HIGH);
    
  }
  else{
    setRegisterPin(5, LOW);
    
  }

  


  //-----Minutenanzeige-----//


  //LED1

  if ((minute() == 5) || (minute() == 10) || (minute() == 15) || (minute() == 20) || (minute() == 25) || (minute() == 30) || (minute() == 35) || (minute() == 40) || (minute() == 45) || (minute() == 50) || (minute() == 55) || (minute() == 00))
  {
    digitalWrite(minuteOne, LOW);
  }
  else
  {
    digitalWrite(minuteOne, HIGH);
  }

  //LED2

  if ((minute() == 1) || (minute() == 5) || (minute() == 6) || (minute() == 10) || (minute() == 11) || (minute() == 15) || (minute() == 16) || (minute() == 20) || (minute() == 21) || (minute() == 25) || (minute() == 26) ||
      (minute() == 30) || (minute() == 31) || (minute() == 35) || (minute() == 36) || (minute() == 40) || (minute() == 41) || (minute() == 45) || (minute() == 46) || (minute() == 50) || (minute() == 51) || (minute() == 55) ||
      (minute() == 56) || (minute() == 00))
  {
    digitalWrite(minuteTwo, LOW);
  }
  else
  {
    digitalWrite(minuteTwo, HIGH);
  }

  //LED3

  if ((minute() == 3) || (minute() == 4 ) || (minute() == 8) || (minute() == 9) || (minute() == 13) || (minute() == 14) || (minute() == 18) || (minute() == 19) || (minute() == 23) || (minute() == 24) || (minute() == 28) ||
      (minute() == 29) || (minute() == 33) || (minute() == 34) || (minute() == 38) || (minute() == 39) || (minute() == 43) || (minute() == 44) || (minute() == 48) || (minute() == 49) || (minute() == 53) || (minute() == 54) ||
      (minute() == 58) || (minute() == 59))
  {
    digitalWrite(minuteThree, HIGH);
  }
  else
  {
    digitalWrite(minuteThree, LOW);
  }

  //LED4

  if ((minute() == 4) || (minute() == 9) || (minute() == 14) || (minute() == 19) || (minute() == 24) || (minute() == 29) || (minute() == 34) || (minute() == 39) || (minute() == 44) || (minute() == 49) || (minute() == 54) ||
      (minute() == 59))
  {
    digitalWrite(minuteFour, HIGH);
  }
  else
  {
    digitalWrite(minuteFour, LOW);
  }


  //-----LEDs anschalten-----}



  writeRegisters();
  Serial.println(String(hour()) + ":" + String(minute()) + ":" + String(second()));
  Serial.println(String(setS));
  //digitalWrite(singleS, LOW);
  

  //interrupts();     //Interrups wieder aktivieren für Sync mit RTC
  //Serial.begin(115200);
  //oldminute = minute();

  delay(10);

}


