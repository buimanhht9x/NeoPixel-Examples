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
  // Slower:
  // Strobe(0xff, 0x77, 0x00, 10, 100, 1000);
  // Fast:
  Strobe(0xff, 0x00, 0xff, 100, 500);
}
uint32_t timeStrobe = 0;
int countStrobe = 0;
uint8_t stateStrobe = 0;

void Strobe(byte red, byte green, byte blue, int FlashDelay, int EndPause){
  switch(stateStrobe)
  {
     case 0:
        if(millis() -   timeStrobe > FlashDelay)
        {
             timeStrobe = millis();
             countStrobe ++;
             if(countStrobe >= 8)
             {
                countStrobe = 0;
                stateStrobe = 1;
                setAll(0,0,0);
                showStrip();
             }
             if(countStrobe %2 == 0)
             {
                setAll(red,green,blue);
                showStrip();
             }
             else
             {
                setAll(0,0,0);
                showStrip();
             }  
        }
        break;
     case 1:
         
         if(millis() -   timeStrobe > EndPause)
        {
             timeStrobe = millis();
             stateStrobe = 2;
        }
        break;
     case 2:
        if(millis() -   timeStrobe > FlashDelay)
        {
             timeStrobe = millis();
             countStrobe ++;
             if(countStrobe >= 8)
             {
                countStrobe = 0;
                stateStrobe = 3;
                setAll(0,0,0);
                showStrip();
             }
             if(countStrobe %2 == 0)
             {
                setAll(red,green,blue);
                showStrip();
             }
             else
             {
                setAll(0,0,0);
                showStrip();
             }  
        }
        break;
     case 3:    
         if(millis() -   timeStrobe > EndPause)
        {
             timeStrobe = millis();
             stateStrobe = 4;
        }
        break;
    case 4:
        if(millis() -   timeStrobe > FlashDelay)
        {
             timeStrobe = millis();
             countStrobe ++;
             if(countStrobe >= 23)
             {
                countStrobe = 0;
                stateStrobe = 5;
                setAll(0,0,0);
                showStrip();
             }
             if(countStrobe %2 == 0)
             {
                setAll(red,green,blue);
                showStrip();
             }
             else
             {
                setAll(0,0,0);
                showStrip();
             }  
        }
        break;
     case 5:
         
         if(millis() -   timeStrobe > EndPause)
        {
             timeStrobe = millis();
             stateStrobe = 0;
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
