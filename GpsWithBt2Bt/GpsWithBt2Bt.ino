#include <SoftwareSerial.h>
#include <TinyGPS.h>

#define BAND_NUMBER 0
#define BtSerial_Rx 5
#define BtSerial_Tx 6
#define GpsSerial_Rx 9
#define GpsSerial_Tx 10
#define PRECISION_OF_GPS 9
#define LAT_OFFSET 3
#define LON_OFFSET 4
#define DELAY_OFFSET 10

SoftwareSerial BtSerial(BtSerial_Rx, BtSerial_Tx);
SoftwareSerial GpsSerial(GpsSerial_Rx, GpsSerial_Tx);
TinyGPS myGps;
void BtBroadcasting(float, float, char);

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
      /* Serial.print("Chars: ");
      Serial.print(chars);
      Serial.print(", Sentences: ");
      Serial.print(sentences);
      Serial.print(", Failed_checkedsum: ");
      Serial.println(failedCheckedSum); */
      Serial.print("LAT= ");
      String sLat = String(fLat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : fLat, PRECISION_OF_GPS);
      Serial.println(sLat);
      Serial.print(", LON= ");
      String sLon = String(fLon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 :fLon, PRECISION_OF_GPS);
      Serial.println(sLon);
      BtBroadcasting(fLat, fLon, 'G');
      Serial.println("----------------------------------------");
    }
  }
  else if(chars == 0) {
    Serial.println("**No characters received from GPS: check wiring **");
  }
}

void BtBroadcasting(float lat, float lon, char bandStatus) {
  String msgAVDA = "AT+AVDA=";
  msgAVDA += BAND_NUMBER;
  msgAVDA += bandStatus;
  String sLat = String(lat, PRECISION_OF_GPS);
  sLat = sLat.substring(LAT_OFFSET);
  String sLon = String(lon, PRECISION_OF_GPS);
  sLon = sLon.substring(LON_OFFSET);
  String messageA = msgAVDA + 'A' + sLat;
  Serial.println(messageA);
  //BtSerial.write(messageA);
  delay(DELAY_OFFSET);
  String messageB = msgAVDA + 'B' + sLon;
  Serial.println(messageB);
  //BtSerial.write(messageB);
  delay(DELAY_OFFSET);
}

