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

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

// *** REPLACE FROM HERE ***
void loop() {
  TwinkleRandom(20, 100, false);
}
uint32_t timeTwinkleRandom = 0;
int countTwinkleRandom = 0;
uint8_t stateTwinkleRandom = 0;
void TwinkleRandom(int Count, int SpeedDelay, boolean OnlyOne) {
  switch(stateTwinkleRandom)
  {
     case 0:
       setAll(0,0,0); 
       stateTwinkleRandom = 1;
       break;
     case 1:
       if(millis() -   timeTwinkleRandom > SpeedDelay)
       {
          timeTwinkleRandom = millis();
          countTwinkleRandom ++;
          if(countTwinkleRandom >= Count)
          {
              countTwinkleRandom = 0;
              stateTwinkleRandom = 2;
          }
          setPixel(random(NUM_LEDS),random(0,255),random(0,255),random(0,255));
          showStrip();
           if(OnlyOne)
             setAll(0,0,0); 
        }
        break;
     case 2:
       if(millis() -   timeTwinkleRandom > SpeedDelay)
       {
          timeTwinkleRandom = millis();
          stateTwinkleRandom = 0;
       }
       break;
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
