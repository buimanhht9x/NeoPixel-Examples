#include <arduino-timer.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <stdint.h>
#include <avr/wdt.h>

// Biến báo done hiệu ứng,  1 : done
uint8_t done_effectLED_RGB = 0;
uint8_t done_effectLED_rainbow = 0;
uint8_t done_effectLED_fade = 0;
uint8_t done_effectLED_sangdan1 = 0;
uint8_t done_effectLED_sangdan2 = 0;
/*
uint8_t func_effectLED_RGB(uint32_t time_run,uint32_t animationDelay) ;
uint8_t func_effectLED_rainbow(uint32_t time_run,uint16_t animationDelay, uint8_t animationDirection) ;
uint8_t func_effectLED_fade(uint32_t time_run,uint16_t animationDelay) ;
void func_effectLED_sangdan1(uint32_t time_run,uint16_t animationDelay, uint8_t animationDirection) ;
void func_effectLED_sangdan2(uint32_t time_run,uint16_t animationDelay, uint8_t animationDirection) ;
*/

//enum cho hiệu ứng LED
typedef  enum
{
  state1,
  state2,
  state3,
  state4,
  state5,
  state6,
  state7,
  state8,
  state9
}STATE;
STATE state = state1;
STATE stateBomDaiTo = state1;
/*
 * Input bao gom
 * + 24 I/O
 * + 4 I/O kích biến tần bơm chìm tạo sóng nối chung 
 * + 4 I/O kích biến tần bơm đài to nối chung
 * + 4 I/O kích biến tần bơm đài nhỏ nối chung
 */ 
// Khởi tạo biến cho 24 I/O
uint8_t IO_Van[25] = {0,22,23,24,25,26,27,28,29,30,31,32,
                    33,34,35,36,37,38,39,40,41,42,43,44,45};

// Khởi tạo biến cho 4 I/O bơm chìm tạo sóng
uint8_t IO_BomTaoSong[5] = {0, 46,47,48,49};
// Khởi tạo biến cho 4 I/O kích biến tần bơm đài to
uint8_t IO_BomDaiTo[5] = {0, 10,11,12,13};
// Khởi tạo biến cho 4 I/O kích biến tần bơm đài nhỏ
uint8_t IO_BomDaiNho[5] = {0,6 ,7,8,9};

//Khai báo enum biến tần
enum BienTan
{
  BT_BomTaoSong,
  BT_BomDaiNho,
  BT_BomDaiTo
};
enum ONOFF
{
  OFF, // 0
  ON  // 1
};
enum DIRECTION
{
  LEFTtoRIGHT, // 0
  RIGHTtoLEFT  // 1
};

// Khai báo PCA9685
Adafruit_PWMServoDriver pwmLed1 = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pwmLed2 = Adafruit_PWMServoDriver(0x41);
Adafruit_PWMServoDriver pwmLed3 = Adafruit_PWMServoDriver(0x42);
Adafruit_PWMServoDriver pwmLed4 = Adafruit_PWMServoDriver(0x43);
Adafruit_PWMServoDriver pwmLed5 = Adafruit_PWMServoDriver(0x44);
Adafruit_PWMServoDriver pwmLed6 = Adafruit_PWMServoDriver(0x45);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for(uint8_t i = 1; i< 25; i++)
  {
    pinMode(IO_Van[i], OUTPUT);
    pinMode(IO_BomTaoSong[i], OUTPUT);
    pinMode(IO_BomDaiTo[i], OUTPUT);
    pinMode(IO_BomDaiNho[i], OUTPUT);
  }
  pwmLed1.begin(); pwmLed2.begin(); pwmLed3.begin();pwmLed4.begin();pwmLed5.begin();pwmLed6.begin();
  pwmLed1.setOscillatorFrequency(27000000);pwmLed1.setPWMFreq(1000);
  pwmLed2.setOscillatorFrequency(27000000);pwmLed2.setPWMFreq(1000);
  pwmLed3.setOscillatorFrequency(27000000);pwmLed3.setPWMFreq(1000);
  pwmLed4.setOscillatorFrequency(27000000);pwmLed4.setPWMFreq(1000);
  pwmLed5.setOscillatorFrequency(27000000);pwmLed5.setPWMFreq(1000);
  pwmLed6.setOscillatorFrequency(27000000);pwmLed6.setPWMFreq(1000);
  Wire.setClock(400000);

  func_offBienTan(BT_BomTaoSong);
  func_offBienTan(BT_BomDaiNho);
  func_offBienTan(BT_BomDaiTo);
  func_setVanAll(OFF);
  func_setLedAll(0,0,0);
}

void loop() {
  func_BomTaoSong(3);
  func_BomDaiNho(4);


}

