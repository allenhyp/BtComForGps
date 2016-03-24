#include <SoftwareSerial.h>
#include <TinyGPS.h>

#define BAND_NUMBER 0
#define BtSerial_Rx 5
#define BtSerial_Tx 6
#define GpsSerial_Rx 9
#define GpsSerial_Tx 10
#define SwitchInput 8
#define PRECISION_OF_GPS 9
#define LAT_OFFSET 3
#define LON_OFFSET 4
#define DELAY_OFFSET 2500
#define MESSAGE_STRING_LENGTH 21
#define DELAY_GPS 5000

// Functions..
void BtBroadcasting(float, float, char);
bool GpsGetData();
void ShowLocationOnSerial(float, float);

// Variables..
SoftwareSerial BtSerial(BtSerial_Rx, BtSerial_Tx);
SoftwareSerial GpsSerial(GpsSerial_Rx, GpsSerial_Tx);
TinyGPS myGps;
unsigned long prevMillis = 0;
unsigned long curMillis = 0;
const unsigned long gpsInterval = 5000; // milliseconds

void setup() {
  Serial.begin(115200);
  while(!Serial){;}
  BtSerial.begin(9600);
  GpsSerial.begin(9600);
  while (!GpsSerial.available()) {
    if (GpsSerial.read() == '\r')
      break;
  }
  Serial.println("Finnished Setting");
  pinMode(SwitchInput, INPUT);
}

void loop() {
  float fLat, fLon;
  prevMillis = millis();
  curMillis = prevMillis;
  int count = 0;
  float aLat = 0.0;
  float aLon = 0.0;
  unsigned long age;

  if (GpsGetData() && digitalRead(SwitchInput) == LOW)
  {
    Serial.println("hi");
    while (curMillis - prevMillis < 5000)
    {
      if (GpsGetData())
      {
        
        myGps.f_get_position(&fLat, &fLon, &age);
        if (age == TinyGPS::GPS_INVALID_AGE) 
        {
          Serial.println("No fix detected.");
        }
        else if (age > 5000) 
        {
          Serial.println("Warning:possible stale data!");
        }
        else 
        {
          Serial.print("valid data: ");
          Serial.print(fLat, 6);
          Serial.println(fLon, 6);
          aLat += fLat;
          aLon += fLon;
          count ++;
        }
      }
      else {continue;}
      curMillis = millis();
    }
    if (count > 0)
    {
      aLat = aLat / count;
      aLon = aLon / count;
      BtBroadcasting(aLat, aLon, 'G');
      aLat = 0.0;
      aLon = 0.0;
      count = 0;
    }
    else
    {
      aLat = 25.0;
      aLon = 121.0;
      BtBroadcasting(aLat, aLon, 'N');
    }
  }
  else if( digitalRead(SwitchInput) == HIGH && false)
  {
    Serial.println("SOS!!!");
    while(true)
    {
      Serial.println("oh no~");
      while(curMillis - prevMillis < 5000)  // consider not to reduce the cycle time
      {
        if(GpsGetData())
        {
          myGps.f_get_position(&fLat, &fLon, &age);
          aLat += fLat;
          aLon += fLon;
          count ++;
        }
      }
      if (count > 0)
      {
        aLat = aLat / count;
        aLon = aLon / count;
        BtBroadcasting(aLat, aLon, 'B');
      }
      curMillis = millis();
      if(digitalRead(SwitchInput) == LOW)
        break;
    }
  }
  else
  {
    Serial.println("**No characters received from GPS: check wiring **");
  }
}

void BtBroadcasting(float flat, float flon, char bandStatus) {
  String msgAVDA = "AT+AVDA=";
  msgAVDA += BAND_NUMBER;
  msgAVDA += bandStatus;
  String sLat = String(flat, PRECISION_OF_GPS);
  sLat = sLat.substring(LAT_OFFSET);
  String sLon = String(flon, PRECISION_OF_GPS);
  sLon = sLon.substring(LON_OFFSET);
  String messageA = msgAVDA + 'A' + sLat;
  SetAVDA(messageA);
  delay(DELAY_OFFSET);
  String messageB = msgAVDA + 'B' + sLon;
  SetAVDA(messageB);
  //BtSerial.write(messageB);
  delay(DELAY_OFFSET);
}

void SetAVDA(String input) {
  char message[MESSAGE_STRING_LENGTH];
  String msgReceived = "";
  for(int i = 0; i < MESSAGE_STRING_LENGTH; i++) {
    message[i] = input[i];
  }
  message[MESSAGE_STRING_LENGTH-1]='\0';
  Serial.println(message);
  BtSerial.write(message);
  /*while(!BtSerial.available()) {
    Serial.println("Waiting for bluetooth module to respond...");
  }
  while(BtSerial.available()) {
    msgReceived+=char(BtSerial.read());
    //Serial.println();
    delay(1);
  }
  
  if(!Serial.available() && msgReceived!="") {
    Serial.print("RESPOND: ");
    Serial.println(msgReceived);
  }*/
}

bool GpsGetData()
{
  bool newData = false;
  for(unsigned long starter = millis(); millis()-starter < 1000;) {
    while(GpsSerial.available()) {
      char gpsRawData = GpsSerial.read();
      // Serail.write(gpsRawData);
      if(myGps.encode(gpsRawData)) {
        newData = true;
      }
    }
  }
  /*if(newData)
  {
    Serial.println("received data.");
  }
  else
  {
    Serial.println("received no data.");
  }*/
  return newData;
}

void ShowLocationOnSerial(float fLat, float fLon)
{
  unsigned long chars;
  unsigned short sentences, failedCheckedSum;myGps.stats(&chars, &sentences, &failedCheckedSum);
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
  Serial.println("----------------------------------------");
}

