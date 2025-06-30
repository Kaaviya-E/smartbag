// ==== BLYNK TEMPLATE CONFIG ====
#define BLYNK_TEMPLATE_ID "TMPL3o9doPDaE"
#define BLYNK_TEMPLATE_NAME "BagIQ"
#define BLYNK_AUTH_TOKEN "2iF80zO8o3ib4DRPQOIGfE1GnFg_XK_W"

// ==== LIBRARIES ====
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <HX711.h>
#include <SPI.h>
#include <MFRC522.h>
#include <TinyGPSPlus.h>
#include <HardwareSerial.h>
#include <map>

// ==== WIFI CONFIG ====
char ssid[] = "Kaaviya";
char pass[] = "chanakya";

// ==== BLYNK VIRTUAL PINS ====
#define VPIN_RFID       V0  // Shows missing book
#define VPIN_WEIGHT     V1  // Shows current weight
#define VPIN_LATITUDE   V2
#define VPIN_LONGITUDE  V3
#define VPIN_MAPLINK    V5

// ==== HX711 LOAD CELL ====
#define DOUT  4
#define CLK   5
HX711 scale;

// ==== RFID RC522 ====
#define RST_PIN  22
#define SS_PIN   21
MFRC522 rfid(SS_PIN, RST_PIN);

// ==== GPS SETUP ====
HardwareSerial gpsSerial(1);
#define GPS_RX 16
#define GPS_TX 17
TinyGPSPlus gps;

// ==== RFID BOOKS CONFIG ====
std::map<String, String> bookMap = {
  {"CEDFF804", "Physics"},
  {"7F5EC7C4", "Pouch"},
  {"537071E3", "Chemistry"},
  {"03DF13DA", "Math"}
};
std::map<String, bool> bookInBag;

// ==== SETUP ====
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Booting Smart Bag...");

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  SPI.begin();
  rfid.PCD_Init();
  scale.begin(DOUT, CLK);
  scale.set_scale(-5716.5); // Adjust based on calibration
  scale.tare();
  gpsSerial.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);

  for (auto const& pair : bookMap) {
    bookInBag[pair.first] = true;
  }

  Serial.println("Initialized all modules.");
}

// ==== MAIN LOOP ====
void loop() {
  Blynk.run();

  // --- Load Cell (Weight Tracking) ---
  float weight = scale.get_units();
  Serial.print("Weight: ");
  Serial.println(weight);

  Blynk.virtualWrite(VPIN_WEIGHT, weight); // Show weight in gauge

  if (weight > 5.0) {
    Blynk.logEvent("overweight_bag", "Alert: Bag is overweight!");
    Serial.println("Overweight alert sent!");
  }

  // --- RFID (Smart Reminder) ---
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    String tag = "";
    for (byte i = 0; i < rfid.uid.size; i++) {
      tag += String(rfid.uid.uidByte[i] < 0x10 ? "0" : "");
      tag += String(rfid.uid.uidByte[i], HEX);
    }
    tag.toUpperCase();

    if (bookMap.count(tag) > 0) {
      String bookName = bookMap[tag];
      bookInBag[tag] = !bookInBag[tag];

      if (!bookInBag[tag]) {
        String msg = "Reminder: " + bookName + " is NOT in the bag!";
        Serial.println(msg);
        Blynk.virtualWrite(VPIN_RFID, msg);
        Blynk.logEvent("book_missing", msg);
      } else {
        String msg = "Book added: " + bookName;
        Serial.println(msg);
        Blynk.virtualWrite(VPIN_RFID, msg);
      }
    } else {
      Serial.println("Unknown RFID tag.");
    }

    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }

  // --- GPS (Location Updates) ---
  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }

  if (gps.location.isUpdated() && gps.location.isValid()) {
    float lat = gps.location.lat();
    float lng = gps.location.lng();
    Serial.print("GPS Location: ");
    Serial.print(lat, 6);
    Serial.print(", ");
    Serial.println(lng, 6);

    // Send latitude & longitude
    Blynk.virtualWrite(VPIN_LATITUDE, lat);
    Blynk.virtualWrite(VPIN_LONGITUDE, lng);

    // Send Google Maps link to V5
    String mapsLink = "https://maps.google.com/?q=" + String(lat, 6) + "," + String(lng, 6);
    Blynk.virtualWrite(VPIN_MAPLINK, mapsLink);
  } else {
    Serial.println("Waiting for valid GPS location...");
  }

  delay(1000); // Avoid flooding
}
