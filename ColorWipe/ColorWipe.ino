#include <Adafruit_NeoPixel.h>
#define PIN 6
#define NUM_LEDS 24
// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);
uint32_t timeDelay = 0;
uint8_t countNumLed = 0;
uint8_t countEffColorWipe = 0;
#define maxEff 13

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

// *** REPLACE FROM HERE ***
void loop() {
  switch(countEffColorWipe)
  {
    case 0:
       colorWipe(0x00,0xff,0x00, 50);
       break;
    case 1:
       colorWipe(0x00,0x00,0x00, 10);
       break;
    case 2:
       colorWipe(0xff,0x00,0x00, 50);
       break;
    case 3:
       colorWipe(0x00,0x00,0x00, 10);
       break;
    case 4:
       colorWipe(0x00,0x00,0xff, 50);
       break;
    case 5:
       colorWipe(0x00,0x00,0x00, 10);
       break;
    case 6:
       colorWipe(0xff,0xff,0x00, 50);
       break;
    case 7:
       colorWipe(0x00,0x00,0x00, 10);
       break;
    case 8:
       colorWipe(0x00,0xff,0xff, 50);
       break;
    case 9:
       colorWipe(0x00,0x00,0x00, 10);
       break;
    case 10:
       colorWipe(0xff,0x00,0xff, 50);
       break;
    case 11:
       colorWipe(0x00,0x00,0x00, 10);
       break;
    case 12:
       colorWipe(0xff,0xff,0xff, 50);
       break;
    case 13:
       colorWipe(0x00,0x00,0x00, 10);
       break;
  }
}

void colorWipe(byte red, byte green, byte blue, int SpeedDelay) {
  
  if(millis()  - timeDelay > SpeedDelay)
  {
    timeDelay = millis();
    setPixel(countNumLed, red, green, blue);
    showStrip();
    countNumLed++;
    if(countNumLed >= NUM_LEDS)
    {
      countNumLed =0;
      countEffColorWipe ++;
      if(countEffColorWipe > maxEff)
        countEffColorWipe = 0;
    }
  }
}

// *** REPLACE TO HERE ***

void showStrip() {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   strip.show();
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   FastLED.show();
 #endif
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H 
   // NeoPixel
   strip.setPixelColor(Pixel, strip.Color(red, green, blue));
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H 
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
 #endif
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue); 
  }
  showStrip();
}
