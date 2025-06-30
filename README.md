# Smartbag
 The system is built around the ESP32 microcontroller, incorporating modules such as RFID for  item tracking, a load cell with HX711 amplifier for real-time weight monitoring, and a GPS module  for lost and found tracking.
 
# BagIQ - The Smart Bag

**BagIQ** is an IoT-based smart backpack system powered by the ESP32 that helps you stay organized, track your belongings, and locate your bag in case it's lost. It combines RFID-based item tracking, GPS-based location monitoring, and real-time weight sensing for smarter everyday usage.

---

##  Features

###  1. Smart Reminder System
- Uses **RC522 RFID** reader to detect tagged items (e.g., books).
- Scans items while packing.
- If an item is scanned twice (indicating it was removed after packing), the system sends a **reminder alert via Blynk** and **email** notification.

###  2. Lost-and-Found Locator
- Uses **NEO-6M GPS Module** to get real-time coordinates.
- Sends the current **GPS location to the user via Blynk**.
- Ideal for locating your bag if misplaced or stolen.

###  3. Weight Tracking System
- Incorporates a **Load Cell + HX711 Amplifier**.
- Monitors the weight of the bag in real-time.
- Alerts user if weight crosses a certain threshold or drops below expected (useful for checking if something is missing).

---

##  Components Used

| Component              | Description                         |
|------------------------|-------------------------------------|
| ESP-WROOM-32           | Main microcontroller (ESP32)        |
| RC522 RFID Reader      | For scanning tagged items           |
| RFID Tags              | Attached to items like books        |
| NEO-6M GPS Module      | For location tracking               |
| Load Cell (5kg/10kg)   | For weight sensing                  |
| HX711 Module           | Load cell amplifier                 |
| Blynk IoT Platform     | For remote notifications & GPS data|
| Google Apps Script     | (Optional) For email alerts         |

---

##  How It Works

1. **RFID Scanning**: Every item placed in the bag is scanned. If an item is scanned twice, it's assumed to be removed and a reminder is sent.
2. **GPS Tracking**: Continuously fetches and uploads the bag’s current location using GPS and displays it in the Blynk app.
3. **Weight Monitoring**: Measures and displays weight in real-time. Can detect if weight changes significantly (missing item or overload).

---

##  Setup Instructions

1. Connect the module:
   - RFID to ESP32 via SPI
   - GPS via UART (TX-RX)
   - Load Cell via HX711 (connected to ESP32 GPIOs)

2. Flash the ESP32 with the provided Arduino code using the Arduino IDE.

3. Set up:
   - Blynk account and project.
   
   
4. Update credentials and API keys in the code:
   ```cpp
   char auth[] = "YourBlynkAuthToken";
   char ssid[] = "YourWiFiSSID";
   char pass[] = "YourWiFiPassword";
