# ESP32 Smart PC Power Controller

Control your PC's power button remotely using an ESP32, a relay module, and a web interface — perfect for remote setups, headless PCs, or DIY smart home integration.

---

## Features

- 🖱️ Physical button or web-triggered PC power control
- 🌐 Simple web interface served from ESP32 (LittleFS)
- 🔁 1-second relay pulse simulates pressing your PC's power button
- 📶 Wi-Fi based — no need for external servers
- 💡 Status LED to indicate system is running

---

## Hardware Requirements

| Component              | Quantity |
|------------------------|----------|
| ESP32 Dev Board        | 1         |
| 5V Relay Module        | 1        |
| Push Button (normally open) | 1   |
| LED + 220Ω resistor    | 1 (for status) |
| Jumper Wires + Breadboard | As needed |
| Power Adapter (5V micro-USB or USB-C) | 1 |

---

## Wiring Diagram (Simplified)

| ESP32 Pin | Connected To              |
|-----------|---------------------------|
| GPIO 18   | Relay IN                  |
| GPIO 5    | Status LED (through 220Ω) |
| GPIO 12   | Push Button (to GND)      |
| GND       | Relay GND + Button GND    |
| 5V        | Relay VCC (if required)   |

**Relay NO (Normally Open)** and **COM** connect to your PC's power switch header.

---

## Web Interface

- Hosted on the ESP32 using LittleFS
- Pages:
  - `/pc.html`: control panel
  - `/style.css`: basic styling
- Trigger endpoint: `POST /trigger`

---

## Project Structure

