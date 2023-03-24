#include <arduino-timer.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <stdint.h>
#include <avr/wdt.h>

/*
 * Output bao gom
 * + 24 I/O
 * + 4 I/O kích biến tần bơm chìm tạo sóng nối chung 
 * + 4 I/O kích biến tần bơm đài to nối chung
 * + 4 I/O kích biến tần bơm đài nhỏ nối chung
 */ 
// Khởi tạo biến cho 24 I/O
uint8_t IO_Van[25] = {0,30,31,32,33,34,35,36,37,38,39,40,
                      41,42,43,44,45,46,47,48,49,50,51,52,53};

// Khởi tạo biến cho 4 I/O bơm chìm tạo sóng
uint8_t IO_BienTan[17] = {0,18,12,11,10,9,8,7,6,5,4,3,2,
                          14,15,16,17};
                          

// Khởi tạo biến cho 4 I/O bơm chìm tạo sóng
uint8_t IO_BomTaoSong[5] = {0,17 ,16,15,14};
// Khởi tạo biến cho 4 I/O kích biến tần bơm đài to
uint8_t IO_BomDaiTo[5] = {0, 2,3,4,5};
// Khởi tạo biến cho 4 I/O kích biến tần bơm đài nhỏ
uint8_t IO_BomDaiNho[5] = {0,6,7,8,9};

//Khai bao may thoi khí
uint8_t IO_MayThoiKhi1 = 10;
uint8_t IO_MayThoiKhi2 = 11;

#define VAN_ON  0
#define VAN_OFF 1
#define MTK_ON  0
#define MTK_OFF 1

#define THUAN  1
#define NGHICH 0
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
  state9,
  state10,
  state11
}STATE;


uint32_t t_time1 =0;

#define BIENTAN_ON  0
#define BIENTAN_OFF 1

