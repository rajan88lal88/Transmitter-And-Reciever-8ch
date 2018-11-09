#include<SPI.h>
#include<nRF24L01.h>
#include<RF24.h>

const uint64_t pipeOut = 1234;

RF24 radio(9, 10);//CSN pins


struct Data{
  int throttle;
  int yaw;
  int pitch;
  int roll;
  byte AUX1;
  byte AUX2;
  byte AUX3;
  byte AUX4;
}data;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(pipeOut);
  radio.startListening();
}

void loop() {
    while ( radio.available() ) {
    radio.read(&data, sizeof(Data));
    Serial.print("\n");
    Serial.print(data.throttle);
    Serial.print("\t");
    Serial.print(data.yaw);
    Serial.print("\t");
    Serial.print(data.pitch);
    Serial.print("\t");
    Serial.print(data.roll);
    Serial.print("\t");
    Serial.print(data.AUX1);
    Serial.print("\t");
    Serial.print(data.AUX2);
    Serial.print("\t");
    Serial.print(data.AUX3);
    Serial.print("\t");
    Serial.print(data.AUX4);
    }
}
