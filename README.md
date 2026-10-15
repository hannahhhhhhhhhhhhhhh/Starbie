# Starbie !
<img width="1810" height="919" alt="image" src="https://github.com/user-attachments/assets/c9d79570-6f7f-468a-bca1-861cc322a18c" />
Starbie is a custom gun shaped digital pet designed in KiCad.

---

## Hardware Specifications & Component List
To fully assemble the Starbie PCB,  the following components are needed:

*   **Microcontroller:** 1x Seeed Studio XIAO-ESP32-C3
*   **Sensors:** 
    *   1x DHT11 Environment Sensor (Temperature & Humidity)
    *   1x MPU6050 6-Axis Motion Tracking Module (Accelerometer & Gyroscope)
*   **Display:** 1x I2C OLED Display Module (4-pin setup: VCC, GND, SCL, SDA)
*   **Discreet Components:**
    *   2x 6x6mm Tactile Push Buttons (SW_Push)
    *   1x 10kΩ Axial Resistor (Pull-up for DHT11 data line)

---

##  Project Structure Info
*   `/starbie.kicad_pro` - Main KiCad project configuration file.
*   `/starbie.kicad_pcb` - 2-layer gun shaped PCB layout file.
*   `/starbie.kicad_sch` - Schematic capture file mapping the components and I2C buses.


Also included the gerber files for the pcb
---