/*
 * Hàm cho bơm đài to
 * Func:        func_BomDaiTo
 * Parameters : none
 * Return :     none
*/
void func_BomDaiTo()
{
  switch(stateBomDaiTo)
  {
    case state1:
      func_effectLED_RGB(120,10);
      func_setVanAll(ON);
      func_setBienTan(BT_BomDaiTo,4);
      if(done_effectLED_RGB == 1)
      {
        done_effectLED_RGB = 0;
        stateBomDaiTo = state2;
      }
      break;
    case state2:
      func_effectLED_rainbow(60,10,RIGHTtoLEFT);
      func_setVanAll(ON);
      func_setBienTan(BT_BomDaiTo,3);
      if(done_effectLED_RGB == 1)
      {
        done_effectLED_rainbow = 0;
        stateBomDaiTo = state3;
      }
      break;
    case state3:
      func_effectLED_rainbow(60,10,LEFTtoRIGHT);
      func_setVanAll(ON);
      func_setBienTan(BT_BomDaiTo,3);
      if(done_effectLED_RGB == 1)
      {
        done_effectLED_rainbow = 0;
        stateBomDaiTo = state4;
      }
      break;
    case state4:
      func_effectLED_sangdan1(60,200,LEFTtoRIGHT);
      func_setVanAll(ON);
      func_setBienTan(BT_BomDaiTo,4);
      if(done_effectLED_RGB == 1)
      {
        done_effectLED_sangdan1 = 0;
        stateBomDaiTo = state5;
      }
      break;
    case state5:
      func_effectLED_sangdan1(60,200,RIGHTtoLEFT);
      func_setVanAll(ON);
      func_setBienTan(BT_BomDaiTo,4);
      if(done_effectLED_RGB == 1)
      {
        done_effectLED_sangdan1 = 0;
        stateBomDaiTo = state6;
      }
      break;
    case state6:
      func_effectLED_sangdan2(60,200,LEFTtoRIGHT);
      func_setVanAll(ON);
      func_setBienTan(BT_BomDaiTo,4);
      if(done_effectLED_RGB == 1)
      {
        done_effectLED_sangdan2 = 0;
        stateBomDaiTo = state7;
      }
      break;
    case state7:
      func_effectLED_sangdan2(60,200,LEFTtoRIGHT);
      func_setVanAll(ON);
      func_setBienTan(BT_BomDaiTo,4);
      if(done_effectLED_RGB == 1)
      {
        done_effectLED_sangdan2 = 0;
        stateBomDaiTo = state1;
      }
      break;
  }
}


/*
 * Hàm cho bơm chìm tạo sóng, chu kì bất tắt timeOnOff
 * Func:        func_BomTaoSong
 * Parameters : uint8_t timeOnOff (s)
 * Return :     none
*/
void func_BomTaoSong(uint8_t timeOnOff)
{
  static uint32_t count_time = 0;
  static uint8_t k = 0;
  if(millis()  - count_time > timeOnOff*1000)
  {
    count_time = millis();
    k = 1-k;
    switch(k)
    {
      case 0 :
        func_setBienTan(BT_BomTaoSong, 2);
        break;
      case 1:
        func_setBienTan(BT_BomTaoSong, 6);
        break;
    }  
  }
}



/*
 * Hàm cho bơm đài nhỏ, set mặc định level từ đầu
 * Func:        func_BomDaiNho
 * Parameters : uint8_t level
 * Return :     none
*/
void func_BomDaiNho(uint8_t level)
{
  func_setBienTan(BT_BomDaiNho, level);
}

