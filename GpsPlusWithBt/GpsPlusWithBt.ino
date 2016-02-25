#include <SoftwareSerial.h>
#include <TinyGPS++.h>

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
TinyGPSPlus gps;
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
  static const double LONDON_LAT = 51.508131, LONDON_LON = -0.128002;

  String sLat = printFloat(gps.location.lat(), gps.location.isValid(), 13, 8);
  String sLon = printFloat(gps.location.lng(), gps.location.isValid(), 14, 9);
  //printDateTime(gps.date, gps.time);
  //printFloat(gps.altitude.meters(), gps.altitude.isValid(), 7, 2);
  //printStr(gps.course.isValid() ? TinyGPSPlus::cardinal(gps.course.value()) : "*** ", 6);
  
  Serial.println();
  Serial.println(sLat);
  Serial.println(sLon);
  
  smartDelay(1000);
  
  if (millis() > 5000 && gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring"));

}

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (GpsSerial.available())
      gps.encode(GpsSerial.read());
  } while (millis() - start < ms);
}

String printFloat(float val, bool valid, int len, int prec)
{
  if (!valid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  }
  else
  {
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i)
      Serial.print(' ');
  }
  smartDelay(0);
  return String(val, prec);
}

static void printInt(unsigned long val, bool valid, int len)
{
  char sz[32] = "*****************";
  if (valid)
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i=strlen(sz); i<len; ++i)
    sz[i] = ' ';
  if (len > 0) 
    sz[len-1] = ' ';
  Serial.print(sz);
  smartDelay(0);
}

static void printDateTime(TinyGPSDate &d, TinyGPSTime &t)
{
  if (!d.isValid())
  {
    Serial.print(F("********** "));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d/%02d/%02d ", d.month(), d.day(), d.year());
    Serial.print(sz);
  }
  
  if (!t.isValid())
  {
    Serial.print(F("******** "));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d:%02d:%02d ", t.hour(), t.minute(), t.second());
    Serial.print(sz);
  }

  printInt(d.age(), d.isValid(), 5);
  smartDelay(0);
}

static void printStr(const char *str, int len)
{
  int slen = strlen(str);
  for (int i=0; i<len; ++i)
    Serial.print(i<slen ? str[i] : ' ');
  smartDelay(0);
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

