## arduino_wordclock

this repo contains code for a [QLOCKTWO](https://qlocktwo.com/de/)-style clock with a German layout.
There are two versions of code for differents hardware-attemps:

<B>1: WS2812B based hardware</B> 
* using adressable led-stripes with the [fastled](https://github.com/FastLED/FastLED)-library
* more complex and expensive build due to light-segment-seperation with continous led-strip

<B>2: normal LEDs and bitshift-ICs</B>
* using standard LEDs and controlling them via a bitshift-IC like a 74HC595
* cheaper build, can be easyily realized with provided 3D-printable frontplate and an [IKEA RIBBA](https://www.ikea.com/de/de/p/ribba-rahmen-weiss-00378403/) picture frame
