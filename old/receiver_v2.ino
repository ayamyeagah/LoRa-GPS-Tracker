#define BLYNK_TEMPLATE_ID "TMPL6FqKvSAhY"
#define BLYNK_TEMPLATE_NAME "Ship Tracker"
#define BLYNK_AUTH_TOKEN "vaSCXg7jSlRUnvCfA5KeawUBVRXvjIxs"

#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
 
#define SS      15   // GPIO15 -- SX1276's CS
#define RST     16   // GPIO16 -- SX1276's RESET
#define DI0     4   // GPIO4 -- SX1276's IRQ(Interrupt Request)

char auth[] = BLYNK_AUTH_TOKEN;     // Blynk Authentication Token
char ssid[] = "WarkopLegend5G";              // WiFi SSID
char pass[] = "mahameru";              // WiFi Password
 
void setup() {
  Serial.begin(115200); // Start the Serial communication to send messages to the computer
  Blynk.begin(auth, ssid, pass);  // Start Blynk
  delay(1000); // wait for a second
 
  // Start LoRa using the frequency appropriate for your region (eg. 868E6 for Europe, 915E6 for North America)
  LoRa.setPins(SS,RST,DI0);
  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa Receiver");
}
 
void loop() {
  Blynk.run(); // Initiates Blynk
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.println("Received packet:");
    
    String incoming = "";
    while (LoRa.available()) {
      incoming += (char)LoRa.read();
    }
 
    // Parse out the latitude, longitude and speed
    int latIndex = incoming.indexOf("LAT:");
    int lonIndex = incoming.indexOf(",LONG:");
    int speedIndex = incoming.indexOf(",SPEED:");
 
    float lat = incoming.substring(latIndex + 4, lonIndex).toFloat();
    float lon = incoming.substring(lonIndex + 6, speedIndex).toFloat();
    float speed = incoming.substring(speedIndex + 7).toFloat();
 
    // Reformat the strings with specified number of digits after decimal
    String latString = String(lat, 6);
    String lonString = String(lon, 6);
    String speedString = String(speed, 2) + " kmph"; // Assuming speed is in meters per second
    
    Serial.print("Latitude: ");
    Serial.println(latString);
    Serial.print("Longitude: ");
    Serial.println(lonString);
    Serial.print("Speed: ");
    Serial.println(speedString);
    
    // print RSSI of packet
    Serial.print("RSSI: ");
    Serial.println(LoRa.packetRssi());
 
    //Send to Blynk
    Blynk.virtualWrite(V1, latString.c_str());
    Blynk.virtualWrite(V2, lonString.c_str());
    Blynk.virtualWrite(V3, speedString.c_str());
 
    //Send to Google Map Widget
    Blynk.virtualWrite(V0, (lonString+","+latString).c_str()); 
  }
}