enum BienTan
{
  BT_BomTaoSong,
  BT_BomDaiNho,
  BT_BomDaiTo
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
  }
    for(uint8_t i = 1; i< 25; i++)
  {
    digitalWrite(IO_Van[i], VAN_ON);
  }
   pinMode(IO_MayThoiKhi1,OUTPUT);
    pinMode(IO_MayThoiKhi2,OUTPUT);
    digitalWrite(IO_MayThoiKhi1, MTK_OFF);
    digitalWrite(IO_MayThoiKhi2, MTK_OFF);
   for(uint8_t i = 1; i< 5; i++)
  {
    pinMode(IO_BomTaoSong[i], OUTPUT);
    pinMode(IO_BomDaiTo[i], OUTPUT);
    pinMode(IO_BomDaiNho[i], OUTPUT);
  }
  for(uint8_t i = 1; i< 5; i++)
  {
    digitalWrite(IO_BomTaoSong[i], HIGH);
    digitalWrite(IO_BomDaiTo[i], HIGH);
    digitalWrite(IO_BomDaiNho[i], HIGH);
  }
  pwmLed1.begin(); pwmLed2.begin(); pwmLed3.begin();pwmLed4.begin();pwmLed5.begin();pwmLed6.begin();
  pwmLed1.setOscillatorFrequency(27000000);pwmLed1.setPWMFreq(1000);
  pwmLed2.setOscillatorFrequency(27000000);pwmLed2.setPWMFreq(1000);
  pwmLed3.setOscillatorFrequency(27000000);pwmLed3.setPWMFreq(1000);
  pwmLed4.setOscillatorFrequency(27000000);pwmLed4.setPWMFreq(1000);
  pwmLed5.setOscillatorFrequency(27000000);pwmLed5.setPWMFreq(1000);
  pwmLed6.setOscillatorFrequency(27000000);pwmLed6.setPWMFreq(1000);
  Wire.setClock(400000);
  wdt_enable(WDTO_2S);
  func_offBienTan(BT_BomTaoSong);
  func_offBienTan(BT_BomDaiTo);
  func_offBienTan(BT_BomDaiNho);
  func_BomDaiNho(4);
  func_MayThoiKhi(MTK_ON, MTK_ON);
  func_setBienTan(BT_BomDaiTo,7);
  //func_setVanAll(VAN_ON);
}
uint8_t alowOpenBT = 1;
void loop() {
  // put your main code here, to run repeatedly:
  func_BomTaoSong(3000);
  func_LED_RAND(1000);
  func_DKVan(20); //protectBTDaiTo();
  
  //func_DKLED(10);
  wdt_reset();
}


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
        digitalWrite(IO_BomTaoSong[i], BIENTAN_OFF);
      }
      break;
    case BT_BomDaiNho:
      for(uint8_t i = 1; i< 5; i++)
      {
        digitalWrite(IO_BomDaiNho[i], BIENTAN_OFF);
      }
      break;
    case  BT_BomDaiTo:
      for(uint8_t i = 1; i< 5; i++)
      {
        digitalWrite(IO_BomDaiTo[i], BIENTAN_OFF);
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
          digitalWrite(IO_BomTaoSong[1], BIENTAN_ON);
          digitalWrite(IO_BomTaoSong[2], BIENTAN_ON);
          break;
        case BT_BomDaiTo:
          digitalWrite(IO_BomDaiTo[1], BIENTAN_ON);
          digitalWrite(IO_BomDaiTo[2], BIENTAN_ON);
          break;
        case BT_BomDaiNho:
          digitalWrite(IO_BomDaiNho[1], BIENTAN_ON);
          digitalWrite(IO_BomDaiNho[2], BIENTAN_ON);
          break;
      }
      break;
    case 2:
      switch(BT)
      {
        case BT_BomTaoSong:
          digitalWrite(IO_BomTaoSong[1], BIENTAN_ON);
          digitalWrite(IO_BomTaoSong[3], BIENTAN_ON);
          break;
        case BT_BomDaiTo:
          digitalWrite(IO_BomDaiTo[1], BIENTAN_ON);
          digitalWrite(IO_BomDaiTo[3], BIENTAN_ON);
          break;
        case BT_BomDaiNho:
          digitalWrite(IO_BomDaiNho[1], BIENTAN_ON);
          digitalWrite(IO_BomDaiNho[3], BIENTAN_ON);
          break;
      } 
      break;
    case 3:
      switch(BT)
      {
        case BT_BomTaoSong:
          digitalWrite(IO_BomTaoSong[1], BIENTAN_ON);
          digitalWrite(IO_BomTaoSong[4], BIENTAN_ON);
          break;
        case BT_BomDaiTo:
          digitalWrite(IO_BomDaiTo[1], BIENTAN_ON);
          digitalWrite(IO_BomDaiTo[4], BIENTAN_ON);
          break;
        case BT_BomDaiNho:
          digitalWrite(IO_BomDaiNho[1], BIENTAN_ON);
          digitalWrite(IO_BomDaiNho[4], BIENTAN_ON);
          break;
      }
      break;
    case 4:
      switch(BT)
      {
        case BT_BomTaoSong:
          digitalWrite(IO_BomTaoSong[1], BIENTAN_ON);
          digitalWrite(IO_BomTaoSong[2], BIENTAN_ON);
          digitalWrite(IO_BomTaoSong[3], BIENTAN_ON);
          break;
        case BT_BomDaiTo:
          digitalWrite(IO_BomDaiTo[1], BIENTAN_ON);
          digitalWrite(IO_BomDaiTo[2], BIENTAN_ON);
          digitalWrite(IO_BomDaiTo[3], BIENTAN_ON);
          break;
        case BT_BomDaiNho:
          digitalWrite(IO_BomDaiNho[1], BIENTAN_ON);
          digitalWrite(IO_BomDaiNho[2], BIENTAN_ON);
          digitalWrite(IO_BomDaiNho[3], BIENTAN_ON);
          break;
      } 
      break;
    case 5:
      switch(BT)
      {
        case BT_BomTaoSong:
          digitalWrite(IO_BomTaoSong[1], BIENTAN_ON);
          digitalWrite(IO_BomTaoSong[2], BIENTAN_ON);
          digitalWrite(IO_BomTaoSong[4], BIENTAN_ON);
          break;
        case BT_BomDaiTo:
          digitalWrite(IO_BomDaiTo[1], BIENTAN_ON);
          digitalWrite(IO_BomDaiTo[2], BIENTAN_ON);
          digitalWrite(IO_BomDaiTo[4], BIENTAN_ON);
          break;
        case BT_BomDaiNho:
          digitalWrite(IO_BomDaiNho[1], BIENTAN_ON);
          digitalWrite(IO_BomDaiNho[2], BIENTAN_ON);
          digitalWrite(IO_BomDaiNho[4], BIENTAN_ON);
          break;
      } 
      break;
    case 6:
      switch(BT)
      {
        case BT_BomTaoSong:
          digitalWrite(IO_BomTaoSong[1], BIENTAN_ON);
          digitalWrite(IO_BomTaoSong[3], BIENTAN_ON);
          digitalWrite(IO_BomTaoSong[4], BIENTAN_ON);
          break;
        case BT_BomDaiTo:
          digitalWrite(IO_BomDaiTo[1], BIENTAN_ON);
          digitalWrite(IO_BomDaiTo[3], BIENTAN_ON);
          digitalWrite(IO_BomDaiTo[4], BIENTAN_ON);
          break;
        case BT_BomDaiNho:
          digitalWrite(IO_BomDaiNho[1], BIENTAN_ON);
          digitalWrite(IO_BomDaiNho[3], BIENTAN_ON);
          digitalWrite(IO_BomDaiNho[4], BIENTAN_ON);
          break;
      }
      break;
    case 7:
      switch(BT)
      {
        case BT_BomTaoSong:
          digitalWrite(IO_BomTaoSong[1], BIENTAN_ON);
          digitalWrite(IO_BomTaoSong[2], BIENTAN_ON);
          digitalWrite(IO_BomTaoSong[3], BIENTAN_ON);
          digitalWrite(IO_BomTaoSong[4], BIENTAN_ON);
          break;
        case BT_BomDaiTo:
          digitalWrite(IO_BomDaiTo[1], BIENTAN_ON);
          digitalWrite(IO_BomDaiTo[2], BIENTAN_ON);
          digitalWrite(IO_BomDaiTo[3], BIENTAN_ON);
          digitalWrite(IO_BomDaiTo[4], BIENTAN_ON);
          break;
        case BT_BomDaiNho:
          digitalWrite(IO_BomDaiNho[1], BIENTAN_ON);
          digitalWrite(IO_BomDaiNho[2], BIENTAN_ON);
          digitalWrite(IO_BomDaiNho[3], BIENTAN_ON);
          digitalWrite(IO_BomDaiNho[4], BIENTAN_ON);
          break;
      }
      break;
  }
}

