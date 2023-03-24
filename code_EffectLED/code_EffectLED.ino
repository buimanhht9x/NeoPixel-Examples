#include <Adafruit_NeoPixel.h>
#define PIN 6
#define NUM_LEDS 24
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);



/////////////////////////////////////////////////////////////////////////////////////
// Color Wipe
uint32_t timeDelayColorWipe = 0;
uint8_t countEffColorWipe = 0;
#define maxEffColorWipe 13
void funcColorWipe()
{
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
  static uint8_t countNumLed = 0;
  if(millis()  - timeDelayColorWipe > SpeedDelay)
  {
    timeDelayColorWipe = millis();
    setPixel(countNumLed, red, green, blue);
    showStrip();
    countNumLed++;
    if(countNumLed >= NUM_LEDS)
    {
      countNumLed =0;
      countEffColorWipe ++;
      if(countEffColorWipe > maxEffColorWipe)
        countEffColorWipe = 0;
    }
  }
}
/////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////
// Effect Cylon
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
/////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////
// Fade In Out
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

////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////
// Effect Fire
uint32_t timeFire = 0;
int countFire = 0;
uint8_t stateFire = 0;

void Fire(int Cooling, int Sparking, int SpeedDelay) {
  static byte heat[NUM_LEDS];
  int cooldown;
  switch(stateFire)
  {
    case 0:
        // Step 1.  Cool down every cell a little
       for( int i = 0; i < NUM_LEDS; i++) 
       {
           cooldown = random(0, ((Cooling * 10) / NUM_LEDS) + 2);
            
           if(cooldown>heat[i]) 
           {
              heat[i]=0;
           } 
           else 
           {
              heat[i]=heat[i]-cooldown;
           }
       }
      
      // Step 2.  Heat from each cell drifts 'up' and diffuses a little
      for( int k= NUM_LEDS - 1; k >= 2; k--) {
        heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
      }
        
      // Step 3.  Randomly ignite new 'sparks' near the bottom
      if( random(255) < Sparking ) {
        int y = random(7);
        heat[y] = heat[y] + random(160,255);
        //heat[y] = random(160,255);
      }
      if(millis() -   timeFire > SpeedDelay)
      {
         timeFire = millis();
         countFire ++;
         if(countFire >= NUM_LEDS)
         {
            countFire = 0;
            stateFire = 1;
              
         }
      } 
      setPixelHeatColor(countFire, heat[countFire] );
      showStrip();  
      break;
    case 1:
      if(millis() -   timeFire > SpeedDelay)
      {
          timeFire = millis();
          stateFire = 0;
      }
      break;
  }
}


void setPixelHeatColor (int Pixel, byte temperature) {
  // Scale 'heat' down from 0-255 to 0-191
  byte t192 = round((temperature/255.0)*191);
 
  // calculate ramp up from
  byte heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252
 
  // figure out which third of the spectrum we're in:
  if( t192 > 0x80) {                     // hottest
    setPixel(Pixel, 255, 255, heatramp);
  } else if( t192 > 0x40 ) {             // middle
    setPixel(Pixel, 255, heatramp, 0);
  } else {                               // coolest
    setPixel(Pixel, heatramp, 0, 0);
  }
}


/////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////
// Effect Rainbow Twinkle
uint32_t timeTwinkleRandom = 0;
int countTwinkleRandom = 0;
uint8_t stateTwinkleRandom = 0;
void TwinkleRandom(int Count, int SpeedDelay) {
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

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Effect Running Lights
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

///////////////////////////////////////////////////////////////////////////////////////////////
// Sparkle
uint32_t timeSparkle = 0;
int countSparkle = 0;
uint8_t stateSparkle = 0;
void Sparkle(byte red, byte green, byte blue, int SpeedDelay) {
  if(millis() -   timeSparkle > SpeedDelay)
  {
          timeSparkle = millis();
          int Pixel = random(NUM_LEDS);
          setPixel(Pixel,red,green,blue);
          showStrip();

          Pixel = random(NUM_LEDS);
          setPixel(Pixel,red,green,blue);
          showStrip();

          Pixel = random(NUM_LEDS);
          setPixel(Pixel,red,green,blue);
          showStrip();
          
          setPixel(Pixel,0,0,0);
  }
}

////////////////////////////////////////////////////////////////////////////////////////
// Strobe
// Nhaý flash 
// Strobe(0xff, 0x00, 0xff, 100, 500);
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
//////////////////////////////////////////////////////////////////////////////
// RainBow cycle
uint32_t timeRainbowCycle = 0;
int countRainbowCycle = 0;

void rainbowCycle(int SpeedDelay) {
  byte *c;
  uint16_t i, j;
  if(millis() -   timeRainbowCycle > SpeedDelay)
  {
          timeRainbowCycle = millis();
          countRainbowCycle++;
          if(countRainbowCycle >=  256*5)
          {
              countRainbowCycle = 0;
          }
          for(i=0; i< NUM_LEDS; i++) 
          {
              c=Wheel(((i * 256 / NUM_LEDS) + countRainbowCycle) & 255);
              setPixel(i, *c, *(c+1), *(c+2));
          }
          showStrip();
  }
}
byte * Wheel(byte WheelPos) {
  static byte c[3];
  
  if(WheelPos < 85) {
   c[0]=WheelPos * 3;
   c[1]=255 - WheelPos * 3;
   c[2]=0;
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   c[0]=255 - WheelPos * 3;
   c[1]=0;
   c[2]=WheelPos * 3;
  } else {
   WheelPos -= 170;
   c[0]=0;
   c[1]=WheelPos * 3;
   c[2]=255 - WheelPos * 3;
  }
  return c;
}
/////////////////////////////////////////////////////////////////////////////////////////////////




// Function time run
#define maxEff      29
uint8_t eff = 9;
uint32_t timeRunChange = 0;
uint8_t countTimeRun = 0;
void timeRunChangeMode(uint8_t Time)
{
    if(millis() - timeRunChange > 1000)
    {
       timeRunChange = millis();
       countTimeRun ++;  
       Serial.println( countTimeRun);   
    }
    if(countTimeRun > Time)
    {
         countTimeRun = 0;
         eff++;
         if(eff >= maxEff)
            eff = 0;
         // Count countEffColorWipe
         countEffColorWipe = 0;  
         // Count countEffFadeInOut
         countEffFadeInOut = 0;       
    }
}

//////////////////////////////////////////////////////////////

uint32_t timeBlink1s = 0;
void blinkled()
{ 
   static uint8_t  t =0;
   if(millis() - timeBlink1s > 500)
   {
       t = 1-t;
       timeBlink1s = millis();
       digitalWrite(LED_BUILTIN, t);
   }
}

void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
    pinMode(LED_BUILTIN, OUTPUT);
}



