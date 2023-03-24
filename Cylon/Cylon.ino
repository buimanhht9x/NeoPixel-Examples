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
  CylonBounce(0xff, 0x00, 0, 6, 10, 50);
}


uint32_t timeSpeedDelayCylonBounce = 0;
uint32_t timeReturnDelayCylonBounce = 0;
uint16_t countSpeedDelay = 0;
uint16_t countReturnDelay = 0;
uint8_t stateCylonBounce = 0;
void CylonBounce(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay){

 
  switch(stateCylonBounce)
  {
    case 0:
      if(millis() - timeSpeedDelayCylonBounce > SpeedDelay)
      {
          timeSpeedDelayCylonBounce = millis();
          countSpeedDelay++;
          if(countSpeedDelay >= NUM_LEDS-EyeSize-2)
          { 
            stateCylonBounce = 1;
            timeReturnDelayCylonBounce = millis();
            timeSpeedDelayCylonBounce = millis();
          }
          setAll(0,0,0);
          setPixel(countSpeedDelay, red/10, green/10, blue/10);
          for(int j = 1; j <= EyeSize; j++) {
            setPixel(countSpeedDelay+j, red, green, blue); 
          }
          setPixel(countSpeedDelay+EyeSize+1, red/10, green/10, blue/10);
          showStrip();
      }
      break;
    case 1:
      if(millis() - timeReturnDelayCylonBounce > ReturnDelay)
      {
          timeReturnDelayCylonBounce = millis();
          stateCylonBounce = 2;
      }
      break;
    case 2:
      if(millis() - timeSpeedDelayCylonBounce > SpeedDelay)
      {
          timeSpeedDelayCylonBounce = millis();
          countSpeedDelay--;
          if(countSpeedDelay <= 0)
          {
              stateCylonBounce = 3;
              timeReturnDelayCylonBounce = millis();
          }
          setAll(0,0,0);
          setPixel(countSpeedDelay, red/10, green/10, blue/10);
          for(int j = 1; j <= EyeSize; j++) {
            setPixel(countSpeedDelay+j, red, green, blue); 
          }
          setPixel(countSpeedDelay+EyeSize+1, red/10, green/10, blue/10);
          showStrip();
      }
      break;
    case 3:
      if(millis() - timeReturnDelayCylonBounce > ReturnDelay)
      {
          timeReturnDelayCylonBounce = millis();
          timeSpeedDelayCylonBounce = millis();
          stateCylonBounce = 0;
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
