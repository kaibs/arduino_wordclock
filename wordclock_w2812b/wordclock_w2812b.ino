
#include <bitswap.h>
#include <chipsets.h>
#include <color.h>
#include <colorpalettes.h>
#include <colorutils.h>
#include <controller.h>
#include <cpp_compat.h>
#include <dmx.h>
#include <FastLED.h>
#include <fastled_config.h>
#include <fastled_delay.h>
#include <fastled_progmem.h>
#include <fastpin.h>
#include <fastspi.h>
#include <fastspi_bitbang.h>
#include <fastspi_dma.h>
#include <fastspi_nop.h>
#include <fastspi_ref.h>
#include <fastspi_types.h>
#include <hsv2rgb.h>
#include <led_sysdefs.h>
#include <lib8tion.h>
#include <noise.h>
#include <pixelset.h>
#include <pixeltypes.h>
#include <platforms.h>
#include <power_mgt.h>
#include <Streaming.h>
#include <Wire.h>
#include <Time.h>
#include <TimeLib.h>
#include <DS3232RTC.h>


//DS3231  rtc(SDA, SCL);
const int ButtonM = 2;  //Buttons für Zeiteinstellung
const int ButtonH = 3;
int buttonStateM = LOW;
int buttonStateH = LOW;
int readingM;
int readingH;
int previousM = LOW;
int previousH = LOW;
int LDR = A0;           //Fotowiderstand
int Sensorwert = 0;
boolean ledValues[110];
boolean leo = false;
boolean kai = false;
int oldminute = 0;

#define NUM_LEDS_PART_A 1  //Arraygrößen bestimmen
#define NUM_LEDS_PART_B 1
#define NUM_LEDS_PART_C 1
#define NUM_LEDS_PART_D 1
#define NUM_LEDS_PART_E 110

CRGB led1[NUM_LEDS_PART_A];
CRGB led2[NUM_LEDS_PART_B];
CRGB led3[NUM_LEDS_PART_C];
CRGB led4[NUM_LEDS_PART_D];
CRGB leds[NUM_LEDS_PART_E];


void setup() {

  //-----Pinout initialisieren-----//

  FastLED.addLeds<WS2812B, 4, GRB>(led1, NUM_LEDS_PART_A);
  FastLED.addLeds<WS2812B, 5, GRB>(led2, NUM_LEDS_PART_B);
  FastLED.addLeds<WS2812B, 6, GRB>(led3, NUM_LEDS_PART_C);
  FastLED.addLeds<WS2812B, 7, GRB>(led4, NUM_LEDS_PART_D);
  FastLED.addLeds<WS2812B, 8, GRB>(leds, NUM_LEDS_PART_E);
  pinMode(ButtonH, INPUT);
  pinMode(ButtonM, INPUT);
  hourFormat12();
  oldminute = minute();

  //setTime(12, 45, 00, 20, 7, 2017);


  //--------------------------------serielle Kommunikation-------------------//


  Serial.begin(115200);
  setSyncProvider(RTC.get);
  if (timeStatus() != timeSet) //Notification IC2-Kommunikation bei Stream-Beginn
    Serial.println("Unable to sync with the RTC");
  else
    Serial.println("RTC has set the system time");
}


//-----LEDs schalten-----//

