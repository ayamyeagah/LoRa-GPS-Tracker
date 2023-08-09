#define BLYNK_TEMPLATE_ID "TMPL6FqKvSAhY"
#define BLYNK_TEMPLATE_NAME "Ship Tracker"
#define BLYNK_AUTH_TOKEN "vaSCXg7jSlRUnvCfA5KeawUBVRXvjIxs"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;     // Blynk Authentication Token
char ssid[] = "WarkopLegend5G";              // WiFi SSID
char pass[] = "mahameru";              // WiFi Password

HardwareSerial SerialAT(2);
//Variabel array untuk data parsing
String arrData[3];

void setup() {
  Serial.begin(115200);
  SerialAT.begin(9600, SERIAL_8N1, 16, 17);
  Blynk.begin(auth, ssid, pass);  // Start Blynk
  // delay(1000); // wait for a second
}

void loop() {
  Blynk.run();
  static uint32_t dataTime = 0;
  if (millis() - dataTime >= 1000) {
    String data = "";
    while (SerialAT.available() > 0) {
      data += char(SerialAT.read());
      Serial.print(data);
    }
    //buang spasi datanya
    data.trim();

    //uji data
    if (data != "") {
      //format data "10#29.45#89.50" = array (setelah parsing)
      //parsing data
      int index = 0;
      for (int i = 0; i <= data.length(); i++) {
        char delimiter = '#';
        if (data[i] != delimiter)
          arrData[index] += data[i];
        else
          index++;  //variabel index bertambah 1
      }

      //pastikan bahwa data yang dikirim lengkap
      //urutannya 0,1,2
      if (index == 2) {
        //tampilan nilai sensor ke serial monitor
        Serial.println(arrData[0]);  //nilai lat
        Serial.println(arrData[1]);  //nilai lang
        Serial.println(arrData[2]);  //nilai water

        String lat_toBlynk = String(arrData[0], 6);
        String lng_toBlynk = String(arrData[1], 6);
        String pp_toBlynk = String(arrData[2], 2);

        //Send to Blynk
        Blynk.virtualWrite(V1, lat_toBlynk.c_str());
        Blynk.virtualWrite(V2, lng_toBlynk.c_str());
        Blynk.virtualWrite(V3, pp_toBlynk.c_str());
    
        //Send to Google Map Widget
        Blynk.virtualWrite(V0, (lng_toBlynk+","+lat_toBlynk).c_str()); 
      }
      arrData[0] = "";
      arrData[1] = "";
      arrData[2] = "";
    }
    dataTime = millis();
  }
}