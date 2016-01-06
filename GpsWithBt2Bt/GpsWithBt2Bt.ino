#include <SoftwareSerial.h>
#include <TinyGPS.h>

#define BtSerial_Rx 10
#define BtSerial_Tx 11
#define GpsSerial_Rx 8
#define GpsSerial_Tx 9

SoftwareSerial BtSerial(BtSerial_Rx, BtSerial_Tx);
SoftwareSerial GpsSerial(GpsSerial_Rx, GpsSerial_Tx);
TinyGPS myGps;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while(!Serial){;}
  BtSerial.begin(9600);
  GpsSerial.begin(9600);
  while (!GpsSerial.available()) {
    if (GpsSerial.read() == '\r')
      break;
  }
  Serial.println("Finnished Setting");
}

void loop() {
  // put your main code here, to run repeatedly:
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failedCheckedSum;
  for(unsigned long starter = millis(); millis()-starter < 1000;) {
    while(GpsSerial.available()) {
      char gpsRawData = GpsSerial.read();
      // Serail.write(gpsRawData);
      if(myGps.encode(gpsRawData)) {
        newData = true;
      }
    }
  }

  if(newData) {
    float fLat, fLon;
    unsigned long age;

    myGps.f_get_position(&fLat, &fLon, &age);
    if( age == TinyGPS::GPS_INVALID_AGE) {
      Serial.println("No fix detected.");
    }
    else if (age > 5000) {
      Serial.println("Warning:possible stale data!");
    }
    else {
      myGps.stats(&chars, &sentences, &failedCheckedSum);
      Serial.print("Chars: ");
      Serial.print(chars);
      Serial.print(", Sentences: ");
      Serial.print(sentences);
      Serial.print(", Failed_checkedsum: ");
      Serial.println(failedCheckedSum);
      Serial.print("LAT= ");
      Serial.println(fLat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : fLat, 6);
      Serial.print(", LON= ");
      Serial.println(fLon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 :fLon, 6);
      Serial.println("----------------------------------------");

    }
  }
  else if(chars == 0) {
    Serial.println("**No characters received from GPS: check wiring **");
  }
}
