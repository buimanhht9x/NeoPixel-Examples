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
  RunningLights(0xff,0x00,0xff, 50);
}
uint32_t timeRunningLights = 0;
int countRunningLights = 0;
uint8_t stateRunningLights = 0;
int positionRunningLights=0;
void RunningLights(byte red, byte green, byte blue, int WaveDelay) {
  
  if(millis() -   timeRunningLights > WaveDelay)
  {
          timeRunningLights = millis();
          countRunningLights++;
          if(countRunningLights >=  NUM_LEDS*2 - 1)
          {
              countRunningLights = 0;
              positionRunningLights=0;
          }
          
          positionRunningLights++; // = 0; //Position + Rate;
          for(int i=0; i<NUM_LEDS; i++) {
            // sine wave, 3 offset waves make a rainbow!
            //float level = sin(i+Position) * 127 + 128;
            //setPixel(i,level,0,0);
            //float level = sin(i+Position) * 127 + 128;
            setPixel(i,((sin(i+positionRunningLights) * 127 + 128)/255)*red,
                   ((sin(i+positionRunningLights) * 127 + 128)/255)*green,
                   ((sin(i+positionRunningLights) * 127 + 128)/255)*blue);
          }
          showStrip();
          
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