/*
 * Hàm tắt biến tần
 * Func:        func_offBienTan
 * Parameters : uint8_t BT, uint8_t level
 * Return :     none
*/
void func_offBienTan(uint8_t BT)
{
  switch(BT)
  {
    case BT_BomTaoSong:
      for(uint8_t i = 1; i< 5; i++)
      {
        digitalWrite(IO_BomTaoSong[i], HIGH);
      }
      break;
    case BT_BomDaiNho:
      for(uint8_t i = 1; i< 5; i++)
      {
        digitalWrite(IO_BomDaiNho[i], HIGH);
      }
      break;
    case  BT_BomDaiTo:
      for(uint8_t i = 1; i< 5; i++)
      {
        digitalWrite(IO_BomDaiTo[i], HIGH);
      }
      break;  
  }
}
/*
 * Hàm set biến tần
    L1 0: 11hz
    l1 + l2 : 20Hz
    L1+ L3: 24Hz
    L1 + L4: 28HZ
    L1+L2+L3: 32Hz
    L1+L2+L4 : 36HZ
    L1+L3+l4 : 38Hz
    L1 +L2 + L3 +L4: 40 Hz
 * Func:        func_BomTaoSong
 * Parameters : uint8_t BT, uint8_t level
 * Return :     none
*/
void func_setBienTan(uint8_t BT, uint8_t level)
{
  func_offBienTan(BT);
  switch(level)
  {
    case 0:
      func_offBienTan(BT); 
      break;
    case 1: 
      switch(BT)
      {
        case BT_BomTaoSong:
          digitalWrite(IO_BomTaoSong[1], 0);
          digitalWrite(IO_BomTaoSong[2], 0);
          break;
        case BT_BomDaiTo:
          digitalWrite(IO_BomDaiTo[1], 0);
          digitalWrite(IO_BomDaiTo[2], 0);
          break;
        case BT_BomDaiNho:
          digitalWrite(IO_BomDaiNho[1], 0);
          digitalWrite(IO_BomDaiNho[2], 0);
          break;
      }
      break;
    case 2:
      switch(BT)
      {
        case BT_BomTaoSong:
          digitalWrite(IO_BomTaoSong[1], 0);
          digitalWrite(IO_BomTaoSong[3], 0);
          break;
        case BT_BomDaiTo:
          digitalWrite(IO_BomDaiTo[1], 0);
          digitalWrite(IO_BomDaiTo[3], 0);
          break;
        case BT_BomDaiNho:
          digitalWrite(IO_BomDaiNho[1], 0);
          digitalWrite(IO_BomDaiNho[3], 0);
          break;
      } 
      break;
    case 3:
      switch(BT)
      {
        case BT_BomTaoSong:
          digitalWrite(IO_BomTaoSong[1], 0);
          digitalWrite(IO_BomTaoSong[4], 0);
          break;
        case BT_BomDaiTo:
          digitalWrite(IO_BomDaiTo[1], 0);
          digitalWrite(IO_BomDaiTo[4], 0);
          break;
        case BT_BomDaiNho:
          digitalWrite(IO_BomDaiNho[1], 0);
          digitalWrite(IO_BomDaiNho[4], 0);
          break;
      }
      break;
    case 4:
      switch(BT)
      {
        case BT_BomTaoSong:
          digitalWrite(IO_BomTaoSong[1], 0);
          digitalWrite(IO_BomTaoSong[2], 0);
          digitalWrite(IO_BomTaoSong[3], 0);
          break;
        case BT_BomDaiTo:
          digitalWrite(IO_BomDaiTo[1], 0);
          digitalWrite(IO_BomDaiTo[2], 0);
          digitalWrite(IO_BomDaiTo[3], 0);
          break;
        case BT_BomDaiNho:
          digitalWrite(IO_BomDaiNho[1], 0);
          digitalWrite(IO_BomDaiNho[2], 0);
          digitalWrite(IO_BomDaiNho[3], 0);
          break;
      } 
      break;
    case 5:
      switch(BT)
      {
        case BT_BomTaoSong:
          digitalWrite(IO_BomTaoSong[1], 0);
          digitalWrite(IO_BomTaoSong[2], 0);
          digitalWrite(IO_BomTaoSong[4], 0);
          break;
        case BT_BomDaiTo:
          digitalWrite(IO_BomDaiTo[1], 0);
          digitalWrite(IO_BomDaiTo[2], 0);
          digitalWrite(IO_BomDaiTo[4], 0);
          break;
        case BT_BomDaiNho:
          digitalWrite(IO_BomDaiNho[1], 0);
          digitalWrite(IO_BomDaiNho[2], 0);
          digitalWrite(IO_BomDaiNho[4], 0);
          break;
      } 
      break;
    case 6:
      switch(BT)
      {
        case BT_BomTaoSong:
          digitalWrite(IO_BomTaoSong[1], 0);
          digitalWrite(IO_BomTaoSong[3], 0);
          digitalWrite(IO_BomTaoSong[4], 0);
          break;
        case BT_BomDaiTo:
          digitalWrite(IO_BomDaiTo[1], 0);
          digitalWrite(IO_BomDaiTo[3], 0);
          digitalWrite(IO_BomDaiTo[4], 0);
          break;
        case BT_BomDaiNho:
          digitalWrite(IO_BomDaiNho[1], 0);
          digitalWrite(IO_BomDaiNho[3], 0);
          digitalWrite(IO_BomDaiNho[4], 0);
          break;
      }
      break;
    case 7:
      switch(BT)
      {
        case BT_BomTaoSong:
          digitalWrite(IO_BomTaoSong[1], 0);
          digitalWrite(IO_BomTaoSong[2], 0);
          digitalWrite(IO_BomTaoSong[3], 0);
          digitalWrite(IO_BomTaoSong[4], 0);
          break;
        case BT_BomDaiTo:
          digitalWrite(IO_BomDaiTo[1], 0);
          digitalWrite(IO_BomDaiTo[2], 0);
          digitalWrite(IO_BomDaiTo[3], 0);
          digitalWrite(IO_BomDaiTo[4], 0);
          break;
        case BT_BomDaiNho:
          digitalWrite(IO_BomDaiNho[1], 0);
          digitalWrite(IO_BomDaiNho[2], 0);
          digitalWrite(IO_BomDaiNho[3], 0);
          digitalWrite(IO_BomDaiNho[4], 0);
          break;
      }
      break;
  }
}