uint32_t time_BomTaoSong=0;
void func_BomTaoSong(int countMillis)
{
  func_setBienTan(BT_BomTaoSong,4);
  /*
  static uint8_t stateBTS=0;
  if(millis() - time_BomTaoSong > countMillis)
  {
     time_BomTaoSong = millis();
     stateBTS = 1 - stateBTS;
  }
  if(stateBTS == 0)
    
    func_setBienTan(BT_BomTaoSong,7);
  else
    func_setBienTan(BT_BomTaoSong,2);
  */
}
void func_MayThoiKhi(uint8_t stateMTK1, uint8_t stateMTK2)
{
   digitalWrite(IO_MayThoiKhi1,stateMTK1);
   digitalWrite(IO_MayThoiKhi2,stateMTK2);
}
uint32_t timePTR = 0;
void protectBTDaiTo()
{
  static uint8_t statePRT = state1;
  static uint8_t countVanON;
  switch(statePRT)
  {
     case state1:
        for(uint8_t i = 1; i<=24;i++)
        {
          if(digitalRead(IO_Van[i]) == VAN_ON)
             countVanON++;
        }
        if(countVanON==0)
        {
            func_offBienTan(BT_BomDaiTo);
            alowOpenBT =0;
        }
        else 
            countVanON =0;
        timePTR = millis();
         
        statePRT = state2;
        break;
     case state2:
        if(millis() - timePTR >3000)
        {
          timePTR = millis();
          alowOpenBT =1;
        }
        break;
  }
  
  
}


