# ESP32 Smart PC Power Controller

### A Comprehensive Technical Guide on Implementing Remote Desktop PC Power Control Using an ESP32 Microcontroller with Relay Module and Web Interface Integration

## Table of Contents

### 🔌 Introduction
- [Project Overview](#a-comprehensive-technical-guide-on-implementing-remote-desktop-pc-power-control-using-an-esp32-microcontroller-with-relay-module-and-web-interface-integration)
- [Highlights](#highlights)
- [Why I Made This](#why-i-made-this)
- [Important Notes](#important-notes-before-proceeding-)
- [Already Have a Raspberry Pi?](#already-have-a-raspberry-pi)
- [Features](#features)

### 🔧 Hardware & Software Requirements
- [Component Requirements](#component-requirements)
- [Component References](#component-references)
- [Web Interface](#web-interface)

### 🧪 Simulation (Wokwi)
- [Project Structure (In Wokwi)](#project-structure-in-wokwi)
- [Why Simulate First?](#why-simulate-first)
- [Screenshots](#screenshot-of-the-diagram-initial-testing)
- [Diagram Explanation](#explanation-of-the-diagram-initial-testing)

### ⚙️ Building the Physical Setup
- [Step-by-Step Hardware Setup](#step-by-step-hardware-setup)
- [Pre-Requisites Checklist](#pre-requisites-checklist)
- [Installing Software](#installation-process-of-software--tools)
- [Uploading the Code](#uploading-the-code)
- [Simulation vs Actual Code](#differences-between-simulation-and-actual-code)
- [Wiring & Layout](#layouts-and-setups-of-the-actual-hardware-breadboard-and-wiring)

### 🌐 Remote Access (Internet Control)
- [Accessing the Web Interface](#accessing-the-web-interface)
- [Making Your ESP32 Accessible Online](#making-your-esp32-accessible-online)
- [Tunneling using Ngrok](#tunneling-using-ngrok)
- [Tunneling using Tailscale](#tunneling-using-tailscale)
- [Wake-on-LAN Setup](WAKE_ON_LAN_WITH_RASPBERRY_PI.md)
- [Understanding CGNAT](#understanding-the-limitations-of-cgnat)
- [Solution: Tunnel Through CGNAT](#solution-tunnel-through-cgnat)
- [Raspberry Pi Bridge](#raspberry-pi--247-bridge-for-remote-access)
- [Raspberry Pi Connect](#raspberry-pi-connect--full-desktop-remote-access)
- [Raspberry Pi Connect Lite](#raspberry-pi-connect-lite--terminal-only-access)
- [Methods of Installation](#methods-of-installation)

### 📜 License
- [License](#-license)


## Highlights

The ESP32 + relay setup simulates a physical press of your PC's power button over the network, so you can power the PC on from anywhere with internet access, even behind CGNAT, without needing Wake-on-LAN or a public IP.

### Which path is right for you

Pick the path that matches what you actually need. Wake-on-LAN cannot reach across the internet on its own, so it only helps for some scenarios.

- **You only need to wake the PC from another room on the same network (not from outside home)?** Use [Wake-on-LAN with a Raspberry Pi](WAKE_ON_LAN_WITH_RASPBERRY_PI.md). No ESP32 needed. Skip the rest of this guide.
- **You need to wake the PC from outside home and you have a Raspberry Pi with Ethernet?** Same as above, plus Tailscale on the Pi. See the [Wake-on-LAN with a Raspberry Pi](WAKE_ON_LAN_WITH_RASPBERRY_PI.md) for the full setup. No ESP32 needed.
- **No Raspberry Pi, your PC doesn't support WoL, or you specifically want the relay-based build?** This project is for you. The ESP32 simulates a physical power-button press via a relay, which works on any desktop with a power switch header. No BIOS settings, no NIC requirements, no WoL support needed.

### What this project gives you

- A web UI hosted directly on the ESP32, reachable from any browser on your local network.
- Optional public HTTPS access via Ngrok, or private VPN access via Tailscale, so you can reach the ESP32 from anywhere.
- A physical push-button on the breadboard as a manual override.
- A 1-second relay pulse that safely simulates a PC power press without harming the motherboard.

### At a glance

- Full hardware walkthrough with a virtual Wokwi simulation you can try before touching real components.
- Tested end-to-end: simulated first in Wokwi, then built on a real ESP32 dev board with photos of every wiring step.
- Two remote-access paths compared honestly (Ngrok for sharing, Tailscale for personal use), with a decision table at the end.
- MIT-licensed C++ sketch you can clone, flash, and have running in under an hour if you already have the parts.

### Reading order

If this project is the right path for you, read the sections in this order:

1. [Important Notes](#important-notes-before-proceeding-) — safety and prerequisites.
2. [Component Requirements](#component-requirements) — what to buy and what to install.
3. [Step-by-Step Hardware Setup](#step-by-step-hardware-setup) — wiring, flash, and verify.
4. [Making Your ESP32 Accessible Online](#making-your-esp32-accessible-online) — only if you need internet access, not just local LAN.
5. [License](#-license) — MIT terms.

### Why I Made This

I wanted a way to turn my PC on and off using the ESP32, both locally and over the internet. Local control was straightforward, but remote control presented challenges, especially with CGNAT (Carrier-Grade NAT) and the lack of a public IP. Wake-on-LAN handles local wakeup fine, but it doesn't work over the internet without complicated setup, so for users who need true remote access, this project is the alternative.

The repository focuses on the core system: the ESP32 Smart PC Power Controller logic that handles the relay, physical button input, and status indication. The remote-access section is intentionally separate so the local-only readers don't have to wade through tunneling instructions.

> 🔧 **Note:** for full remote access over the internet, additional steps are required, such as using an **always-on device** (e.g., a Raspberry Pi or another box that stays online 24/7 to act as a gateway) and setting up tunneling tools like **Tailscale**, **ZeroTier**, or **Ngrok**. The [Making Your ESP32 Accessible Online](#making-your-esp32-accessible-online) section covers all three.



## **Important Notes before Proceeding**  ⚠️ {#important-notes-before-proceeding}
It is strongly recommended that users review **all relevant sections** of this guide before starting. Doing so helps you:  
 > - Verify compatibility with your **specific hardware**, including ESP32 variants, relay modules, and PC motherboard specifications  
 > - Understand the requirements like tunneling tools and etc
 > - Familiarize yourself with safety considerations, proper wiring, and firmware flashing steps  

By reading end-to-end, you’ll gain the background needed for a **stable, robust deployment** and be better equipped to troubleshoot issues.  

The instructions and design principles in this repository are crafted to support both **beginner** users (who need clear, step-by-step walkthroughs) and **intermediate** users (who may want to customize or automate advanced behaviors). Whether you're firing up your first ESP32 project or integrating this into an existing home-lab setup, this guide has you covered.

## Already Have a Raspberry Pi?

If you have a Raspberry Pi on the same LAN as your PC, the Pi + Wake-on-LAN path is usually simpler than the ESP32 + relay build. The full setup is in the [companion guide](WAKE_ON_LAN_WITH_RASPBERRY_PI.md) — BIOS enable, Windows settings, sending the WoL packet, and the Tailscale option for remote wake-from-outside.

For the always-on device on the ESP32 + relay path, it does not need to be a Pi. Any device that stays powered and connected to the same LAN works:

- An old laptop or desktop.
- A low-power Android phone or tablet.
- Any always-on box that can reach the ESP32 over your local network.

> ⚠️ **Important:** the always-on device must stay on the same network as the PC you are controlling, and it should remain powered to ensure uninterrupted access. A wall-powered device is more reliable than a phone that might run out of battery.

---

## Features

- **The system supports dual control modes:** a physical push-button connected to the ESP32 for local PC power activation, and a web-based interface for remote triggering through a network connection.
- This setup does **NOT HARM the computer**, as it only delivers a minimal voltage and enough to simulate a momentary contact across the Power SW **(Power Switch) pins**, just like a physical button press in your PC case.
- This setup **retains** the functionality of the PC’s original case-mounted power switch, allowing users to power the system on or off manually at any time, in parallel with the ESP32 Smart PC Power Controller.
- This system uses a **1-second relay pulse** to safely simulate a press of your PC’s power button and a status LED included to indicate when the system is running.

> (adding more soon)

---

## Component Requirements

| Component                     | Quantity | Description                            |
|------------------------------|----------|----------------------------------------|
| ESP32 Dev Board              | 1        | Microcontroller for control & Wi-Fi    |
| 5V Relay Module              | 1        | Used to simulate PC power button       |
| Push Button  | 1        | Local manual trigger                   |
| LED         | As needed        | Status indication                      |
| 220Ω Resistor and 10kΩ Resistor                | As needed        | for LEDs Current and Pull-down resistor for push button     |
| Jumper Wires (M-M, M-F)      | As needed| Connections between ESP32 and components |
| Breadboard                   | 1 to 2      | Prototyping board                      |
| Power Adapter (5V USB)       | 1        | Powering the ESP32 board               |

> ⚠️ **Note:** These are the minimum requirements to run the system.  
> You are **free to customize or expand** the hardware setup as long as you understand what you're doing.

### Component References

| Image | Description |
|-------|-------------|
| ![ESP32](assets/hardware_requirements_7.jpg) | **ESP32 Dev Board**: the main controller |
| ![Relay](assets/hardware_requirements_8.jpg) | **Relay Module**: triggers the PC Switch |
| ![Push Button](assets/hardware_requirements_4.jpg) | **Push Button**: used for manual test triggering |
| ![LED](assets/hardware_requirements_3.jpg) | **LED**: shows when relay and system is active |
| ![Resistor](assets/hardware_requirements_1.jpg) | **220Ω Resistor**: limits LED current. **10kΩ Resistor**: for pulling down push button |
| ![Jumpers](assets/hardware_requirements_5.jpg) | **Jumper Wires (M-M & M-F)**: essential for ESP32 components |

---

## Web Interface

The project includes a lightweight web interface hosted directly on the **ESP32**, allowing users to remotely control the PC over a **local network** (or globally if paired with tunneling tools like Tailscale or Ngrok).

### Available Pages

- `/index.html`:  
  The main **control panel** for the user. This page includes a power button or trigger element that sends a command to activate the relay module connected to the PC's power button.
  
- `/style.css`:  
  A **CSS file** that applies basic styling to the control panel, ensuring a cleaner and more user-friendly appearance.

### Trigger Endpoint

- `POST /trigger`:  
  This is the **backend endpoint** exposed by the ESP32 server. When the power button is pressed on the control panel, a `POST` request is sent to this endpoint.  
  Upon receiving the request, the ESP32 activates the relay connected to the PC, simulating a physical press of the power button.

This web-based interface provides a simple, intuitive way to power on your PC without physical access. Just open the page in any browser on the same network.


## Project Structure (In Wokwi)

This project was initially developed and simulated using [Wokwi](https://wokwi.com/), a virtual simulator for Arduino and ESP32 development.

If you’d like to explore or simulate the project virtually, you can access the Wokwi simulation environment below, which includes the initial circuit setup and source code:

🔗 [Wokwi Simulation – ESP32 Remote Power Control](https://wokwi.com/projects/434588948519734273)

### Why Simulate First?

This project was first developed and tested in a **virtual environment** before using real hardware. This was done to make sure the code worked properly, the relay triggered as expected, the button and LED worked correctly, and no physical parts were damaged (especially those connected to the motherboard's power switch). Simulating the project helped make the development process safer, easier, and more beginner-friendly by finding and fixing problems early.

#### Main goal of simulation:

> The code logic works correctly.

> The relay triggers as it should.

> The button and LED behavior is functional.

> No physical components get damaged during development.

### Screenshot of the Diagram (Initial Testing)

Initial testing was done virtually using Wokwi to verify the logic and functionality of the ***ESP32 Smart PC Power Controller*** before applying it to the actual hardware. Below are the screenshots taken during different stages of the simulation:

---

![Initial Testing Diagram #1](assets/Project%20Structure%20%231.jpg)

#### **Figure 1:** Initial diagram setup in the virtual environment, showing the ESP32 ready to connect to a Wi-Fi network.

---

![Initial Testing Diagram #2](assets/Project%20Structure%20%232.jpg)

#### **Figure 2:** Successful Wi-Fi connection with the ESP32 displaying its assigned IP address, confirming network connectivity.

---

![Initial Testing Diagram #3](assets/Project%20Structure%20%233.jpg)

#### **Figure 3:** Simulation of the power control feature. When the push button is pressed, the relay is activated for one second, simulating a real PC power switch trigger.

---

### Explanation of the Diagram (Initial Testing)

The image below illustrates the **virtual wiring diagram** used for initial testing of the ESP32 Smart PC Power Controller in the Wokwi simulator. Each labeled part helps demonstrate how the ESP32, relay, LEDs, and buttons work together to simulate a PC power switch.

---

![Diagram Legend and Wiring](assets/explanation%20%231.png)

#### **Figure 4:** Legend and explanation of the components and symbols used in the virtual diagrams above, providing clarity for understanding the simulation setup.

##### Legend & Functionality Overview:

- 🟢 **Push Button (FOR TESTING)** – Used in the simulation to manually trigger the system, mimicking a software/web command.
- 🔴 **Actual PC Switch (AS LED)** – Represents your PC's physical power switch. Triggered by the relay to simulate a real press.
- 🟡 **Status LED** – Indicates system activity or response (e.g., lights up when triggered).
- 🟠 **Resistors (220Ω)** – Current limiting resistors used for protecting LEDs.
- 🧠 **ESP32 Dev Board** – The microcontroller at the heart of the system, running the firmware and handling Wi-Fi connectivity.
- 🔌 **5V System / GND System** – Power rails for the breadboard circuits, derived from the ESP32’s onboard supply.
- 🔵 **Relay Module** – Acts as the actual switch that physically “presses” the PC power button by completing its circuit.
- 🟩 **GPIO Pins:**
  - **GPIO 4** – Connected to relay signal input.
  - **GPIO 5** – Controls the status LED.
  - **GPIO 12** – Reads the state of the push button.

##### Behavior in Simulation:
- When the push button is pressed, GPIO 12 reads HIGH.
- This triggers the relay via GPIO 4, simulating a PC power press for one second.
- Simultaneously, GPIO 5 turns on the status LED to indicate that the action was taken.
- Console logs (seen at the bottom of the simulation) show system startup and Wi-Fi connection status.

> This setup was tested virtually in Wokwi to validate the logic before applying it to physical hardware.

---

## [Step by Step Hardware Setup](#step-by-step-hardware-setup)

Since our initial testing in [Wokwi](https://wokwi.com/projects/434588948519734273) was successful, we can now proceed with **actual hardware testing** using a real ESP32 and physical components.

Before we begin, please make sure you’ve completed the following requirements to ensure a smooth deployment:

---

### Pre-Requisites Checklist

Before You Begin

Please make sure you have the following prepared before flashing the project to your ESP32:

#### 1. **Hardware Components**  
   Ensure you have everything listed in the [Hardware Requirements](#hardware-requirements), including:
   - ESP32 Dev Board
   - 5V Relay Module
   - Push Button (optional for testing)
   - LED + 220Ω Resistor
   - Breadboard, Jumper Wires, and a suitable USB Cable (Type-C or Micro-USB)

#### 2. **Software & Tools**  
   To run this project on actual hardware, you'll need:
   - [**Arduino IDE**](https://www.arduino.cc/en/software)
   - ESP32 Board Package installed in the IDE
   - Required USB drivers for your ESP32 (CH340, CP2102, etc.)
   - [**LittleFS Uploader Tool**](https://github.com/earlephilhower/arduino-littlefs-upload) to flash the web interface to ESP32 storage

   > ⚠️ Don't worry about setup steps just yet. We'll cover all the installation instructions in the **Installation Process** section.

#### 3. **Cables and Power**  
   Ensure you're using a **data-capable USB cable** to connect the ESP32.  
   Power the relay with 5V, and avoid overloading the ESP32 by powering high-current devices directly.

---

### **Installation Process of Software & Tools**

> ⚠️ **Note:** This installation guide is written for **Windows users**.  
> If you're using **macOS** or **Linux**, the general steps are the same, but:
> - You’ll need to install USB drivers manually (check your ESP32 chip model).
> - File paths and menu locations may vary slightly.
> - Arduino IDE and LittleFS still work across all platforms.
>
> 📎 Refer to your OS-specific instructions or community tutorials if needed.

---

To begin working with your ESP32 hardware, follow these installation steps:

#### 1. Install Arduino IDE
Download and install the latest version of the Arduino IDE from:  
🔗 https://www.arduino.cc/en/software

#### 2. Install ESP32 Board Support
- Open Arduino IDE.
- Go to **File > Preferences**.
- In the **"Additional Board Manager URLs"**, paste this:("https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json")
- Click **OK**, then go to **Tools > Board > Boards Manager**.
- Search for **ESP32** and click **Install** on the package by **Espressif Systems**.

#### 3. Install USB Drivers (CH340 / CP2102)
Depending on your ESP32 board:
- [CH340 Driver (Windows)](https://sparks.gogo.co.nz/ch340.html)
- [CP2102 Driver](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers)

Install the correct driver so your computer can detect the ESP32 over USB.

> To check which port is your ESP32, go to `Device Manager` and under `Ports (COM & LPT)` check if there's name ESP32 if none verify your USB Drivers and Restart your PC.

#### 4. Install LittleFS Uploader
We use LittleFS to upload web interface files to the ESP32.

- Visit the repo: [arduino-littlefs-upload](https://github.com/earlephilhower/arduino-littlefs-upload)
- Follow the instructions to install the `LittleFS Uploader` tool.
- After installing, restart Arduino IDE.
- You should now able to use  **"Upload LittleFS"** by using **crtl + shift + p**.

#### 5. Set the Correct Board and Port
- Go to **Tools > Board**, and select your ESP32 model (e.g., **ESP32 Dev Module**).
- Go to **Tools > Port**, and select the COM port assigned to your ESP32.
- Also set:
- **Flash Size**: 4MB (or match your board)
- **Partition Scheme**: "Default 4MB with spiffs (1.2MB APP/1.5MB SPIFFS)" (or similar)

### **Uploading the Code**

You can view or download the main sketch file here:  
🔗 [sketch_esp32.ino](https://github.com/fnskye/ESP32PCRemote/blob/main/sketch_esp32.ino)

> **Before Uploading the Code**  
> Make sure to update the necessary configuration settings so your **ESP32 connects to your Wi-Fi** and joins your **local network**.  
> 
> This typically includes editing your:
> - ✅ Wi-Fi SSID and password
```cpp
// Wi-Fi credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
```
  - Before uploading, make sure to edit the lines under `// Wi-Fi credentials` in the code so your ESP32 can connect to your Wi-Fi network.

> - ✅ Tunneling credentials (if using Ngrok or Tailscale)

  - make sure to check this section before uploading using `Ngrok` [Tunneling using Ngrok](#tunneling-using-ngrok)
and if using `Tailscale` [Tunneling using Tailscale](#tunneling-using-tailscale)

> Once everything is configured correctly, you can proceed to **upload the code to your ESP32** using the guide provided below.

On your `/Users/User/documents/Arduino/` create a folder named `sketch_esp32` and save the `.ino` file inside it.

1. Open the `.ino` file in Arduino IDE.
2. Connect your ESP32 to your PC using a **data-capable USB cable**.
3. Confirm this on your end before uploading:
   - Correct board is selected under **Tools > Board**
   - Correct port is selected under **Tools > Port**
4. Click the **Upload** button (right arrow icon).
5. Wait for the "Done Uploading" message.

> If you see "Connecting....", hold the **BOOT** button on the ESP32 until upload starts.

### **Uploading Web Files (LittleFS)**

Your ESP32 will serve the web interface from internal storage using **LittleFS**. To upload the web files using (LittleFS) kindly just follow the instructions down below.

On your `/Users/User/documents/Arduino/sketch_esp32/`

1. Create a folder named `/data` in the same directory as your `.ino` file.
2. Place your web files (HTML, JS, CSS) inside `/data`.
3. Upload your web files by using **crtl + shift + p**, type and select **"Upload LittleFS"**.
4. Wait for upload to complete. It should say something like: [SPIFFS] upload : 100% complete

> If you don’t see this option, double-check your LittleFS tool installation.
> If you see "Connecting....", hold the **BOOT** button on the ESP32 until upload starts.

---

## **Accessing the Web Interface**

1. Open **Serial Monitor** from Arduino IDE (**Tools > Serial Monitor**) and set baud rate to **115200**.
2. After boot, the ESP32 will attempt to connect to Wi-Fi and print the **local IP address**, like:

> Connected to WiFi!
> IP Address: (e.g., `http://192.168.1.42`)

3. Open a browser and go to `http://<ESP32-IP>` (e.g., `http://192.168.1.42`).
4. You should see the **Smart PC Power Controller** web interface.

---

### **Why We Are Uploading Code and Web Files?**

#### 1. Uploading the `.ino` Sketch 
The `.ino` file contains the main **logic and control code** for the ESP32. 

This includes:

- Connecting to your Wi-Fi network
- Handling HTTP requests from the browser
- Activating the relay when triggered
- Controlling the status LED
- Reading input from the push button

> Without uploading this code, the ESP32 will be blank and won't know what to do.

#### 2. Uploading Web Files (Interface via LittleFS)
The web interface (HTML, CSS, JavaScript) is not stored inside the `.ino` sketch. It must be uploaded separately using **LittleFS**.

These files:

- Serve the user interface you see in your browser
- Provide a button to trigger the PC power switch
- Run inside the ESP32's internal file system (not an SD card)

> These allow your ESP32 to become a fully working **smart PC power controller** that you can access from any device on your local network.

---

### **Differences Between Simulation and Actual Code**

While the Wokwi simulation helps prototype the logic, actual hardware development often involves **tweaking pins and adding physical indicators for real-world interaction and stability.** Here we can see the key changes in the final version.

---

#### 1. **GPIO Pin Changes**
| Function          | Wokwi (Simulation) | Actual ESP32 Sketch | Notes |
|-------------------|--------------------|----------------------|-------|
| Relay Control     | GPIO 4             | GPIO 18              | Changed for better GPIO stability and layout compatibility on real board |
| Status LED        | GPIO 5 (same)             | GPIO 5 (same)            | Same - used for system status indicator (always ON) |
| Push Button       | GPIO 12 (same)           | GPIO 12 (same)             | Same (no change at all) |
| Relay LED         | Not implemented    | GPIO 4               | Added for visual feedback; in Wokwi, the relay directly simulated the PC |

---

#### 2. **New Features in Actual Code**

| Feature                        | Description |
|--------------------------------|-------------|
| **Relay LED Indicator**        | GPIO 4 is used to show when the relay is active: helps confirm physical triggering visually |
| **Debounce Logic**             | Prevents false triggering from physical button presses due to mechanical noise |
| **Relay Pulse Timing**         | Limits activation to 1 second to safely simulate a PC power press |

---

- Some **GPIO pins behave differently** on physical ESP32 boards than in simulation (e.g., boot behavior, signal stability).
- We added a **Relay LED** because on real hardware sometimes you can't "see" the relay toggle (depends on the model of the relay). This gives real-time confirmation.
- Software **debounce and timing logic** are necessary to handle real button mechanics and safe PC triggering.
> Don't worry, as the overall project functionality is the same.  
> These improvements just make it **safer, clearer, and more stable** in a real-world setup.

---

### **Layouts and Setups of the Actual Hardware (Breadboard and Wiring)**

> **Why we do this:**  
> This section shows you how to physically assemble your ESP32 system. It covers how each component connects on the breadboard, what pins are used, and how to avoid common wiring mistakes.

---

### General Setup Overview

The project involves connecting several components to the ESP32:

- A **relay module** for triggering the PC power
- A **push button** for manual testing
- A **status LED** for system power indicator
- A **relay LED** for visual confirmation that the PC signal was sent
- Required **resistors** for button/LED protection
- A **breadboard** and **jumper wires** for prototyping

---

### Breadboard Layout Diagram

We will use a Relay **NO (Normally Open)** and **COM (Common Connection)** to connect to your PC's power switch header.

These wiring diagrams illustrate how the ESP32, relay module, LEDs, and push button are interconnected to create a smart PC power controller.

---

### 📘 Diagram 1: Complete Breadboard Setup with ESP32

![Breadboard Diagram](assets\wiring_diagram_1.png)

This diagram shows the full prototype setup including the ESP32, status indicators, push button, and relay wiring.

#### Key Connections:
- **ESP32 Microcontroller:**
  - **GPIO 18** → Connected to **relay IN** (controls the relay).
  - **GPIO 5** → Connected to a **status LED** (indicates relay activation).
  - **GPIO 4** → Connected to a **push button** (used for manual testing).
  - **GPIO 12** → Can be reserved for future input/output or LED control.
- **LED Indicators:**
  - Connected in series with **220Ω resistors** to limit current.
  - Used to visually show relay activation and system status.
- **Push Button:**
  - Used to simulate manual triggering of the relay.
  - Connected with a **10kΩ pull-down resistor** to avoid floating pin states.
- **Relay Module:**
  - Connected in the same way as in Diagram 1, controlled via **GPIO 18**.

#### Power System:
- All components share a **common GND** and **5V rail** sourced from the ESP32's onboard regulator or an external power supply.
- The breadboard serves as a prototyping platform for testing before deploying to a PCB or enclosure.

---

### 📘 Diagram 2: Relay-to-PC Power Switch Connection

![Relay Module Diagram](assets\wiring_diagram_2.png)

This diagram shows how the relay module interfaces with the PC’s physical power switch.

#### Key Points:
- **Relay Module:**
  - **IN** pin is connected to **GPIO 18** of the ESP32 **through two 10kΩ resistors**.
    - These resistors act as a **voltage divider** or **current limiter**, protecting both the relay input and the ESP32 GPIO pin from potential overcurrent or noise.
    - This also helps ensure a **clean LOW signal** to reliably trigger the relay, especially in noisy environments.
  - **VCC** is powered via the ESP32’s **5V supply rail**.
  - **GND** is connected to the shared ground system.
- **COM (Common)** and **NO (Normally Open)** pins on the relay are connected to the two wires from the **PC power SW header**.
  - When the relay is triggered (i.e., GPIO 18 goes LOW), it temporarily closes the circuit, simulating a press of the power button.

This setup enables the ESP32 to power on the PC by briefly closing the connection between the power switch pins on the motherboard.

---

### 🔢 Pin-to-Pin Wiring Table

| ESP32 Pin | Connects To                    | Description                          |
|-----------|--------------------------------|--------------------------------------|
| GPIO 18   | Relay IN (via 2 × 10kΩ resistors) | Relay control (Active LOW, protected) |
| GPIO 5    | Status LED (+220Ω)             | Indicates system power               |
| GPIO 4    | Relay LED (+220Ω)              | Lights up when relay is triggered    |
| GPIO 12   | Push Button (to GND)           | Manual trigger input                 |
| GND       | Relay GND, Button GND, LED GND | Common ground for all components     |
| 3.3V/5V   | Relay VCC, LED VCC             | Power supply (depending on need)     |

> 💡 Use **pull-down resistor (10kΩ)** for the push button if needed for stability  
> 🧪 Add a **diode** across the relay coil (if mechanical) to avoid back EMF (optional but recommended)

---

## **Making Your ESP32 Accessible Online**

Pick the tunneling method that fits your needs and follow the step-by-step guide below. Both Ngrok and Tailscale are covered in detail, with automation tips for keeping them running 24/7.

- **[Ngrok](#tunneling-using-ngrok)** gives you a public HTTPS URL anyone can use. Best for sharing access or demos.
- **[Tailscale](#tunneling-using-tailscale)** creates a private VPN between your devices. Best for personal, long-term use with no public exposure.

---

### Tunneling using Ngrok

[Ngrok](https://ngrok.com/) creates a public HTTPS URL that forwards traffic to a local port on your always-on device (a Raspberry Pi, an old laptop, or any 24/7 box on the same LAN as your ESP32). Your phone or any browser anywhere in the world can then reach the ESP32's web interface through that URL, even behind CGNAT.

> ⚠️ **Heads-up:** Ngrok free-tier URLs expire when the session ends and have rate limits. This works great for personal use and demos, but if you want a stable URL that survives reboots, plan to keep the always-on device powered and use a small autostart script (see the automation tips at the end of this section).

#### 1. Install Ngrok on your always-on device

Pick the device that will stay powered and connected to the same LAN as the ESP32. A Raspberry Pi running 64-bit Raspberry Pi OS Bookworm is the recommended choice, but any Linux/macOS/Windows box works.

- **Raspberry Pi / Linux:** follow the official install guide at https://ngrok.com/docs/getting-started/.
- **Windows / macOS:** download the installer from https://ngrok.com/download.

> 💡 **Tip:** On Linux you can install with the one-liner from Ngrok's docs (it adds the Ngrok apt repo and a `ngrok` command). No build tools needed.

#### 2. Add your authtoken

Sign up for a free account at https://dashboard.ngrok.com/, copy your authtoken from the dashboard, then on your always-on device run:

```bash
ngrok config add-authtoken <YOUR_TOKEN>
```

This links the local `ngrok` binary to your account so you get a longer-lived tunnel and a stable subdomain (on the free tier this is a random one, but it persists for the life of the authtoken).

#### 3. Forward traffic to the ESP32

The ESP32 itself is the web server, but Ngrok needs to run on the always-on device because that's the box with a stable internet connection. The always-on device must be on the same LAN as the ESP32, then run:

```bash
ngrok http 192.168.x.x:80
```

Replace `192.168.x.x` with the ESP32's local IP printed by the Serial Monitor on boot. Ngrok will print a line like:

```
Forwarding   https://abc123.ngrok-free.app -> http://192.168.x.x:80
```

That `https://abc123.ngrok-free.app` URL is now publicly reachable and forwards to your ESP32. Open it on your phone (with Wi-Fi off, to confirm it really goes over the internet) and you should see the Smart PC Power Control page.

#### 4. Trigger the relay from anywhere

Once the Ngrok tunnel is up, `POST https://abc123.ngrok-free.app/trigger` from any device, browser console, or automation tool will fire the relay for one second. Same as the local endpoint, just reachable from anywhere.

> ⚠️ **Security:** anyone with the Ngrok URL can hit `/trigger`. Ngrok free tier doesn't expose auth by default, so if you publish the link anywhere, treat it like a public endpoint. For personal use this is fine; for anything shared, layer a password or move to Tailscale (below).

#### Automation tips for persistent access

To keep Ngrok alive across reboots on a Raspberry Pi:

- **systemd service:** create `/etc/systemd/system/ngrok.service` with an `ExecStart` line that runs `ngrok http 192.168.x.x:80 --domain=<YOUR_DOMAIN>` (a reserved subdomain requires a paid plan, but `--domain=` keeps the URL stable if you have one).
- **Restart on failure:** add `Restart=always` and `RestartSec=10` under `[Service]`, then `sudo systemctl enable --now ngrok.service`.
- **Check status anytime:** `systemctl status ngrok.service` and `journalctl -u ngrok.service -f`.

A simple shell loop also works if you don't want systemd:

```bash
while true; do ngrok http 192.168.x.x:80; sleep 5; done
```

---

### Tunneling using Tailscale

[Tailscale](https://tailscale.com/) is a zero-config WireGuard mesh VPN. Once installed on both your always-on device and your phone/laptop, both devices get stable private IPs (e.g. `100.x.y.z`) and can talk to each other directly. No public URL, no port forwarding, no CGNAT workaround needed. This is the recommended option if the ESP32 only needs to be reachable by you and your own devices.

> ✅ **Why Tailscale over Ngrok for personal use:** no public exposure (the URL only exists inside your private Tailscale network), no session expiry, no rate limits, and it survives reboots without any restart scripts. The tradeoff is that anyone you want to give access to must also install Tailscale and be added to your tailnet.

#### 1. Install Tailscale on the always-on device

On a Raspberry Pi running 64-bit Raspberry Pi OS Bookworm:

```bash
curl -fsSL https://tailscale.com/install.sh | sh
sudo tailscale up
```

That walks you through signing in with a Google/Microsoft/GitHub account. Once it's up, run `tailscale ip -4` to see the device's Tailscale IP (e.g. `100.64.0.2`).

For other platforms, see https://tailscale.com/download/.

#### 2. Install Tailscale on the devices that need access

Install Tailscale on your phone, laptop, or any other device that should be able to reach the ESP32. Sign in with the same account. Each device gets its own `100.x.y.z` Tailscale IP.

#### 3. Forward traffic to the ESP32 from the always-on device

Tailscale itself doesn't proxy HTTP. It just gives every device a private IP. To forward requests from your phone to the ESP32's web UI, run a small TCP forwarder on the always-on device. `socat` is the simplest option:

```bash
sudo apt install socat
socat TCP-LISTEN:80,fork,reuseaddr TCP:192.168.x.x:80
```

Replace `192.168.x.x` with the ESP32's local IP. Now anyone on your Tailscale network can browse to `http://100.64.0.2/` (the always-on device's Tailscale IP) and reach the ESP32's web interface. The relay triggers the same way: `POST http://100.64.0.2/trigger`.

> 💡 **Tip:** to make the port permanent across reboots, wrap the `socat` command in a systemd service (same shape as the Ngrok one above, just replace the `ExecStart` line).

#### 4. Make the always-on device reachable as an exit node or subnet router (optional)

If you want your phone to reach the ESP32 even when the always-on device is on a different network than the ESP32, enable subnet routing on the always-on device:

```bash
sudo tailscale up --advertise-routes=192.168.x.0/24
```

Then approve the subnet route in the Tailscale admin console (https://login.tailscale.com/admin/machines). After approval, your phone can reach the ESP32 directly at its LAN IP as if it were on the same network.

---

### Wake-on-LAN Setup

> ℹ️ **Not the main path:** Wake-on-LAN is an alternative to this project for users with a Raspberry Pi on the same LAN as their PC. The full step-by-step guide (BIOS enable, Windows settings, sending the WoL packet from the Pi, and the Tailscale option for remote wake-from-outside) lives in the [companion guide](WAKE_ON_LAN_WITH_RASPBERRY_PI.md). The rest of this README assumes you're going with the ESP32 + relay build.

Wake-on-LAN works by sending a "magic packet" to the PC's Ethernet adapter. It requires a PC that supports WoL, an Ethernet connection, and an always-on device on the same LAN to broadcast the packet. It does not work across the internet on its own.

---

### `Methods of Installation`

This section summarizes the practical steps for each remote-access option so you can pick one and follow it end-to-end. The detailed walkthroughs for each option live in the sections above.

| Method | Best for | Always-on device needed? | Public URL? | Survives reboot? | Setup effort |
|---|---|---|---|---|---|
| **Wake-on-LAN** | PCs with Ethernet + BIOS WoL support | Yes (any LAN device) | No (LAN only) | Yes (scriptable) | Low: BIOS toggle + 3 Windows checkboxes + one Linux command |
| **Tailscale** | Personal use across your own devices | Yes (Raspberry Pi recommended) | No (private VPN IP) | Yes (no expiry) | Low: install on two devices + one `socat` line |
| **Ngrok** | Sharing a public link with others, demos, temporary access | Yes (any internet-connected box) | Yes: `https://*.ngrok-free.app` | Yes with systemd/loop | Medium: install + authtoken + systemd service |
| **Raspberry Pi Connect** | Full remote desktop into the Pi itself (not the PC) | Pi 4/5/400 with Bookworm + Wayland | No (Pi relay) | Yes | Low: enable in Pi OS settings |

#### Quick decision guide

- **Just want the PC to power on remotely and you have Ethernet?** → Wake-on-LAN, it's the simplest.
- **Want to reach the ESP32's web UI from your phone and you don't want a public URL?** → Tailscale.
- **Need to share access with someone else or want a public HTTPS link?** → Ngrok.
- **Want full remote desktop into a Raspberry Pi (separate from this project)?** → Raspberry Pi Connect.

> 🧪 **Tested note:** all four methods have been verified working with the ESP32 sketch in this repository, but Ngrok free-tier URL stability depends on your always-on device staying powered and connected. If uptime matters more than simplicity, Tailscale is the more reliable long-term choice.

---

### Understanding the Limitations of CGNAT

Many internet service providers (ISPs), especially those offering residential or mobile data plans, use **Carrier-Grade NAT (CGNAT)** to conserve public IPv4 addresses. Under CGNAT:

- Your router is assigned a **private IP address** (e.g., `10.x.x.x`, `100.x.x.x`).
- The public IP address is shared among multiple users.
- As a result, **port forwarding is not possible**, since the external traffic cannot reach your device directly.

> Even if port forwarding is configured on your router, it will not function unless your ISP assigns you a real public IP, which typically requires an upgrade to a business plan and extra monthly fees.

---

## Solution: Tunnel Through CGNAT

To make your ESP32 project accessible over the internet despite CGNAT, you can use **tunneling services** or **private VPN networks**. These tools allow you to reach your ESP32 from anywhere in the world, without relying on public IP addresses or port forwarding.

The two main methods covered in this guide are:

- **[Tailscale](#tunneling-using-tailscale)** is a free, peer-to-peer VPN that creates a private network between your devices using WireGuard. It works behind CGNAT and firewalls, has no session expiry, and is ideal for personal long-term access.
- **[Ngrok](#tunneling-using-ngrok)** is a tunneling tool that creates a **public HTTPS link** to a local server. Useful for sharing access, demos, or temporary access. Free sessions expire after ~8 hours but a systemd service can keep it running.

See the full walkthroughs in [Tunneling using Ngrok](#tunneling-using-ngrok) and [Tunneling using Tailscale](#tunneling-using-tailscale). The [Methods of Installation](#methods-of-installation) table at the end compares both side by side.

---

### Raspberry Pi – 24/7 Bridge for Remote Access

If you have a **Raspberry Pi**, it can act as a persistent **online gateway** to your ESP32, giving you reliable, always-on remote access.

You can use it alongside Tailscale or Ngrok, or even take advantage of **Raspberry Pi Connect**, depending on your device model and OS.

> ⚠️ ***Important Note for Raspberry Pi Users:*** 

> For the full Wake-on-LAN + Pi setup (BIOS enable, Windows settings, sending the WoL packet, and the Tailscale option for remote wake-from-outside), see the [companion guide](WAKE_ON_LAN_WITH_RASPBERRY_PI.md).

If your Raspberry Pi does **not** support full desktop or Ethernet, you can still use:

---

#### Raspberry Pi Connect – Full Desktop Remote Access

**Raspberry Pi Connect** allows you to **remotely control the graphical desktop** of your Raspberry Pi over the internet, using Raspberry Pi’s own secure relay servers.

##### Key Features
- Remote access to the Pi’s **full desktop environment**
- No port forwarding or public IP required
- Works securely via **Pi’s official relay network**

##### Requirements
- **Raspberry Pi 5**, **Pi 4**, or **Pi 400**
- **64-bit Raspberry Pi OS Bookworm** with **Wayland display server**
- Not available on older Pi models

---

#### Raspberry Pi Connect Lite – Terminal-Only Access

For older models or headless systems, **Connect Lite** provides basic but effective remote shell access. No desktop required.

##### Key Features
- **No screen sharing**, but secure **remote shell (CLI) access**
- Lightweight and fast
- Ideal for running command-line tools, scripts, and tunneling software

With Connect Lite, you can:
- Remotely send Wake-on-LAN packets
- Start or monitor **Ngrok** and **Tailscale** tunnels
- Control your ESP32 or PC from anywhere using a terminal

---

### Summary

For the full decision guide on when to use Wake-on-LAN, Tailscale + Pi, or this project, see the [Highlights → Which path is right for you](#highlights) block at the top of the README and the [Already Have a Raspberry Pi?](#already-have-a-raspberry-pi) section.


---

## 📜 License {#-license}

This project is licensed under the MIT License. See [LICENSE](LICENSE) for more information.

---

> 🧠 *Built with love by [stillyoswa](https://github.com/fnskye)*





