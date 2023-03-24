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
  funFadeInOut();
}
uint8_t countEffFadeInOut = 0;
#define maxEffFadeInOut 7
void funFadeInOut()
{
  switch(countEffFadeInOut)
  {
    case 0:
       FadeInOut(0xff, 0x00, 0x00);
       break;
    case 1:
       FadeInOut(0x00, 0xff, 0x00);
       break;
    case 2:
       FadeInOut(0x00, 0x00, 0xff);
       break;
    case 3:
       FadeInOut(0xff, 0xff, 0x00);
       break;
    case 4:
       FadeInOut(0x00, 0xff, 0xff);
       break;
    case 5:
       FadeInOut(0xff, 0x00, 0xff);
       break;
    case 6:
       FadeInOut(0xff, 0xff, 0xff);
       break;
  }
}


uint32_t timeFadeInOut = 0;
int countFadeInOut = 0;
uint8_t stateFadeInOut = 0;
#define timeDelayFadeInOut   2
void FadeInOut(byte red, byte green, byte blue){
  float r, g, b;
  switch(stateFadeInOut)
  {
     case 0:
        if(millis() -   timeFadeInOut > timeDelayFadeInOut)
        {
           timeFadeInOut = millis();
           countFadeInOut++;
           if(countFadeInOut >= 255)
           {
              stateFadeInOut = 1;
           } 
        }
        r = (countFadeInOut/256.0)*red;
        g = (countFadeInOut/256.0)*green;
        b = (countFadeInOut/256.0)*blue;
        setAll(r,g,b);
        showStrip();
        break;
     case 1:
        if(millis() -   timeFadeInOut > 10)
        {
           timeFadeInOut = millis();
           countFadeInOut = countFadeInOut - 2;
           if(countFadeInOut <= 0)
           {
              stateFadeInOut = 0;
              countEffFadeInOut ++;
              if(countEffFadeInOut >= maxEffFadeInOut)
                countEffFadeInOut = 0;
           } 
        }
        r = (countFadeInOut/256.0)*red;
        g = (countFadeInOut/256.0)*green;
        b = (countFadeInOut/256.0)*blue;
        setAll(r,g,b);
        showStrip();
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
