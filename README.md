# Robotic-Arm-based-on-ESP-NOW
Wireless 5-DOF robotic arm using ESP-NOW with real-time joystick control, pose saving (EEPROM), OLED UI, and bidirectional feedback system.

# 🤖 Smart Wireless Robotic Arm (ESP-NOW Based)

This project is a 5-DOF wireless robotic arm system built using Arduino Mega and ESP8266 modules. It combines real-time control, memory-based automation, and a user-friendly interface to create a complete robotic control ecosystem.

The system allows users to manually control the arm, save positions, and replay them sequentially, similar to industrial robotic arms used in automation.

# 🚀 Key Features
🎮 Real-Time Wireless Control
Control 5 servos using dual joysticks
Smooth movement with dead-zone filtering
Wireless communication using ESP-NOW (low latency, no router required)
💾 Pose Memory System (EEPROM)
Save multiple arm positions (poses)
Each pose stores all 5 servo angles
Supports up to 20 saved positions
Data persists even after power OFF
▶ Automated Execution Mode
Execute saved poses sequentially
Step-by-step servo movement
Ideal for repeatable tasks like:
Pick and place
Sorting
Farming automation
📺 OLED User Interface
Real-time display of:
Servo angles
System status (LIVE / SAVED mode)
Execution progress
Visual feedback with progress bars
Interactive UI controlled via buttons
🔁 Bidirectional Communication
Feedback system sends servo positions back to controller
Ensures accurate monitoring
Helps in debugging and precision control
🔘 Multi-Button Control System
Button	Function
RESET	Moves all servos to home position (90°)
SAVE	Hold 2s → Save pose
DELETE	Hold 10s → Clear all memory
TOGGLE	Switch between LIVE and SAVED mode
EXECUTE	Run selected saved pose
📡 ESP-NOW Communication
Peer-to-peer wireless protocol
No WiFi router required
Fast and efficient data transfer
Reliable for real-time robotics control
⚙️ System Architecture
🧠 Controller Unit (Arduino Mega)
Reads joystick inputs
Handles button logic
Manages EEPROM storage
Displays data on OLED
Sends commands via Serial to ESP8266
📡 Transmitter (ESP8266)
Converts serial data to structured packets
Sends data via ESP-NOW
Receives feedback from receiver
🤖 Receiver Unit (ESP8266 + Servos)
Controls 5 servo motors
Executes commands
Sends real-time feedback
🔌 Hardware Components
Arduino Mega
2 × ESP8266 (NodeMCU / ESP-12)
5 × Servo Motors
2 × Joystick Modules
OLED Display (SSD1306, I2C)
Push Buttons
External 5V Power Supply
🧠 Capabilities
Manual control + Automation in one system
Memory-based repeatable actions
Wireless robotic control without internet
Expandable for:
AI vision
Object detection
Smart farming robots
Industrial automation
🔮 Future Improvements
Smooth trajectory planning (servo interpolation)
Mobile app control (WiFi/Bluetooth)
AI-based object detection & picking
Autonomous navigation integration
Safety fail-safe system
📌 Applications
Smart Farming 🌱
Pick and Place Systems
Robotics Learning Projects
Industrial Automation Prototypes
IoT + Robotics Integration
🏁 Conclusion

This project demonstrates the integration of:

Embedded Systems
Wireless Communication
Human-Machine Interface (HMI)
Robotics Control

It serves as a strong foundation for advanced robotics systems and can be extended into fully autonomous smart machines.
