
# 🚗 Smart Parking System using ESP32 + Ultrasonic Sensors

A low-cost, Wi-Fi enabled smart parking system that uses ultrasonic sensors to detect car presence and recommends the nearest available parking spot. Built with ESP32, I2C LCD display, and a simple web server interface.


---

## 📦 Features

- 🧠 Recommends nearest available parking spot
- 📏 Uses two ultrasonic sensors for vehicle detection
- 🌐 Displays status on an I2C LCD and Web Dashboard
- 🔁 Real-time updates every second
- ⚠️ Displays error messages for sensor malfunction
- 💡 Built with affordable components, suitable for small lots or demo setups

---

## 🧰 Hardware Components

| Component               | Description                           |
|------------------------|---------------------------------------|
| ESP32-WROOM-32         | Main microcontroller with Wi-Fi       |
| Ultrasonic Sensor (x2) | Distance measurement for each slot    |
| I2C LCD (16x2)         | Displays slot availability and alerts |
| Resistors, wires, breadboard | For prototyping connections     |

---

## 🧑‍💻 Software & Libraries

- Arduino IDE with ESP32 board package
- Libraries Used:
  - WiFi.h
  - Wire.h
  - LiquidCrystal_PCF8574.h

---

## 📟 LCD Display Logic

- If both slots are empty → "2 Spots Free" → "Use Spot 1"
- If one is empty → displays available slot → "Park Here"
- If full → "Parking Full" → "Please Wait"
- If sensors fail → "Sensor Error" → "Check Wiring"

---

## 🌐 Web Dashboard

Accessible over the local Wi-Fi network:

- Auto-refreshing HTML page (every 5 seconds)
- Shows distance from each ultrasonic sensor
- Displays parking recommendation
- HTML hosted from ESP32's internal web server

---

## 📐 Circuit Diagram

📷 Add Fritzing diagram or circuit schematic if available.

---

## 🚀 Getting Started

1. Clone this repo and open in Arduino IDE
2. Install dependencies: LiquidCrystal_PCF8574, ESP32 board drivers
3. Set your Wi-Fi credentials in the code:
   ```
   const char* ssid = "Your_WiFi_Name";
   const char* password = "Your_WiFi_Password";
   ```
4. Upload the code to ESP32 (select correct board and COM port)
5. Open Serial Monitor (115200 baud) to see IP address
6. Access your web dashboard via that IP in browser

---

## 📊 Sample Output (Serial Monitor)

```
Distance 1: 8 cm, Distance 2: 70 cm
Recommend: Use Spot 2
```

---

## 🛠 Future Enhancements

- Add RFID or QR entry system
- Expand to more slots (4+)
- Integrate with Android app via HTTP API
- Add buzzer/light feedback for driver guidance

---

## 🏁 License

This project is open-source under the MIT License.  
Feel free to fork, use, or extend it for academic or personal use.