void loop() {
  // put your main code here, to run repeatedly:
  blinkled();
  switch(eff)
  {
    case 0:
      funcColorWipe();
      timeRunChangeMode(5);
      break;
     Fire(22,120,2);
    case 1:
      CylonBounce(0xff, 0x00, 0, 6, 10, 50);
      timeRunChangeMode(5);
      break;
    case 2:
      CylonBounce(0x00, 0xff, 0, 6, 10, 50);
      timeRunChangeMode(5);
      break;
    case 3:
      CylonBounce(0x00, 0x00, 0xff, 6, 10, 50);
      timeRunChangeMode(5);
      break;
    case 4:
      CylonBounce(0xff, 0xff, 0, 6, 10, 50);
      timeRunChangeMode(5);
      break;
    case 5:
      CylonBounce(0x00, 0xff, 0xff, 6, 10, 50);
      timeRunChangeMode(5);
      break;
    case 6:
      CylonBounce(0xff, 0x00, 0xff, 6, 10, 50);
      timeRunChangeMode(5);
      break;
    case 7:
      CylonBounce(0xff, 0xff, 0xff, 6, 10, 50);
      timeRunChangeMode(5);
      break;
    case 8:
      funFadeInOut();
      timeRunChangeMode(50);
      break;
    case 9:
      Fire(22,120,2);
      timeRunChangeMode(100);
      break; 
    case 10:
      TwinkleRandom(20, 100);
      timeRunChangeMode(10);
      break; 
    case 11:
      RunningLights(0xff,0x00,0x00, 50);
      timeRunChangeMode(10);
      break; 
    case 12:
      RunningLights(0x00,0xff,0x00, 50);
      timeRunChangeMode(10);
      break; 
    case 13:
      RunningLights(0x00,0x00,0xff, 50);
      timeRunChangeMode(10);
      break; 
    case 14:
      RunningLights(0xff,0xff,0x00, 50);
      timeRunChangeMode(10);
      break; 
    case 15:
      RunningLights(0x00,0xff,0xff, 50);
      timeRunChangeMode(10);
      break; 
    case 16:
      RunningLights(0xff,0x00,0xff, 50);
      timeRunChangeMode(10);
      break; 
    case 17:
      RunningLights(0xff,0xff,0xff, 50);
      timeRunChangeMode(10);
      break; 
    case 18:
      Sparkle(0xff,0x00,0x00, 50);
      timeRunChangeMode(5);
      break;
    case 19:
      Sparkle(0x00,0xff,0x00, 50);
      timeRunChangeMode(5);
      break;
    case 20:
      Sparkle(0x00,0x00,0xff, 50);
      timeRunChangeMode(5);
      break;
    case 21:
      Sparkle(0xff,0xff,0x00, 50);
      timeRunChangeMode(5);
      break;
    case 22:
      Sparkle(0x00,0xff,0xff, 50);
     timeRunChangeMode(5);
      break;
    case 23:
      Sparkle(0xff,0x00,0xff, 50);
      timeRunChangeMode(5);
      break;
    case 24:
      Sparkle(0xff,0xff,0xff, 50);
      timeRunChangeMode(5);
      break;
    case 25:
      rainbowCycle(20);
      timeRunChangeMode(50);
      break;
    case 26:
      Strobe(0xff, 0x00, 0xff, 100, 500);
      timeRunChangeMode(10);
      break;
    case 27:
      Strobe(0x00, 0xff, 0xff, 100, 500);
      timeRunChangeMode(10);
      break;
    case 28:
      Strobe(0xff, 0xff, 0x00, 100, 500);
      timeRunChangeMode(10);
      break;
    
     
  }
  
}





void setPixel(int Pixel, byte red, byte green, byte blue) 
{
   strip.setPixelColor(Pixel, strip.Color(red, green, blue));
}
void setAll(byte red, byte green, byte blue) 
 {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue); 
  }
  showStrip();
}
void showStrip()
{
   strip.show();
}