/*
 * Hàm RESET Led
 * Func:        func_resetLED
 * Parameters : none
 * Return :     none
*/
void func_resetLED()
{
  for(int i = 0; i<=15;i++)
  {
    pwmLed1.setPWM(i, 0, 0 );
    pwmLed2.setPWM(i, 0, 0 );
    pwmLed3.setPWM(i, 0, 0 );
    pwmLed4.setPWM(i, 0, 0 );
    pwmLed5.setPWM(i, 0, 0 );
    pwmLed6.setPWM(i, 0, 0 );  
  }
}

/*
 * Hàm RESET Van
 * Func:        func_resetVan
 * Parameters : none
 * Return :     none
*/
void func_resetVan()
{
  for(uint8_t i = 1; i< 25; i++)
  {
    digitalWrite(IO_Van[i], HIGH);
  }
}


/*
 * Hàm setLed , 0 đến 255, mapping sang dải 50 đến 600, 
 * Func:        func_setLed
 * Parameters : int led, int R,int G, int B
 * Return :     none
*/
void func_setLed(int led, int R,int G, int B)
{
  static uint16_t val_Red = 0, val_Green = 0,val_Blue = 0;
  val_Red = map(R,0,255,50,600);
  val_Green = map(G,0,255,50,600);
  val_Blue = map(B,0,255,50,600);
  switch(led)
  {
    case 1:
      pwmLed1.setPWM(0, 0, val_Red );
      pwmLed1.setPWM(1, 0, val_Green );
      pwmLed1.setPWM(2, 0, val_Blue );
      break;
    case 2:
      pwmLed1.setPWM(3, 0, val_Red );
      pwmLed1.setPWM(4, 0, val_Green );
      pwmLed1.setPWM(5, 0, val_Blue );
      break;
    case 3:
      pwmLed1.setPWM(6, 0, val_Red );
      pwmLed1.setPWM(7, 0, val_Green);
      pwmLed1.setPWM(8, 0, val_Blue );  
      break;
    case 4:
      pwmLed1.setPWM(9, 0, val_Red );
      pwmLed1.setPWM(10, 0, val_Green );
      pwmLed1.setPWM(11, 0, val_Blue );  
      break;
    case 5:
      pwmLed2.setPWM(0, 0, val_Red );
      pwmLed2.setPWM(1, 0, val_Green );
      pwmLed2.setPWM(2, 0, val_Blue ); 
      break;
    case 6:
      pwmLed2.setPWM(3, 0, val_Red );
      pwmLed2.setPWM(4, 0, val_Green );
      pwmLed2.setPWM(5, 0, val_Blue ); 
      break;
    case 7:
      pwmLed2.setPWM(6, 0, val_Red );
      pwmLed2.setPWM(7, 0, val_Green );
      pwmLed2.setPWM(8, 0, val_Blue ); 
      break;
    case 8:
      pwmLed2.setPWM(9, 0, val_Red );
      pwmLed2.setPWM(10, 0, val_Green );
      pwmLed2.setPWM(11, 0, val_Blue ); 
      break;
    case 9:
      pwmLed3.setPWM(0, 0, val_Red );
      pwmLed3.setPWM(1, 0, val_Green );
      pwmLed3.setPWM(2, 0, val_Blue ); 
      break;
    case 10:
      pwmLed3.setPWM(3, 0, val_Red );
      pwmLed3.setPWM(4, 0, val_Green );
      pwmLed3.setPWM(5, 0, val_Blue ); 
      break;
    case 11:
      pwmLed3.setPWM(6, 0, val_Red );
      pwmLed3.setPWM(7, 0, val_Green );
      pwmLed3.setPWM(8, 0, val_Blue ); 
      break;
    case 12:
      pwmLed3.setPWM(9, 0, val_Red );
      pwmLed3.setPWM(10, 0, val_Green );
      pwmLed3.setPWM(11, 0, val_Blue ); 
      break;
    case 13:
      pwmLed4.setPWM(0, 0, val_Red );
      pwmLed4.setPWM(1, 0, val_Green );
      pwmLed4.setPWM(2, 0, val_Blue ); 
      break;
    case 14:
      pwmLed4.setPWM(3, 0, val_Red );
      pwmLed4.setPWM(4, 0, val_Green );
      pwmLed4.setPWM(5, 0, val_Blue ); 
      break;
    case 15:
      pwmLed4.setPWM(6, 0, val_Red );
      pwmLed4.setPWM(7, 0, val_Green );
      pwmLed4.setPWM(8, 0, val_Blue );
      break;
    case 16:
      pwmLed4.setPWM(9, 0, val_Red );
      pwmLed4.setPWM(10, 0, val_Green );
      pwmLed4.setPWM(11, 0, val_Blue );
      break;
    case 17:
      pwmLed5.setPWM(0, 0, val_Red );
      pwmLed5.setPWM(1, 0, val_Green );
      pwmLed5.setPWM(2, 0, val_Blue );
      break;
    case 18:
      pwmLed5.setPWM(3, 0, val_Red );
      pwmLed5.setPWM(4, 0, val_Green );
      pwmLed5.setPWM(5, 0, val_Blue );
      break;
    case 19:
      pwmLed5.setPWM(6, 0, val_Red );
      pwmLed5.setPWM(7, 0, val_Green );
      pwmLed5.setPWM(8, 0, val_Blue );
      break;
    case 20:
      pwmLed5.setPWM(9, 0, val_Red );
      pwmLed5.setPWM(10, 0, val_Green );
      pwmLed5.setPWM(11, 0, val_Blue );
      break;
    case 21:
      pwmLed6.setPWM(0, 0, val_Red );
      pwmLed6.setPWM(1, 0, val_Green );
      pwmLed6.setPWM(2, 0, val_Blue );
      break;
    case 22:
      pwmLed6.setPWM(3, 0, val_Red );
      pwmLed6.setPWM(4, 0, val_Green );
      pwmLed6.setPWM(5, 0, val_Blue );
      break;
    case 23:
      pwmLed6.setPWM(6, 0, val_Red );
      pwmLed6.setPWM(7, 0, val_Green );
      pwmLed6.setPWM(8, 0, val_Blue );
      break;
    case 24:
      pwmLed6.setPWM(9, 0, val_Red );
      pwmLed6.setPWM(10, 0, val_Green );
      pwmLed6.setPWM(11, 0, val_Blue );
      break;
    default:
      break; 
  }
}

