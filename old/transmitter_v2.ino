#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <LoRa.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
 
// Define GPS serial communication pins
#define rxGPS 3
#define txGPS 2
 
// Define LoRa parameters
#define ss 10    // Pin for the CS (chip select)
#define rst 9    // Pin for the RESET
#define dio0 7   // Pin for the interrupt. Can be any digital pin.
 
// Create gps and serial instance
TinyGPSPlus gps;    // This is the GPS object that will manage the GPS data
SoftwareSerial gpsSerial(rxGPS, txGPS); // This will be the serial connection to the GPS
 
volatile int f_wdt = 1; // This flag is used to keep track of the sleep status
 
// Watchdog timer interrupt service routine
ISR(WDT_vect) {
  if (f_wdt == 0) {
    f_wdt = 1;  // If f_wdt was 0, set it to 1
  } else {
    WDTCSR |= _BV(WDIE);  // Enable the WD interrupt (note no reset)
  }
}
 
void setup() {
  Serial.begin(9600); // Begin serial communication with a baud rate of 9600
  gpsSerial.begin(9600); // Begin GPS Serial with a baud rate of 9600
  Serial.println("GPS initialized...");
 
  // Initialize LoRa with specific pins and frequency
  LoRa.setPins(ss, rst, dio0);
  if (!LoRa.begin(868E6)) { // If LoRa does not start successfully, print an error message
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa initialized...");
 
  // Set sleep mode to power down mode for energy efficiency
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
}
 
void loop() {
  while (gpsSerial.available()) { // Check if there is communication from GPS
    if (gps.encode(gpsSerial.read())) { // If GPS data can be encoded...
      if (gps.location.isValid()) { // ...and if the GPS data is valid
 
        // Store GPS data in strings
        String lat = String(gps.location.lat(), 6);
        String lon = String(gps.location.lng(), 6);
        String speed = String(gps.speed.mps());
 
        // Print the acquired GPS data to the Serial Monitor
        Serial.println("GPS data acquired...");
        Serial.println("LAT: " + lat);
        Serial.println("LONG: " + lon);
        Serial.println("SPEED: " + speed);
 
        // Combine GPS data into one string to send
        String dataToSend = "LAT:" + lat + ",LONG:" + lon + ",SPEED:" + speed;
 
        // Begin LoRa transmission
        LoRa.beginPacket();
        LoRa.print(dataToSend); // Send the GPS data string
        int result = LoRa.endPacket(); // Finish LoRa transmission
 
        // Check if the LoRa transmission was successful and print result to Serial Monitor
        if (result) {
          Serial.println("Packet transmission successful");
        } else {
          Serial.println("Packet transmission failed");
        }
 
        delay(2000); // Wait 2 seconds
 
        // Go to sleep mode to save energy
        Serial.println("Going to sleep now");
        Serial.println();
        delay(100);
 
        // Sleep for 12 seconds (8 seconds + 4 seconds)
        f_wdt = 0;
        setup_watchdog(9); // Set watchdog timer for approx 8 seconds
        while (f_wdt == 0) {
          system_sleep();
        }
 
        f_wdt = 0;
        setup_watchdog(8); // Set watchdog timer for approx 4 seconds
        while (f_wdt == 0) {
          system_sleep();
        }
 
        // Disable watchdog timer
        wdt_disable();
 
        delay(2000); // Wait 2 seconds
      } else {
        Serial.println("Waiting for valid GPS data..."); // If the GPS data is invalid, print an error message
      }
    }
  }
}
 
// Function to put the system to sleep to save energy
void system_sleep() {
  ADCSRA &= ~(1 << ADEN); // Switch Analog to Digitalconverter OFF
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Set sleep mode to power down mode
  sleep_enable(); // Enable sleep mode
  sleep_mode();  // System actually sleeps here
  sleep_disable(); // Disable sleep mode
  ADCSRA |= (1 << ADEN); // Switch Analog to Digitalconverter ON
}
 
// Function to set up watchdog timer
void setup_watchdog(int i) {
  byte bb;
  int ww;
  if (i > 9 ) i = 9; // Ensure that the interval does not exceed the maximum
  bb = i & 7;
  if (i > 7) bb |= (1 << 5); // Set the correct bit for longer time intervals
  bb |= (1 << WDCE); // Enable changes
  ww = bb;
 
  MCUSR &= ~(1 << WDRF); // Reset the reset flag
  WDTCSR |= (1 << WDCE) | (1 << WDE); // Start timed sequence
  WDTCSR = bb; // Set new watchdog timeout value
  WDTCSR |= _BV(WDIE); // Enable watchdog interrupt
}