STATE state = state1;
int stateLED = state1;
uint32_t t_timeLED = 0;
void func_DKLED(int countMillis)
{
  if(millis() - t_timeLED > countMillis*1000)
  {
    t_timeLED = millis();
    stateLED++;
    if(stateLED > state6) stateLED = state1;
  }
  switch(stateLED)
  {
    case state1: 
        func_LED_RAND(3000);
       // func_EffLED1(250,THUAN);
        break;
    case state2: 
        func_LED_RAND2(2000);
       // func_EffLED1(250,THUAN);
        break;
    case state3: 
        func_LED_RAND2(1000);
       // func_EffLED1(250,THUAN);
        break;
    case state4: 
        func_LED_RAND2(500);
       // func_EffLED1(250,THUAN);
        break;
    case state5: 
        func_LED_RAND2(200);
       // func_EffLED1(250,THUAN);
        break;
     case state6: 
        func_effectLED_RGB(100);
       // func_EffLED1(250,THUAN);
        break;
        
  }
}


///////////////////////
static int k1=0,k2=0,k3=0;
static uint32_t count=0;
void func_effectLED_RGB( uint32_t animationDelay)
{  
  if(millis()  - count >animationDelay)
  {
        count = millis();
        switch( state)
        {
          case state1:
            k1=0;k2=250;k3=0;
            state = state3;
            break;
          case state2:
            
            k1--;k2=k2+2;
            if(k1==20) k1 =20;
            if(k2 >= 255) 
              state = state3;
            
            break;
          case state3:

            count = millis();
            k2--;k3=k3+2;
    
            if(k2==0) k2 =0;
            if(k3 >= 255)
                state = state4;
            break;
          case state4:
        //    255, 255, 125
            k3--;k2++;k1++;           
            if(k3<=125) k3 =125;
            if(k1 == 255) k1 = 255;
            if(k1 == 255)
                state = state5;
            
            break;
          case state5:
         //   0, 255, 255
            k1--;k3++;
            if(k1<=0) k1 =0;
            if(k3 == 255)
                state = state6;
            break;
          case state6:
          //255, 0, 255
            k1++;k2--;
            if(k1==255) k1 =255;
            if(k2 == 0)
                state = state7;
            break;
          case state7:
          //255, 255, 0
            count = millis();
            k2++;k3--;
            if(k2==255) k1 =255;
            if(k3 == 0)
                state = state8;
            break;
          case state8:
            count = millis();
            k1--;
            if(k1 == 20)
            state = state1;
             
            break;
        }
    func_setLedAll(k1,k2,k3);
  }
}



///////////////////////