/*
 * Hàm set Van ONOFF 
 * Func:        func_setVan
 * Parameters : int van, int onoff
 * Return :     none
*/
void func_setVan(int van, int onoff)
{
  static uint8_t val =0;
  switch(onoff)
  {
    case 0:
      val = 1;
      break;
    case 1:
      val = 0;
      break;
  }
  switch(van)
  {
    case 1:
      digitalWrite(IO_Van[1], val);
      break;
    case 2:
      digitalWrite(IO_Van[2], val);
      break;
    case 3:
      digitalWrite(IO_Van[3], val);
      break;
    case 4:
      digitalWrite(IO_Van[4], val);
      break;
    case 5:
      digitalWrite(IO_Van[5], val);
      break;
    case 6:
      digitalWrite(IO_Van[6], val);
      break;
    case 7:
      digitalWrite(IO_Van[7], val);
      break;
    case 8:
      digitalWrite(IO_Van[8], val);
      break;
    case 9:
      digitalWrite(IO_Van[9], val);
      break;
    case 10:
      digitalWrite(IO_Van[10], val);
      break;
    case 11:
      digitalWrite(IO_Van[11], val);
      break;
    case 12:
      digitalWrite(IO_Van[12], val);
      break;
    case 13:
      digitalWrite(IO_Van[13], val);
      break;
    case 14:
      digitalWrite(IO_Van[14], val);
      break;
    case 15:
      digitalWrite(IO_Van[15], val);
      break;
    case 16:
      digitalWrite(IO_Van[16], val);
      break;
    case 17:
      digitalWrite(IO_Van[17], val);
      break;
    case 18:
      digitalWrite(IO_Van[18], val);
      break;
    case 19:
      digitalWrite(IO_Van[19], val);
      break;
    case 20:
      digitalWrite(IO_Van[20], val);
      break;
    case 21:
      digitalWrite(IO_Van[21], val);
      break;
    case 22:
      digitalWrite(IO_Van[22], val);
      break;
    case 23:
      digitalWrite(IO_Van[23], val);
      break;
    case 24:
      digitalWrite(IO_Van[24], val);
      break;
    default:
      break; 
  }
}


/*
 * Hàm SET tất cả van ON hoặc OFF
 * Func:        func_setVanAll
 * Parameters : none
 * Return :     none
*/
void func_setVanAll(uint8_t onoff)
{
  for(uint8_t i=1; i<25; ++i)
  {
    digitalWrite(IO_Van[i], onoff);
  }
}

