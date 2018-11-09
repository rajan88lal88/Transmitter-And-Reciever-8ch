
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//74hc164 for displaying trottle on 8 LED indicator
#define srCLOCK 3
#define srDATA 5

const uint64_t pipeOut = 1234;//change it to any hexadecimal number and should match the code on reciever

RF24 radio(9, 10);
//32 byte Max
struct MyData {
  int throttle;
  int yaw;
  int pitch;
  int roll;
  byte AUX1;
  byte AUX2;
  byte AUX3;
  byte AUX4;
};

MyData data;//this will store all 8 values in one structure variable

void resetData() 
{
  data.throttle = 0;
  data.yaw = 127;
  data.pitch = 127;
  data.roll = 127;
  data.AUX1= 0;
  data.AUX2= 0;
  data.AUX3= 0;
  data.AUX4= 0;
}

void setup()
{
  pinMode(4, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  Serial.begin(19200);
  radio.openWritingPipe(pipeOut);
   
    
    //74hc164
  
  pinMode(srCLOCK, OUTPUT);
  pinMode(srDATA, OUTPUT);
  shiftOut(srDATA, srCLOCK, MSBFIRST, 1);
  
  resetData();
}

/**************************************************/


int mapJoystickValues(int val, int lower, int middle, int upper, bool reverse)
{
  val = constrain(val, lower, upper);
  if ( val < middle )
    val = map(val, lower, middle, 0, 128);
  else
    val = map(val, middle, upper, 128, 255);
  return ( reverse ? 255 - val : val );
}
int disp;
void loop()
{

  data.throttle = analogRead(A0);
  data.yaw      = analogRead(A1);
  data.pitch    = analogRead(A2);
  data.roll     = analogRead(A3);
  
 
  data.AUX1     = digitalRead(4);
  data.AUX2     = digitalRead(6);
  data.AUX3     = digitalRead(7);
  data.AUX4     = digitalRead(8);
  disp=data.throttle;

  radio.write(&data, sizeof(MyData));

  if(disp<1)
    disp=0;
  else if(disp<32)
    disp=1;
    else if(disp<64)
      disp=3;
      else if(disp<96)
        disp=7;
        else if(disp<128)
          disp=15;
          else if(disp<160)
            disp=31;
            else if(disp<192)
              disp=63;
              else if(disp<224)
                disp=127;
                else if(disp<256)
                  disp=255;
  shiftOut(srDATA, srCLOCK, MSBFIRST, disp);
}
