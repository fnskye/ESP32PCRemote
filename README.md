## ESP32 Smart PC Power Controller

Control your PC's power button remotely using an ESP32, a relay module, and a web interface — ideal for headless setups, remote access, or smart home integration.

---

### Why I Made This

I created this project because I wanted a way to **turn my PC on and off using the ESP32**, both **locally** and **over the internet**. While local control was straightforward, **remote control via the internet** presented challenges — especially due to **CGNAT (Carrier-Grade NAT)** and the lack of a public IP.

There’s an existing method called **Wake-on-LAN (WoL)** that lets you wake a PC on the local network. If you only need **local access**, I recommend using that instead — it's much simpler to set up.  
🔍 [GitHub Search: Wake-on-LAN Projects](https://github.com/search?q=wake+on+lan)

However, **WoL typically doesn't work over the internet without complicated setup**, so for users who need **true remote access**, I began developing this project as an alternative solution.

My goal was to build a system that could simulate pressing the PC power button using a relay, with commands sent through a web interface — even from anywhere in the world.

> 🔧 **Note:** For full remote access over the internet, additional steps are required such as:
> - Using an always-on device (like a **Raspberry Pi**, another **ESP32**, or a home server)
> - Setting up tunneling tools like **Tailscale**, **ZeroTier**, or **Ngrok**

This repository focuses on developing the **core system** — the ESP32-based power control logic. If you're interested in the **internet-access setup**, I got a step-by-step tutorial separately to help you get started with secure remote access to your ESP32 controller below.

---

## Features

- 🖱️ Physical button or web-triggered PC power control
- 🌐 Simple web interface served from ESP32 (LittleFS)
- 🔁 1-second relay pulse simulates pressing your PC's power button
- 📶 Wi-Fi based — no need for external servers
- 💡 Status LED to indicate system is running

---

## Hardware Requirements

| Component                      | Quantity        |
|-------------------------------|-----------------|
| ESP32 Dev Board               | 1               |
| 5V Relay Module               | 1               |
| Push Button (normally open)   | 1               |
| LED + 220Ω Resistor           | 1 (for status)  |
| Jumper Wires + Breadboard     | As needed       |

> ⚠️ **Note:** These are the minimum requirements to run the system.  
> You are **free to customize or expand** the hardware setup as long as you understand what you're doing.

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

If you want to view and simulate the project virtually on Wokwi, you can check it out here along with the initial code:  
🔗 [Wokwi Simulation – ESP32 Remote Power Control](https://wokwi.com/projects/434588948519734273)

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

*Figure 3: Simulation of the power control feature — when the push button is pressed, the relay is activated for one second, mimicking a real PC power switch trigger.*

---

![Initial Testing Diagram #4](assets/explanation%20%231.png)

*Figure 4: Legend and explanation of the components and symbols used in the virtual diagrams above, providing clarity for understanding the simulation setup.*
