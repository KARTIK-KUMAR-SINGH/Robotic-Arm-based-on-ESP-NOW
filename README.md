# 🤖 ESP-NOW Smart Wireless Robotic Arm

Wireless 5-DOF robotic arm using ESP-NOW with real-time joystick control, pose saving (EEPROM), OLED UI, and bidirectional feedback system.

---

## 🚀 Smart Wireless Robotic Arm (ESP-NOW Based)

This project is a **5-DOF wireless robotic arm system** built using Arduino Mega and ESP8266 modules. It combines **real-time control, memory-based automation, and a user-friendly interface** to create a complete robotic control ecosystem.

The system allows users to **manually control the arm, save positions, and replay them sequentially**, similar to industrial robotic arms used in automation.

---

## 🔥 Key Features

### 🎮 Real-Time Wireless Control

* Control 5 servos using dual joysticks
* Smooth movement with dead-zone filtering
* ESP-NOW communication (low latency, no router required)

---

### 💾 Pose Memory System (EEPROM)

* Save multiple arm positions (poses)
* Stores all 5 servo angles per pose
* Supports up to 20 saved positions
* Data persists even after power OFF

---

### ▶ Automated Execution Mode

* Execute saved poses sequentially
* Step-by-step servo movement
* Suitable for repeatable tasks:

  * Pick and place
  * Sorting
  * Farming automation

---

### 📺 OLED User Interface

* Displays:

  * Servo angles
  * System mode (LIVE / SAVED)
  * Execution progress
* Visual progress bars
* Button-controlled navigation

---

### 🔁 Bidirectional Communication

* Servo angles sent back to controller
* Real-time feedback system
* Improves accuracy and monitoring

---

### 🔘 Multi-Button Control System

| Button  | Function                    |
| ------- | --------------------------- |
| RESET   | Moves all servos to 90°     |
| SAVE    | Hold 2s → Save pose         |
| DELETE  | Hold 10s → Clear all memory |
| TOGGLE  | Switch LIVE ↔ SAVED mode    |
| EXECUTE | Run selected saved pose     |

---

### 📡 ESP-NOW Communication

* Peer-to-peer wireless protocol
* No WiFi router required
* Fast and reliable data transfer

---

## ⚙️ System Architecture

### 🧠 Controller (Arduino Mega)

* Reads joystick input
* Handles buttons and UI
* Stores poses in EEPROM
* Sends data via Serial

---

### 📡 Transmitter (ESP8266)

* Converts serial data to packets
* Sends via ESP-NOW
* Receives feedback

---

### 🤖 Receiver (ESP8266 + Servos)

* Controls 5 servos
* Executes commands
* Sends feedback

---

## 🔌 Hardware Required

* Arduino Mega
* 2 × ESP8266 (NodeMCU / ESP-12)
* 5 × Servo Motors
* 2 × Joysticks
* OLED Display (SSD1306, I2C)
* Push Buttons
* External 5V Power Supply

---

## 🧠 Capabilities

* Wireless robotic control
* Memory-based automation
* Real-time feedback system
* Expandable for:

  * AI vision
  * Smart farming
  * Industrial automation

---

## 🔮 Future Improvements

* Smooth trajectory planning
* Mobile app control
* AI-based object detection
* Autonomous navigation
* Safety fail-safe system

---

## 📌 Applications

* Smart Farming 🌱
* Pick and Place Systems
* Robotics Projects
* Industrial Automation

---

## 🏁 Conclusion

This project demonstrates the integration of:

* Embedded Systems
* Wireless Communication
* Human-Machine Interface (HMI)
* Robotics Control

It serves as a strong foundation for advanced robotic systems.

---

## 👨‍💻 Author

**Kartik Kumar Singh**

---

## ⭐ Support

If you like this project, give it a ⭐ on GitHub!