uint8_t stateVan=state1;
void func_DKVan(int countMillis)
{
  if(millis() - t_time1 > countMillis*1000)
  {
    t_time1 = millis();
    stateVan++;
    if(stateVan > state11) stateVan = state1;
  }
  switch(stateVan)
  {
    case state1:
      if(alowOpenBT == 1)
        func_setBienTan(BT_BomDaiTo,7);
      for(uint8_t i=1; i <=24 ;i++)
      {
          if(i%2 == 0)
            func_setVan(i, VAN_ON);
          else
            func_setVan(i, VAN_OFF);  
      }
      break;
    case state2:
      if(alowOpenBT == 1)
         func_setBienTan(BT_BomDaiTo,7);
      for(uint8_t i=1; i <=24 ;i++)
      {
          if(i%2 == 1)
            func_setVan(i, VAN_ON);
          else
            func_setVan(i, VAN_OFF);  
      }
      break;
    case state3:
      if(alowOpenBT == 1)
        func_setBienTan(BT_BomDaiTo,7);
      func_setVanAll(VAN_ON);
      break;
    case state4:
      func_VanEff_2COL(500,THUAN);
      if(alowOpenBT == 1)
        func_setBienTan(BT_BomDaiTo,7);
      break;
    case state5:
      func_VanEff_2COL(500,NGHICH);
      if(alowOpenBT == 1)
        func_setBienTan(BT_BomDaiTo,7);
      break;
    case state6:
      func_VanEff_3COL(500,THUAN);
      if(alowOpenBT == 1)
        func_setBienTan(BT_BomDaiTo,7);
      break;
    case state7:
      func_VanEff_3COL(500,NGHICH);
      if(alowOpenBT == 1)
        func_setBienTan(BT_BomDaiTo,7);
      break;
    case state8:
      func_VanEff_4COL(500,THUAN);
      if(alowOpenBT == 1)
        func_setBienTan(BT_BomDaiTo,7);
      break;
    case state9:
      func_VanEff_4COL(500,NGHICH);
      if(alowOpenBT == 1)
        func_setBienTan(BT_BomDaiTo,7);
      break;
    case state10:
      func_VanEff_SD(500,THUAN);
      if(alowOpenBT == 1)
          func_setBienTan(BT_BomDaiTo,7);
      break;
    case state11:
      func_VanEff_SD(500,NGHICH);
      if(alowOpenBT == 1)
        func_setBienTan(BT_BomDaiTo,7);
      break;
  }
}


