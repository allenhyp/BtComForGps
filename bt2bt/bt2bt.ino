#include <SoftwareSerial.h>

SoftwareSerial BtSerial(10,11);

void setup() { 
  // put your setup code here, to run once:
  Serial.begin(9600);
  while(!Serial){
    ;
  }
  Serial.println("Finnished Setup Serial");
  BtSerial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(BtSerial.available()) {
    //Serial.print("RESPOND: ");
    Serial.write(BtSerial.read());
    //Serial.println();
  }
  if(Serial.available()) {
    BtSerial.write(Serial.read());
  }
}
