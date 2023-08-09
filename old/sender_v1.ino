#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#define RXD1 26
#define TXD1 27

static const int RXPin = 16, TXPin = 17;
static const uint32_t GPSBaud = 9600;

// The TinyGPSPlus object
float lat, lng;
char buffer1[20];
char buffer2[20];

TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600,SERIAL_8N1,RXD1,TXD1);
  ss.begin(GPSBaud);
}

void loop() {
  // static const double LONDON_LAT = 51.508131, LONDON_LON = -0.128002;
  lat = gps.location.lat();
  lng = gps.location.lng();

  // printInt(gps.satellites.value(), gps.satellites.isValid(), 5);
  // printFloat(gps.hdop.hdop(), gps.hdop.isValid(), 6, 1);
  // Serial.println(lat);
  // Serial.println(lng);
  dtostrf(lat, 8, 6, buffer1);
  dtostrf(lng, 8, 6, buffer2);
  // Serial.println(buffer1);
  // Serial.println(buffer2);

  // Serial.println();


  smartDelay(1000);

  if (millis() > 5000 && gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring"));

  static uint32_t dataTime = 0;
  if (millis() - dataTime >= 100) {
  String data = "A" + String (buffer1) + "B" + String (buffer2) + "C" + String(150) + "D" + String(200) 
  + "E" + String(250) + "F";
  data += String (data.length()) + "G";
  Serial.println(data); //Kirim data sensor
  Serial1.println(data);
    dataTime = millis();
  }
}


static void smartDelay(unsigned long ms) {
  unsigned long start = millis();
  do {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}
