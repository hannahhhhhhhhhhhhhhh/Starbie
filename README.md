# Starbie !
<img width="1810" height="919" alt="image" src="https://github.com/user-attachments/assets/c9d79570-6f7f-468a-bca1-861cc322a18c" />
Starbie is a custom gun shaped digital pet designed in KiCad.
It features an OLED display that shows a character, which reacts to its environment using onboard sensors. 
The MPU6050 accelerometer and gyroscope module detects movement and orientation, allowing the character to respond when the badge is tilted or shaken. 
The DHT11 sensor reads the surrounding temperature and humidity, which can influence how the character behaves or what gets displayed on screen. 
Two push buttons allow the user to manually interact with the badge. 
All components communicate with the ESP32-C3 over I2C or direct GPIO pins as defined in the schematic, and the board is powered over USB-C through the XIAO module.

---

## Screenshots of the schematic and pcb editor <3
<img width="1660" height="745" alt="image" src="https://github.com/user-attachments/assets/c95a227f-2b6f-48ee-a6a1-4a0bb7cb84bb" />
<img width="1350" height="776" alt="image" src="https://github.com/user-attachments/assets/2072128b-5478-42ee-9ca4-75f847362d47" />
<img width="1342" height="743" alt="image" src="https://github.com/user-attachments/assets/3050bc89-7c75-4e0a-80a1-5f3b9b097c50" />


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
## Why I Made This

I think a digital pet is a really cool concept and that we can code it based on our interests so I mostly followed the idea of the tutorial, but also I wanted it to look and act different which is why I designed it differently. I had a lot of fun making this project and learned a lot about kicad.

## Firmware Flashing Guide

### You Need:
- Arduino IDE
- ESP32 board package installed in Arduino IDE
- USB-C cable
- Install the ESP32 Board Package

### Connecting the Board
1. Plug the XIAO ESP32-C3 into your computer via USB-C
2. Wait for drivers to install

### Selecting the Board in Arduino IDE
1. Go to Tools > Board > esp32 > XIAO_ESP32C3
2. Go to Tools > Port and select the port that appeared when you 
   plugged in the board 

### Upload Sketch
1. Open `Starbie.ino` in Arduino IDE
2. Click the Upload button
3. Wait for "Done uploading" to appear
4. Done!!

## Bill of Materials

| Item | Qty | Supplier | Cost/Item (CAD) | Total Cost (CAD) |
|------|-----|----------|-----------------|------------------|
| XIAO ESP32-S3 | 1 | Seeed Studio | $7.49 | $7.49 |
| SW_Push Button | 1 | YUNZII | $12.99 | $12.99 |
| Conn_01x07 7-pin Header | 1 | Seeed Studio | $0.64 | $0.64 |
| PCB Fabrication (JLCPCB) | 1 | JLCPCB | $9.40 | $9.40 |
| Estimated Shipping | 1 | — | — | $2.00 |
| | | | **Total** | **$32.52** |
