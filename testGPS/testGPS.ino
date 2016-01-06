#include <TinyGPS.h>
#include <SoftwareSerial.h>

TinyGPS myGPS;
SoftwareSerial gSerial(10,11);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  gSerial.begin(9600);
  Serial.println("Here begins the track the gps signal.");
  digitalWrite(11,HIGH);
  while(!gSerial.available()) {
    Serial.println("connecting...");
    if(gSerial.read() == '\r')
      break;
  }
  Serial.println("Start!");
}

void loop() {
  // put your main code here, to run repeatedly:
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;
  for(unsigned long starter = millis(); millis()-starter <1000;) {
    while(gSerial.available()) {
      char c = gSerial.read();
      // Serial.write(c);
    
      if(myGPS.encode(c))
      {
        newData = true;
      }
    }
  }
  if(newData) {
    float flat, flon;
    unsigned long age;
    
    myGPS.f_get_position(&flat, &flon, &age);
    if ( age == TinyGPS::GPS_INVALID_AGE) {
      Serial.println("No fix detected.");
    }
    else if ( age > 5000) {
      Serial.println("Warning: possible stale data!");
    }
    else {
      myGPS.stats (&chars, &sentences, &failed);
      Serial.print("Chars: ");
      Serial.println(chars);
      Serial.print("Sentences: ");
      Serial.println(sentences);
      Serial.print("Failed_checksum:");
      Serial.println(failed);
      Serial.println("-------------------------------------------");
      Serial.print("LAT=");
      Serial.println( flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
      Serial.print("LON=");
      Serial.println( flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    }
  }
  else {
    Serial.println("**No characters received from GPS: check wiring **");
  }
  // Serial.println("***************************************************");
}
