#include <SoftwareSerial.h>
#include <TinyGPS.h>

#define BtSerial_Rx 10
#define BtSerial_Tx 11
#define GpsSerial_Rx 8
#define GpsSerial_Tx 9

SoftwareSerial BtSerial(BtSerial_Rx, BtSerial_Tx);
SoftwareSerial GpsSerial(GpsSerial_Rx, GpsSerial_Tx);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while(!Serial){;}
  BtSerial.begin(9600);
  GpsSerial.begin(9600);
  Serial.println("Finnished Setting");
}

void loop() {
  // put your main code here, to run repeatedly:

}
