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

We will use a Relay **NO (Normally Open)** and **COM** to connect to your PC's power switch header.

---

## Web Interface

- Hosted on the ESP32 using LittleFS
- Pages:
  - `/pc.html`: control panel
  - `/style.css`: basic styling
- Trigger endpoint: `POST /trigger`

---

## Project Structure (In Wokwi)

This project was initially developed and simulated using [Wokwi](https://wokwi.com/), a virtual simulator for Arduino and ESP32 development.

### Why Simulate First?

I built and tested the project virtually before applying it to real hardware to ensure that:

- The code logic works correctly
- The relay triggers as expected
- The button and LED behavior is functional
- No physical components get damaged during development

Simulating the project first helped me **identify and fix potential issues early**, saving time and avoiding unnecessary troubleshooting on real hardware.
This project was initially developed and simulated in Wokwi, a virtual electronics simulator for Arduino and ESP32 projects.

### Screenshot of the Diagram (Initial Testing)

Initial testing was done virtually using Wokwi to verify the logic and functionality of the ESP32-based remote power control project before applying it to the actual hardware. Below are the screenshots taken during different stages of the simulation:

---

![Initial Testing Diagram #1](assets/Project%20Structure%20%231.jpg)

*Figure 1: Initial diagram setup in the virtual environment, showing the ESP32 ready to connect to a Wi-Fi network.*

---

![Initial Testing Diagram #2](assets/Project%20Structure%20%232.jpg)

*Figure 2: Successful Wi-Fi connection with the ESP32 displaying its assigned IP address, confirming network connectivity.*

---

![Initial Testing Diagram #3](assets/Project%20Structure%20%233.jpg)

*Figure 3: Simulation of the power control feature — when the push button is pressed, the relay is activated for one second, simulating PC power switch trigger.*