// 1234    9101112        17181920
//     5678       13141516        21222324
/*
 * Hàm set Van ONOFF 
 * Func:        func_setVan
 * Parameters : int van, int onoff
 * Return :     none
*/
void func_setVan(int van, int onoff)
{
  switch(van)
  {
    case 1:
      digitalWrite(IO_Van[1], onoff);
      break;
    case 2:
      digitalWrite(IO_Van[2], onoff);
      break;
    case 3:
      digitalWrite(IO_Van[3], onoff);
      break;
    case 4:
      digitalWrite(IO_Van[4], onoff);
      break;
    case 5:
      digitalWrite(IO_Van[5], onoff);
      break;
    case 6:
      digitalWrite(IO_Van[6], onoff);
      break;
    case 7:
      digitalWrite(IO_Van[7], onoff);
      break;
    case 8:
      digitalWrite(IO_Van[8], onoff);
      break;
    case 9:
      digitalWrite(IO_Van[9], onoff);
      break;
    case 10:
      digitalWrite(IO_Van[10], onoff);
      break;
    case 11:
      digitalWrite(IO_Van[11], onoff);
      break;
    case 12:
      digitalWrite(IO_Van[12], onoff);
      break;
    case 13:
      digitalWrite(IO_Van[13], onoff);
      break;
    case 14:
      digitalWrite(IO_Van[14], onoff);
      break;
    case 15:
      digitalWrite(IO_Van[15], onoff);
      break;
    case 16:
      digitalWrite(IO_Van[16], onoff);
      break;
    case 17:
      digitalWrite(IO_Van[17], onoff);
      break;
    case 18:
      digitalWrite(IO_Van[18], onoff);
      break;
    case 19:
      digitalWrite(IO_Van[19], onoff);
      break;
    case 20:
      digitalWrite(IO_Van[20], onoff);
      break;
    case 21:
      digitalWrite(IO_Van[21], onoff);
      break;
    case 22:
      digitalWrite(IO_Van[22], onoff);
      break;
    case 23:
      digitalWrite(IO_Van[23], onoff);
      break;
    case 24:
      digitalWrite(IO_Van[24], onoff);
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

///12345678
// 1314151617181920
// 9  - 16
// 21 - 4

uint32_t t_time2;

uint8_t buffEff_2COL[12][24] =
{
   {0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1},
   {1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1},
   {1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1},
   {1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1},
   {1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0},
   {0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0},
   {0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0},
   {0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0},
   {0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0},
   {0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0},
   {0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0,0},
   {0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,0}
};

void func_VanEff_2COL(uint32_t countMillis, uint8_t thuannghich)
{
    static int valueVan = 0;
    if(millis() - t_time2 > countMillis)
    {
        t_time2 = millis();
        if(thuannghich == THUAN)
        {
          valueVan++;
          if(valueVan >11) valueVan = 0;
        }
        else
        {
          valueVan--;
          if(valueVan <0 ) valueVan = 11;
        }
        for(int i = 1 ; i<=24; i++)
        {
          func_setVan(i,buffEff_2COL[valueVan][i-1]);
        }
    }
}


uint8_t buffEff_3COL[9][24] =
{
   {0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1},
   {1,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1},
   {1,1,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1},
   {1,1,1,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0},
   {0,1,1,1,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0},
   {0,0,1,1,1,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0},
   {0,0,0,1,1,1,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0},
   {0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1},
   {0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1},
};

void func_VanEff_3COL(uint32_t countMillis, uint8_t thuannghich)
{
    static int valueVan = 0;
    if(millis() - t_time2 > countMillis)
    {
        t_time2 = millis();
        if(thuannghich == THUAN)
        {
          valueVan++;
          if(valueVan >8) valueVan = 0;
        }
        else
        {
          valueVan--;
          if(valueVan <0 ) valueVan = 8;
        }
        for(int i = 1 ; i<=24; i++)
        {
          func_setVan(i,buffEff_3COL[valueVan][i-1]);
        }
    }
}


uint8_t buffEff_4COL[6][24] =
{
   {0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1},
   {1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1},
   {1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1},
   {1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0},
   {0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0},
   {0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,0},
};

void func_VanEff_4COL(uint32_t countMillis, uint8_t thuannghich)
{
    static int valueVan = 0;
    if(millis() - t_time2 > countMillis)
    {
        t_time2 = millis();
        if(thuannghich == THUAN)
        {
          valueVan++;
          if(valueVan >5) valueVan = 0;
        }
        else
        {
          valueVan--;
          if(valueVan <0 ) valueVan = 5;
        }
        for(int i = 1 ; i<=24; i++)
        {
          func_setVan(i,buffEff_4COL[valueVan][i-1]);
        }
    }
}




int buffEff_SD[9][24] =
{
   {0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0},
   {0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0},
   {0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0},
   {0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0},
   {0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0},
   {0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0},
   {0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0},
   {0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

void func_VanEff_SD(uint32_t countMillis, uint8_t thuannghich)
{
    static int valueVan = 0;
    if(millis() - t_time2 > countMillis)
    {
        t_time2 = millis();
        if(thuannghich == THUAN)
        {
          valueVan++;
          if(valueVan >8) valueVan = 0;
        }
        else
        {
          valueVan--;
          if(valueVan <0 ) valueVan = 8;
        }
        for(int i = 1 ; i<=24; i++)
        {
          func_setVan(i,buffEff_SD[valueVan][i-1]);
        }
    }
}

int buffLEDCoLor[6][3]=
{
  {255,0,0},
  {0,255,0},
  {0,0,255},
  {255,255,0},
  {255,0,255},
  {0,255,255}
};
uint32_t t_time3;
void func_LED_RAND(uint32_t countMillis)
{
  static uint8_t valueLed = 0;
    if(millis() - t_time3 > countMillis)
    {
        t_time3 = millis();
        valueLed++;
        if(valueLed >6) valueLed = 0;
        func_setLedAll(buffLEDCoLor[valueLed][0],buffLEDCoLor[valueLed][1],buffLEDCoLor[valueLed][2]);
    }
}
static uint16_t randLed1 = 0;
static uint16_t randLed2 = 0;
static uint16_t randLed3 = 0;
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

void func_LED_RAND2(uint32_t countMillis)
{
  static uint8_t valueLed = 0;
    if(millis() - t_time3 > countMillis)
    {
        t_time3 = millis();
        valueLed++;
        if(valueLed >6) valueLed = 0;
        func_setLedAll(randLed1,randLed2,randLed3);
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

uint32_t t_timeEffLed=0;

  
//  func_setLed

uint8_t buffEffLED1[13][12][3] ={
    { {255,0,0},{255,0,0},{255,0,0},{220,0,0},{180,0,0},{150,0,0},{120,0,0},{90,0,0},{60,0,0},{30,0,0},{0,0,0},{0,0,0}    },
    { {0,0,0}  ,{255,0,0},{255,0,0},{255,0,0},{220,0,0},{180,0,0},{150,0,0},{120,0,0},{90,0,0},{60,0,0},{30,0,0},{0,0,0}  },
    { {0,0,0}  ,{0,0,0},  {255,0,0},{255,0,0},{255,0,0},{220,0,0},{180,0,0},{150,0,0},{120,0,0},{90,0,0},{60,0,0},{30,0,0}},
    { {30,0,0} ,{0,0,0}  ,{0,0,0},  {255,0,0},{255,0,0},{255,0,0},{220,0,0},{180,0,0},{150,0,0},{120,0,0},{90,0,0},{60,0,0}},
    { {60,0,0} ,{30,0,0} ,{0,0,0}  ,{0,0,0},  {255,0,0},{255,0,0},{255,0,0},{220,0,0},{180,0,0},{150,0,0},{120,0,0},{90,0,0}},
    { {90,0,0} ,{60,0,0} ,{30,0,0} ,{0,0,0}  ,{0,0,0},  {255,0,0},{255,0,0},{255,0,0},{220,0,0},{180,0,0},{150,0,0},{120,0,0}},
    { {120,0,0},{90,0,0} ,{60,0,0} ,{30,0,0} ,{0,0,0}  ,{0,0,0},  {255,0,0},{255,0,0},{255,0,0},{220,0,0},{180,0,0},{150,0,0}},
    { {150,0,0},{120,0,0},{90,0,0} ,{60,0,0} ,{30,0,0} ,{0,0,0}  ,{0,0,0},  {255,0,0},{255,0,0},{255,0,0},{220,0,0},{180,0,0}},
    { {180,0,0},{150,0,0},{120,0,0},{90,0,0} ,{60,0,0} ,{30,0,0} ,{0,0,0}  ,{0,0,0},  {255,0,0},{255,0,0},{255,0,0},{220,0,0}},
    { {220,0,0},{180,0,0},{150,0,0},{120,0,0},{90,0,0} ,{60,0,0} ,{30,0,0} ,{0,0,0}  ,{0,0,0},  {255,0,0},{255,0,0},{255,0,0}},
    { {255,0,0},{220,0,0},{180,0,0},{150,0,0},{120,0,0},{90,0,0} ,{60,0,0} ,{30,0,0} ,{0,0,0}  ,{0,0,0},  {255,0,0},{255,0,0}},
    { {255,0,0},{255,0,0},{220,0,0},{180,0,0},{150,0,0},{120,0,0},{90,0,0} ,{60,0,0} ,{30,0,0} ,{0,0,0}  ,{0,0,0},  {255,0,0}},
    { {255,0,0},{255,0,0},{255,0,0},{220,0,0},{180,0,0},{150,0,0},{120,0,0},{90,0,0} ,{60,0,0} ,{30,0,0} ,{0,0,0}  ,{0,0,0}}
};
void func_EffLED1(uint32_t countMillis, uint8_t thuannghich)
{
    static int valueLED = 0;
    if(millis() - t_timeEffLed > countMillis)
    {
        t_timeEffLed = millis();
        if(thuannghich == THUAN)
        {
            valueLED++;
            if(valueLED >12) valueLED = 0;
        }
        else
        {
            valueLED--;
            if(valueLED <0 ) valueLED = 12;
        }
        for(int i = 1 ; i<=12; i++)
        {
            func_setLed(i,buffEffLED1[valueLED][i][0],buffEffLED1[valueLED][i][1],buffEffLED1[valueLED][i][2]);
        }
    }
}