boolean setLED(int i) {

  for (int i = 0; i < 110; i++) {
    ledValues[i] = false;
  }

  //-----Strukturwörter-----//

  //UHR
  if ((minute() < 5))
  {
    ledValues[107] = true;
    ledValues[108] = true;
    ledValues[109] = true;
  }

  //VOR
  if ((minute() > 24) && (minute() < 30) || (minute() > 49))
  {
    ledValues[33] = true;
    ledValues[34] = true;
    ledValues[35] = true;
  }

  //NACH
  if ((minute() > 4) && (minute() < 25) || (minute() > 34) && (minute() < 45))
  {
    ledValues[40] = true;
    ledValues[41] = true;
    ledValues[42] = true;
    ledValues[43] = true;
  }

  //-----Zahlen-----//

  //EINS
  if (((minute() < 25) && (hourFormat12() == 1) || (minute() > 24) && (hourFormat12() == 0) || (minute() > 24) && (hourFormat12() == 12)) && !ledValues[107])
  {
    ledValues[55] = true;
    ledValues[56] = true;
    ledValues[57] = true;
    ledValues[58] = true;
  }

  //EIN
  if ((hourFormat12() == 1) && ledValues[107]) {
    ledValues[55] = true;
    ledValues[56] = true;
    ledValues[57] = true;
  }

  //ZWEI
  if ((minute() < 25) && (hourFormat12() == 2) || (minute() > 24) && (hourFormat12() == 1))
  {
    ledValues[62] = true;
    ledValues[63] = true;
    ledValues[64] = true;
    ledValues[65] = true;
  }

  //DREI
  if ((minute() < 25) && (hourFormat12() == 3) || (minute() > 24) && (hourFormat12() == 2))
  {
    ledValues[73] = true;
    ledValues[74] = true;
    ledValues[75] = true;
    ledValues[76] = true;
  }

  //VIER
  if ((minute() < 25) && (hourFormat12() == 4) || (minute() > 24) && (hourFormat12() == 3))
  {
    ledValues[66] = true;
    ledValues[67] = true;
    ledValues[68] = true;
    ledValues[69] = true;
  }

  //FÜNF
  if ((minute() < 25) && (hourFormat12() == 5) || (minute() > 24) && (hourFormat12() == 4))
  {
    ledValues[44] = true;
    ledValues[45] = true;
    ledValues[46] = true;
    ledValues[47] = true;
  }

  //SECHS
  if ((minute() < 25) && (hourFormat12() == 6) || (minute() > 24) && (hourFormat12() == 5))
  {
    ledValues[77] = true;
    ledValues[78] = true;
    ledValues[79] = true;
    ledValues[80] = true;
    ledValues[81] = true;
  }

  //SIEBEN
  if ((minute() < 25) && (hourFormat12() == 7) || (minute() > 24) && (hourFormat12() == 6))
  {
    ledValues[93] = true;
    ledValues[94] = true;
    ledValues[95] = true;
    ledValues[96] = true;
    ledValues[97] = true;
    ledValues[98] = true;
  }

  //ACHT
  if ((minute() < 25) && (hourFormat12() == 8) || (minute() > 24) && (hourFormat12() == 7))
  {
    ledValues[84] = true;
    ledValues[85] = true;
    ledValues[86] = true;
    ledValues[87] = true;
  }

  //NEUN
  if ((minute() < 25) && (hourFormat12() == 9) || (minute() > 24) && (hourFormat12() == 8))
  {
    ledValues[99] = true;
    ledValues[100] = true;
    ledValues[101] = true;
    ledValues[102] = true;
  }

  //ZEHN
  if ((minute() < 25) && (hourFormat12() == 10) || (minute() > 24) && (hourFormat12() == 9))
  {
    ledValues[103] = true;
    ledValues[104] = true;
    ledValues[105] = true;
    ledValues[106] = true;
  }

  //ELF
  if ((minute() < 25) && (hourFormat12() == 11) || (minute() > 24) && (hourFormat12() == 10))
  {
    ledValues[47] = true;
    ledValues[48] = true;
    ledValues[49] = true;
  }

  //ZWÖLF
  if ((minute() < 25) && (hourFormat12() == 12) || (minute() > 24) && (hourFormat12() == 11))
  {
    ledValues[88] = true;
    ledValues[89] = true;
    ledValues[90] = true;
    ledValues[91] = true;
    ledValues[92] = true;
  }

  //-----Zeitunterteilung-----//

  //FÜNF
  if ((minute() > 4) && (minute() < 10) || (minute() > 24) && (minute() < 30) || (minute() > 34) && (minute() < 40) || (minute() > 54))
  {
    ledValues[0] = true;
    ledValues[1] = true;
    ledValues[2] = true;
    ledValues[3] = true;
  }

  //ZEHN
  if ((minute() > 9) && (minute() < 15) || (minute() > 39) && (minute() < 45) || (minute() > 49) && (minute() < 55))
  {
    ledValues[11] = true;
    ledValues[12] = true;
    ledValues[13] = true;
    ledValues[14] = true;
  }

  //VIERTEL
  if ((minute() > 14) && (minute() < 20) || (minute() > 44) && (minute() < 50)) //Intervall für Dreiviertel ebenfalls eingeschlossen
  {
    ledValues[22] = true;
    ledValues[23] = true;
    ledValues[24] = true;
    ledValues[25] = true;
    ledValues[26] = true;
    ledValues[27] = true;
    ledValues[28] = true;
  }

  //ZWANZIG
  if ((minute() > 19) && (minute() < 25))
  {
    ledValues[15] = true;
    ledValues[16] = true;
    ledValues[17] = true;
    ledValues[18] = true;
    ledValues[19] = true;
    ledValues[20] = true;
    ledValues[21] = true;
  }

  //HALB
  if ((minute() > 24) && (minute() < 45))
  {
    ledValues[51] = true;
    ledValues[52] = true;
    ledValues[53] = true;
    ledValues[54] = true;
  }

  //DREI(VIERTEL)
  if ((minute() > 44) && (minute() < 50))
  {
    for (int j = 22; j < 33; j++) {
      ledValues[j] = true;
    }
  }

  //Geburttage

  if ((day() == 26) && (month() == 05)) {
    ledValues[36] = true;
    ledValues[37] = true;
    ledValues[38] = true;
    ledValues[39] = true;
    leo = true;
  }
  else
  {
    leo = false;
  }

  if ((day() == 20) && (month() == 07)) {
    ledValues[36] = true;
    ledValues[37] = true;
    ledValues[38] = true;
    ledValues[39] = true;
    kai = true;
  }
  else
  {
    kai = false;
  }

  return ledValues[i];

}