/*
 * Hàm SET tất cả led giống màu nhau, reset thì  set các 
 * value RGB tương ứng 0 0 0
 * Func:        func_setLedAll
 * Parameters : int R,int G, int B
 * Return :     none
*/
void func_setLedAll(int R,int G, int B)
{
  static uint16_t val_Red = 0, val_Green = 0,val_Blue = 0;
  val_Red = map(R,0,255,50,600);
  val_Green = map(G,0,255,50,600);
  val_Blue = map(B,0,255,50,600);
  for(uint8_t i = 0; i<=15;i=i+3)
  {
      pwmLed1.setPWM(i, 0, val_Red );
      pwmLed2.setPWM(i, 0, val_Red );
      pwmLed3.setPWM(i, 0, val_Red );
      pwmLed4.setPWM(i, 0, val_Red );
      pwmLed5.setPWM(i, 0, val_Red );
      pwmLed6.setPWM(i, 0, val_Red );
  }
  for(uint8_t i = 1; i<=15;i=i+3)
  {
      pwmLed1.setPWM(i, 0, val_Green );
      pwmLed2.setPWM(i, 0, val_Green );
      pwmLed3.setPWM(i, 0, val_Green );
      pwmLed4.setPWM(i, 0, val_Green );
      pwmLed5.setPWM(i, 0, val_Green );
      pwmLed6.setPWM(i, 0, val_Green );
  }
  for(uint8_t i = 2; i<=15;i=i+3)
  {
      pwmLed1.setPWM(i, 0, val_Blue );
      pwmLed2.setPWM(i, 0, val_Blue );
      pwmLed3.setPWM(i, 0, val_Blue );
      pwmLed4.setPWM(i, 0, val_Blue );
      pwmLed5.setPWM(i, 0, val_Blue );
      pwmLed6.setPWM(i, 0, val_Blue );
  }
}
// Khởi tạo biến cho hàm func_randLED
static uint16_t randLed1 = 0;
static uint16_t randLed2 = 0;
static uint16_t randLed3 = 0;
/*
 * Hàm random màu LED
 * Func:        func_randLED
 * Parameters : none
 * Return :     none
*/
void func_randLED()
{
      randLed1 =random(0,255);
      randLed2 =random(0,255);
      randLed3 =random(0,255);
      if(randLed1 < 50 && randLed2 < 50 && randLed3 < 50)
      {
        switch(uint8_t ran = random(1,3))
        {
          case 1:     
            randLed1 = random(150,255);
            break;
          case 2:     
            randLed2 = random(150,255);
            break;
          case 3:     
            randLed3 = random(150,255);
            break;
        }
      }
      if(randLed1 > 200 && randLed2 > 200 && randLed3 > 200)
      {
          switch(uint8_t ran = random(1,3))
          {
              case 1:     
                randLed1 = 0;
                break;
              case 2:     
                randLed2 = 0;
                break;
              case 3:     
                randLed3 = 0;
                break;
          }
      }
}



/*
 * Hàm hiệu ứng LED sáng dần 1, sáng dần LED từ 1 đến 12, 13 đến 24 
 * Func:        func_effectLED_sangdan1
 * Parameters : uint32_t animationDelay    : thời gian delay hiệu ứng(ms)
 *              uint32_t time_run          : thời gian chạy hiệu ứng (s)
 *              uint8_t animationDirection :hướng chạy hiệu ứng, trái qua phải hay ngược lại ( 0 : left to right)
 * Return :     none
*/
void func_effectLED_sangdan1(uint32_t time_run,uint16_t animationDelay, uint8_t animationDirection)
{
  // Biến đếm cho hàm millis
  static uint32_t count_time = 0;
  // Biến đếm phụ cho biến num
  static uint8_t count_num1 =0;
  static uint8_t count_num2 =12;
  static uint32_t count_time_run = 0;;
  static uint8_t count_num_run =0;
  if(millis()  - count_time_run > 1000)
  {
      count_time_run = millis();
      if(++count_num_run  >= time_run)
      {
        done_effectLED_sangdan1 = 1;
        count_num_run = 0;
      } 
  }  
  if(millis()  - count_time > animationDelay)
  {
    count_time = millis();
    switch(state)
    {
        case state1:
            func_randLED();
            switch(animationDirection)
            {
              case LEFTtoRIGHT: 
                count_num1 =0;
                count_num2 =12;
                break;
              case RIGHTtoLEFT: 
                count_num1 =13;
                count_num2 =25;
                break;
            }
            count_num1 =0;
            state = state2;
            break;
        case state2:
            switch(animationDirection)
            {
              case LEFTtoRIGHT: 
                count_num1++;
                count_num2++;
                if(count_num1 > 12)
                {
                  func_randLED();
                  state = state1;
                }
                break;
              case RIGHTtoLEFT: 
                count_num1--;
                count_num2--;
                if(count_num1 <= 0)
                {
                  func_randLED();
                  state = state1;
                }
                break;
            }
            func_setLed(count_num1,randLed1,randLed2,randLed3);
            break;
    }
  }
}
/*
 * Hàm hiệu ứng LED sáng dần 2
 * Func:        func_effectLED_sangdan2
 * Parameters : uint32_t animationDelay    : thời gian delay hiệu ứng(ms)
 *              uint32_t time_run          : thời gian chạy hiệu ứng (s)
 *              uint8_t animationDirection :hướng chạy hiệu ứng, trái qua phải hay ngược lại ( 0 : left to right)
 * Return :     none
*/
void func_effectLED_sangdan2(uint32_t time_run,uint16_t animationDelay, uint8_t animationDirection)
{
  // Biến đếm cho hàm millis
  static uint32_t count_time = 0;
  // Biến đếm phụ cho biến num
  static uint8_t count_num1 =0;
  static uint8_t count_num2 =12;
  static uint32_t count_time_run = 0;;
  static uint8_t count_num_run =0;
  if(millis()  - count_time_run > 1000)
  {
      count_time_run = millis();
      if(++count_num_run  >= time_run)
      {
        done_effectLED_sangdan2 = 1;
        count_num_run = 0;
      } 
  }  
  if(millis()  - count_time > animationDelay)
  {
    count_time = millis();
    switch(state)
    {
        case state1:
            func_randLED();
            switch(animationDirection)
            {
              case LEFTtoRIGHT: 
                count_num1 =0;
                count_num2 =12;
                break;
              case RIGHTtoLEFT: 
                count_num1 =13;
                count_num2 =25;
                break;
            }
            count_num1 =0;
            state = state2;
            break;
        case state2:
            switch(animationDirection)
            {
              case LEFTtoRIGHT: 
                count_num1++;
                count_num2++;
                if(count_num1 > 12)
                {
                  func_randLED();
                  count_num1 =0;
                  count_num2 =12;
                  state = state3;
                }
                break;
              case RIGHTtoLEFT: 
                count_num1--;
                count_num2--;
                if(count_num1 <= 0)
                {
                  func_randLED();
                  count_num1 =13;
                  count_num2 =25;
                  state = state3;
                }
                break;
            }
            func_setLed(count_num1,randLed1,randLed2,randLed3);
            func_setLed(count_num2,randLed1,randLed2,randLed3);
            break;
        case state3:
             switch(animationDirection)
            {
              case LEFTtoRIGHT: 
                count_num1++;
                count_num2++;
                if(count_num1 > 12)
                {
                  func_randLED();
                  state = state1;
                }
                break;
              case RIGHTtoLEFT: 
                count_num1--;
                count_num2--;
                if(count_num1 <= 0)
                {
                  func_randLED();
                  state = state1;
                }
                break;
            }
            func_setLed(count_num1,0,0,0);
            func_setLed(count_num2,0,0,0);
            break;
    }
  }
}


