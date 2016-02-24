#include <SoftwareSerial.h>

SoftwareSerial BtSerial(5,6);
String message;
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
  while(BtSerial.available()) {
    message+=char(BtSerial.read());
    //Serial.println();
    delay(1);
  }
  
  if(!Serial.available() && message!="") {
    Serial.print("RESPOND: ");
    Serial.print(message);
    Serial.println();
    message="";
  }
  if(Serial.available()) {
    BtSerial.write(Serial.read());
  }
}