void loop() {

  //-----Uhr stellen----//



  //-----Helligkeitsregelung-----//

  int Helligkeit = analogRead(LDR); //Definition Helligkeit
  Serial.print("Sensorwert = ");    //serielle Ausgabe Sensorwerte
  Serial.println(Helligkeit);
  Serial.print("   ");
  Serial.print(" hour : ");
  Serial.print(hourFormat12());
  Serial.print(" minute : ");
  Serial.print(minute());
  Serial.print(" second : ");
  Serial.print(second());


  int Leuchtstaerke = 50 || 100 || 255;   //definiere Variable (fixiert auf feste Werte um ständige Änderung zu vermeiden)


  if (Helligkeit < 100)      //Regelung Dimmung
  {
    Leuchtstaerke = 100;
  }
  if (Helligkeit < 50)
  {
    Leuchtstaerke = 50;
  }
  else
  {
    Leuchtstaerke = 255;
  }

  //-----Uhrzeitjustierung-----//

  previousM = buttonStateM;
  previousH = buttonStateH;
  //buttonStateH = digitalRead(ButtonH);
  //buttonStateM = digitalRead(ButtonM);


  if (digitalRead(ButtonM) == HIGH) {
    Serial.print(" BM! ");
    delay(50);
    if (digitalRead(ButtonM) == HIGH) {
      setTime( hourFormat12(), minute() + 1, second(), day(), month(), year());
      RTC.set(now());
      delay(400);
    }

  }

  if (digitalRead(ButtonH) == HIGH) {
    Serial.print(" BH! ");
    delay(50);
    if (digitalRead(ButtonH) == HIGH) {
      setTime( hourFormat12() + 1, minute(), second(), day(), month(), year());
      RTC.set(now());
      delay(400);
    }

  }

  //if (digitalRead(ButtonM) == HIGH && previousM == LOW){
  //  previousM = digitalRead(ButtonM);
  //  delay(10);
  //  //buttonStateM = digitalRead(ButtonM);
  //  if(digitalRead(ButtonM) == HIGH && previousM == HIGH){
  //    if(digitalRead(ButtonM) == LOW){
  //        setTime( hourFormat12(), minute() + 1, second(), day(), month(), year());
  //        RTC.set(now());
  //        previousM = LOW;
  //        buttonStateM = LOW;
  //        Serial.print(" BM: 1 ");
  //     }
  //  }
  //}
  //
  //if (digitalRead(ButtonH) == HIGH && previousH == LOW){
  //  previousH = digitalRead(ButtonH);
  //  delay(10);
  //  //buttonStateH = digitalRead(ButtonH);
  //  if(digitalRead(ButtonH) == HIGH && previousH == HIGH){
  //     if(digitalRead(ButtonH) == LOW){
  //        setTime( hourFormat12() +1, minute(), second(), day(), month(), year());
  //        RTC.set(now());
  //        previousH = LOW;
  //        buttonStateH = LOW;
  //        Serial.print(" BH: 1 ");
  //     }
  //  }
  //}


  //-----Zeitsteuereung-----//

  //if( minute() != oldminute){
  Serial.print(" Changing ");
  //Serial.end();
  //noInterrupts();       //interrups unterbrechen, da ws2812b zeitsensitiv


  //-----Minutenanzeige-----//


  //LED1

  if ((minute() == 5) || (minute() == 10) || (minute() == 15) || (minute() == 20) || (minute() == 25) || (minute() == 30) || (minute() == 35) || (minute() == 40) || (minute() == 45) || (minute() == 50) || (minute() == 55) || (minute() == 00))
  {
    led1[0] = CRGB::Black;
    FastLED.show();
  }
  else
  {
    led1[0].setRGB( Leuchtstaerke, Leuchtstaerke, Leuchtstaerke);
    FastLED.show();
  }

  //LED2

  if ((minute() == 1) || (minute() == 5) || (minute() == 6) || (minute() == 10) || (minute() == 11) || (minute() == 15) || (minute() == 16) || (minute() == 20) || (minute() == 21) || (minute() == 25) || (minute() == 26) ||
      (minute() == 30) || (minute() == 31) || (minute() == 35) || (minute() == 36) || (minute() == 40) || (minute() == 41) || (minute() == 45) || (minute() == 46) || (minute() == 50) || (minute() == 51) || (minute() == 55) ||
      (minute() == 56) || (minute() == 00))
  {
    led2[0] = CRGB::Black;
    FastLED.show();
  }
  else
  {
    led2[0].setRGB( Leuchtstaerke, Leuchtstaerke, Leuchtstaerke);
    FastLED.show();
  }

  //LED3

  if ((minute() == 3) || (minute() == 4 ) || (minute() == 8) || (minute() == 9) || (minute() == 13) || (minute() == 14) || (minute() == 18) || (minute() == 19) || (minute() == 23) || (minute() == 24) || (minute() == 28) ||
      (minute() == 29) || (minute() == 33) || (minute() == 34) || (minute() == 38) || (minute() == 39) || (minute() == 43) || (minute() == 44) || (minute() == 48) || (minute() == 49) || (minute() == 53) || (minute() == 54) ||
      (minute() == 58) || (minute() == 59))
  {
    led3[0].setRGB( Leuchtstaerke, Leuchtstaerke, Leuchtstaerke);
    FastLED.show();
  }
  else
  {
    led3[0] = CRGB::Black;
    FastLED.show();
  }

  //LED4

  if ((minute() == 4) || (minute() == 9) || (minute() == 14) || (minute() == 19) || (minute() == 24) || (minute() == 29) || (minute() == 34) || (minute() == 39) || (minute() == 44) || (minute() == 49) || (minute() == 54) ||
      (minute() == 59))
  {
    led4[0].setRGB( Leuchtstaerke, Leuchtstaerke, Leuchtstaerke);
    FastLED.show();
  }
  else
  {
    led4[0] = CRGB::Black;
    FastLED.show();
  }


  //-----LEDs anschalten-----}



  for (int i = 0; i < 36; i++) {
    if (setLED(i) == true) {
      leds[i].setRGB( Leuchtstaerke, Leuchtstaerke, Leuchtstaerke);
      FastLED.show();
    }
    else
    {
      leds[i] = CRGB::Black;
      FastLED.show();
    }
  }
  for (int i = 40; i < 110; i++) {
    if (setLED(i) == true) {
      leds[i].setRGB( Leuchtstaerke, Leuchtstaerke, Leuchtstaerke);
      FastLED.show();
    }

    else
    {
      leds[i] = CRGB::Black;
      FastLED.show();
    }
  }

  if ((ledValues[36] == true) && (leo == true)) {
    leds[36] = CRGB::Red;
    leds[37] = CRGB::Red;
    leds[38] = CRGB::Red;
    leds[39] = CRGB::Red;
    FastLED.show();
  }

  if ((ledValues[36] == true) && (kai == true)) {
    leds[36] = CRGB::Yellow;
    leds[37] = CRGB::Yellow;
    leds[38] = CRGB::Yellow;
    leds[39] = CRGB::Yellow;
    FastLED.show();
  }

  //interrupts();     //Interrups wieder aktivieren für Sync mit RTC
  //Serial.begin(115200);
  //oldminute = minute();



}