/* 
 *  Hiệu ứng Rainbow
 *  https://forum.arduino.cc/t/color-changing-rgb-led-rainbow-spectrum/8561
 *  https://www.youtube.com/watch?v=nxTFyZYhetk
*/
int counter = 0;
int numColors = 255;
int animationDelay = 10; 

/*
 * Hàm hiệu ứng All LED rainbow
 * Func:        func_effectLED_rainbow
 * Parameters :uint32_t animationDelay  : thời gian delay hiệu ứng(ms)
 *             uint32_t time_run : thời gian chạy hiệu ứng (s)
 * Return :     none
*/
void func_effectLED_RGB(uint32_t time_run, uint32_t animationDelay)
{
    float colorNumber = counter > numColors ? counter - numColors: counter;
    float saturation = 1;
    float brightness = 1;
    float hue = (colorNumber / float(numColors)) * 360;
    long color = HSBtoRGB(hue, saturation, brightness); 
    int red = color >> 16 & 255;
    int green = color >> 8 & 255;
    int blue = color & 255;
    static uint32_t count_time = 0;
    static uint32_t count_time_run = 0;
  // Biến đếm phụ cho biến num
    static uint8_t count_num_run =0;
    if(millis()  - count_time > animationDelay)
    {
        count_time = millis();
        func_setLedAll(red, green, blue);
        counter = (counter + 1) % (numColors * 2);
    }
    if(millis()  - count_time_run > 1000)
    {
        count_time_run = millis();
        if(++count_num_run  >= time_run)
        {
          done_effectLED_RGB = 1;
          count_num_run = 0;
        } 
    }
    
}
long HSBtoRGB(float _hue, float _sat, float _brightness) {
   float red = 0.0;
   float green = 0.0;
   float blue = 0.0;  
   if (_sat == 0.0) {
       red = _brightness;
       green = _brightness;
       blue = _brightness;
   } else {
       if (_hue == 360.0) {
           _hue = 0;
       }
       int slice = _hue / 60.0;
       float hue_frac = (_hue / 60.0) - slice;
       float aa = _brightness * (1.0 - _sat);
       float bb = _brightness * (1.0 - _sat * hue_frac);
       float cc = _brightness * (1.0 - _sat * (1.0 - hue_frac));      
       switch(slice) {
           case 0:
               red = _brightness;
               green = cc;
               blue = aa;
               break;
           case 1:
               red = bb;
               green = _brightness;
               blue = aa;
               break;
           case 2:
               red = aa;
               green = _brightness;
               blue = cc;
               break;
           case 3:
               red = aa;
               green = bb;
               blue = _brightness;
               break;
           case 4:
               red = cc;
               green = aa;
               blue = _brightness;
               break;
           case 5:
               red = _brightness;
               green = aa;
               blue = bb;
               break;
           default:
               red = 0.0;
               green = 0.0;
               blue = 0.0;
               break;
       }
   }
   long ired = red * 255.0;
   long igreen = green * 255.0;
   long iblue = blue * 255.0;
   return long((ired << 16) | (igreen << 8) | (iblue));
}


/*
 * Hàm hiệu ứng LED rainbow
 * https://www.youtube.com/watch?v=-3VKkTSAytM&t=21s
 * Func:        func_effectLED_rainbow
 * Parameters :uint32_t animationDelay  : thời gian delay hiệu ứng(ms)
 *             uint32_t time_run : thời gian chạy hiệu ứng (s)
 *              uint8_t animationDirection :hướng chạy hiệu ứng, trái qua phải hay ngược lại ( 0 : left to right)
 * Return :     uint8_t
*/
uint8_t   effStep;
uint8_t func_effectLED_rainbow(uint32_t time_run,uint16_t animationDelay, uint8_t animationDirection) {
    // Strip ID: 0 - Effect: Rainbow - LEDS: 24
    // Steps: 60 - Delay: 20
    // Colors: 3 (255.0.0, 0.255.0, 0.0.255)
    // Options: rainbowlen=60, toLeft=true, 
  float factor1, factor2;
  uint16_t ind;
  static uint32_t count_time = 0;
  static uint32_t count_time_run = 0;;
  static uint8_t count_num_run =0;
  if(millis()  - count_time_run > 1000)
  {
      count_time_run = millis();
      if(++count_num_run  >= time_run)
      {
        done_effectLED_rainbow = 1;
        count_num_run = 0;
      } 
  } 
  if(millis()  - count_time > animationDelay)
  {
        count_time = millis();
        for(uint16_t j=0;j<24;j++) {
          switch(ind)
          {
            case 0:
              ind = effStep + j * 1;
              break;
            case 1:
              ind = 60 - (uint16_t)(effStep - j * 1) % 60;
              break;
          }
          switch((int)((ind % 60) / 20)) {
            case 0: factor1 = 1.0 - ((float)(ind % 60 - 0 * 20) / 20);
                    factor2 = (float)((int)(ind - 0) % 60) / 20;
                    func_setLed(j, 255 * factor1 + 0 * factor2, 0 * factor1 + 255 * factor2, 0 * factor1 + 0 * factor2);
                    break;
            case 1: factor1 = 1.0 - ((float)(ind % 60 - 1 * 20) / 20);
                    factor2 = (float)((int)(ind - 20) % 60) / 20;
                    func_setLed(j, 0 * factor1 + 0 * factor2, 255 * factor1 + 0 * factor2, 0 * factor1 + 255 * factor2);
                    break;
            case 2: factor1 = 1.0 - ((float)(ind % 60 - 2 * 20) / 20);
                    factor2 = (float)((int)(ind - 40) % 60) / 20;
                    func_setLed(j, 0 * factor1 + 255 * factor2, 0 * factor1 + 0 * factor2, 255 * factor1 + 0 * factor2);
                    break;
          }
        }
        if(effStep >= 60) {effStep = 0 ; return 0x03; }
        else effStep++;
        return 0x01;
  }
}

/*
 * Hàm hiệu ứng LED fade
 * https://www.youtube.com/watch?v=-3VKkTSAytM&t=21s
 * https://adrianotiger.github.io/Neopixel-Effect-Generator/
 * Func:        func_effectLED_rainbow
 * Parameters :uint32_t animationDelay  : thời gian delay hiệu ứng(ms)
 *             uint32_t time_run : thời gian chạy hiệu ứng (s)
 * Return :     uint8_t
*/

uint8_t func_effectLED_fade(uint32_t time_run,uint16_t animationDelay) 
{
    // Strip ID: 0 - Effect: Fade - LEDS: 24
    // Steps: 950 - Delay: time_run
    // Colors: 2 (235.76.0, 0.255.255)
    // Options: duration=673, every=1, 
    uint8_t r,g,b;
    double e;
    static uint32_t count_time = 0;
    static uint32_t count_time_run = 0;;
    static uint8_t count_num_run =0;
    if(millis()  - count_time_run > 1000)
    {
        count_time_run = millis();
        if(++count_num_run  >= time_run)
        {
          done_effectLED_rainbow = 1;
          count_num_run = 0;
        } 
    }
      
    if(millis()  - count_time > animationDelay)
    {
          count_time = millis();
          e = (effStep * 5) / (double)673;
          r = ( e ) * randLed1 + 235 * ( 1.0 - e );
          g = ( e ) * randLed2 + 76 * ( 1.0 - e );
          b = ( e ) * randLed3 + 0 * ( 1.0 - e );
          for(uint16_t j=0;j<24;j++) {
            if((j % 1) == 0)
              func_setLed(j, r, g, b);
            else
              func_setLed(j, 0, 0, 0);
          }
          if(effStep >= 950) 
          {
            effStep = 0; 
            func_randLED();
            return 0x03; 
          }
          else effStep++;
          return 0x01;
    }
}
