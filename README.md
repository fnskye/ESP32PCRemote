# ESP32 Smart PC Power Controller

### A Comprehensive Technical Guide on Implementing Remote Desktop PC Power Control Using an ESP32 Microcontroller with Relay Module and Web Interface Integration

This project presents a complete, detailed guide for developing a system that enables remote control of a desktop computer’s power state using an **ESP32 microcontroller**. By integrating a **relay module**, the ESP32 is capable of <ins>simulating a physical press of the PC’s power switch.</ins> This allows users to power their PC **locally** or **remotely via the internet**, with control issued through a responsive, browser-based user interface.

The project is initially designed for <ins>**local network access**</ins>, the system leverages the ESP32’s onboard Wi-Fi capabilities to host a web server that responds to user commands. The documentation also includes step-by-step instructions for extending the project’s functionality to support <ins>**outside local network access**</ins>, even in environments constrained by **Carrier-Grade NAT (CGNAT)** or lacking a dedicated **public IP address.**

Key areas of focus covered in this project include:
- Proper hardware setup and component wiring
- GPIO pin assignment and safe relay triggering logic
- Integration of a physical push-button for manual override
- LED status indicators for power state feedback
- Deployment of a lightweight web server hosted directly on the ESP32
- Solutions for secure, remote access using Tailscale, Ngrok, or Raspberry Pi bridges

This implementation is particularly suitable for users who need **reliable remote PC power-switch control** without depending on **Wake-on-LAN** or a public IP. By directly simulating your PC’s power button, it works across most standard desktop systems—perfect when all you want is “turn on the PC from anywhere,” not a full remote-desktop experience.

## **Important Notes before Proceeding**  ⚠️ 
It is strongly recommended that users review **all relevant sections** of this guide before starting. Doing so helps you:  
 > - Verify compatibility with your **specific hardware**, including ESP32 variants, relay modules, and PC motherboard specifications  
 > - Understand the requirements like tunneling tools and etc
 > - Familiarize yourself with safety considerations, proper wiring, and firmware flashing steps  

By reading end-to-end, you’ll gain the background needed for a **stable, robust deployment** and be better equipped to troubleshoot issues.  

The instructions and design principles in this repository are crafted to support both **beginner** users—who need clear, step-by-step walkthroughs—and **intermediate** users—who may want to customize or automate advanced behaviors. Whether you’re firing up your first ESP32 project or integrating this into an existing home-lab setup, this guide has you covered.

## Note on Wake-on-LAN and Always-On Devices

### Already Have a Raspberry Pi?

If you already have a **Raspberry Pi** with an Ethernet port or a model with reliable Wi-Fi access, you can implement **Wake-on-LAN (WoL)** to power on your PC, assuming that your **PC supports WoL** and is **connected via Ethernet** to your router.  

> This approach eliminates the need for an ESP32 relay system, offering a simpler and software-based method to remotely power on your PC.




You can use this method if your Raspberry Pi meets the following requirements:
> - Has a **LAN (Ethernet) port** for reliable WoL signaling  
> - Runs **64-bit Raspberry Pi OS (Bookworm)**  
> - (Optional) Supports **Raspberry Pi Connect** for full remote desktop access  

If your setup matches these conditions, you may skip the ESP32 steps and proceed directly to the **Wake-on-LAN section** of this guide.

Still, if you’re looking for a **cheap method**, you can:
> - Use **any low-cost like pi that can run 24/7** (like old laptop, cellphone, etc.)
> - Combine it with **Tailscale** or **Ngrok**
> - And still follow the rest of this guide to remotely trigger your ESP32 relay

---

### No Raspberry Pi & No WoL? No Problem!

Even if your PC **doesn’t support WoL** and you **don’t have a Raspberry Pi**, you can still **power on** your PC remotely by:

1. Using an **ESP32** to simulate the physical power button  
2. Tunneling into your network with **Ngrok** or **Tailscale**  
3. Relaying “press power” commands from an **always-on device**

> **This is exactly why this repository exists** — to help users who don’t have WoL-capable PCs or Raspberry Pi devices and still achieve remote **power-switch control** —not full mouse/keyboard desktop control.

#### Examples of Always-On-Device
- 🖥️ An old laptop or desktop that stays powered and online  
- 📱 A low-power Android phone or tablet on your LAN  
- 💻 Any device that can:
  • Stay online 24/7  
  • Join the **same local network** as your PC  
  • Maintain a stable internet connection  

> ⚠️ **Important:** The always-on device must be on the same network as the PC you're trying to control, and it should remain powered to ensure uninterrupted access.

---

## 📑 Table of Contents

<details>
<summary><strong>🔌 Introduction</strong></summary>

- [ESP32 Smart PC Power Controller](#esp32-smart-pc-power-controller)  
- [Project Overview](#a-comprehensive-technical-guide-on-implementing-remote-desktop-pc-power-control-using-an-esp32-microcontroller-with-relay-module-and-web-interface-integration)  
- [Important Notes ⚠️](#important-notes-before-proceeding-⚠️)  
- [Wake-on-LAN and Always-On Devices](#note-on-wake-on-lan-and-always-on-devices)  
  - [Using a Raspberry Pi](#already-have-a-raspberry-pi)  
  - [Without Raspberry Pi or WoL](#no-raspberry-pi--no-wol-no-problem)  
  - [Examples of Always-On Devices](#examples-of-always-on-device)  
- [Why I Made This](#why-i-made-this)  
- [Features](#features)  

</details>

<details>
<summary><strong>🔧 Hardware & Software Requirements</strong></summary>

- [Component Requirements](#component-requirements)  
  - [Component References](#component-references)  
- [Web Interface](#web-interface)  
  - [Available Pages](#available-pages)  
  - [Trigger Endpoint](#trigger-endpoint)  

</details>

<details>
<summary><strong>🧪 Simulation (Wokwi)</strong></summary>

- [Project Structure (In Wokwi)](#project-structure-in-wokwi)  
  - [Why Simulate First?](#why-simulate-first)  
  - [Simulation Goals](#main-goal-of-simulation)  
  - [Screenshots](#screenshot-of-the-diagram-initial-testing)  
    - [Figure 1](#figure-1-initial-diagram-setup-in-the-virtual-environment-showing-the-esp32-ready-to-connect-to-a-wi-fi-network)  
    - [Figure 2](#figure-2-successful-wi-fi-connection-with-the-esp32-displaying-its-assigned-ip-address-confirming-network-connectivity)  
    - [Figure 3](#figure-3-simulation-of-the-power-control-feature--when-the-push-button-is-pressed-the-relay-is-activated-for-one-second-simulating-like-a-real-pc-power-switch-trigger)  
  - [Diagram Explanation](#explanation-of-the-diagram-initial-testing)  
    - [Legend & Overview](#legend--functionality-overview)  
    - [Behavior in Simulation](#behavior-in-simulation)  

</details>

<details>
<summary><strong>⚙️ Building the Physical Setup</strong></summary>

- [Step-by-Step Hardware Setup](#step-by-step-hardware-setup)  
  - [Pre-Requisites](#pre-requisites-checklist)  
    - [1. Hardware Components](#1-hardware-components)  
    - [2. Software & Tools](#2-software--tools)  
    - [3. Cables and Power](#3-cables-and-power)  
  - [Installing Software](#installation-process-of-software--tools)  
    - [1. Arduino IDE](#1-install-arduino-ide)  
    - [2. ESP32 Board Support](#2-install-esp32-board-support)  
    - [3. USB Drivers](#3-install-usb-drivers-ch340--cp2102)  
    - [4. LittleFS Uploader](#4-install-littlefs-uploader)  
    - [5. Set Board & Port](#5-set-the-correct-board-and-port)  
  - [Uploading Code & Web Files](#uploading-the-code)  
    - [Uploading `.ino` Sketch](#1-uploading-the-ino-sketch)  
    - [Uploading Web Files](#2-uploading-web-files-interface-via-littlefs)  
  - [Simulation vs Actual Code](#differences-between-simulation-and-actual-code)  
    - [GPIO Pin Changes](#1-gpio-pin-changes)  
    - [New Features](#2-new-features-in-actual-code)  
  - [Wiring & Layout](#layouts-and-setups-of-the-actual-hardware-breadboard-and-wiring)  
    - [Overview](#general-setup-overview)  
    - [Breadboard Diagram](#breadboard-layout-diagram)  
    - [Diagram 1: Full Setup](#-diagram-1-complete-breadboard-setup-with-esp32)  
    - [Diagram 2: Relay to PC](#-diagram-2-relay-to-pc-power-switch-connection)  
    - [Wiring Table](#-pin-to-pin-wiring-table)  

</details>

<details>
<summary><strong>🌐 Remote Access (Internet Control)</strong></summary>

- [Accessing the Web Interface](#accessing-the-web-interface)  
  - [Why Upload Code & Web Files?](#why-we-are-uploading-code-and-web-files)  
- [Making ESP32 Accessible Online](#making-your-esp32-accessible-online)  
  - [Step-by-Step Guide Over CGNAT](#step-by-step-guide-for-accessing-your-esp32-online-over-cgnat-networks)  
  - [Tunneling with Ngrok or Tailscale](#setting-up-tailscale-or-ngrok)  
    - [Ngrok Setup](#tunneling-using-ngrok)  
    - [Tailscale Setup](#tunneling-using-tailscale)  
  - [Understanding CGNAT](#understanding-the-limitations-of-cgnat)  
  - [Workaround Solutions](#solution-tunnel-through-cgnat)  
    - [Tailscale](#tailscale--secure-persistent-and-private)  
    - [Ngrok](#ngrok--public-access-with-https-links)  
    - [Raspberry Pi Bridge](#raspberry-pi--247-bridge-for-remote-access)  
      - [Full Desktop Access](#raspberry-pi-connect--full-desktop-remote-access)  
      - [Terminal Access Only](#raspberry-pi-connect-lite--terminal-only-access)  
  - [Summary](#summary)  
  - [Installation Methods](#methods-of-installation)  

</details>

- [📜 License](#-license)


## Why I Made This

I created this project because I wanted a way to **turn my PC on and off using the ESP32**, both **locally** and **over the internet**. While local control was straightforward, **remote control via the internet** presented challenges — especially due to **CGNAT (Carrier-Grade NAT)** and the lack of a public IP.

There’s an existing method called **Wake-on-LAN (WoL)** that lets you wake a PC on the local network. If you only need **local access**, I recommend using that instead — it's much simpler to set up.  
🔍 [GitHub Search: Wake-on-LAN Projects](https://github.com/search?q=wake+on+lan++pc)

However, **WoL typically doesn't work over the internet without complicated setup**, so for users who need **true remote access**, I began developing this project as an alternative solution.

---

***My primary objective*** was to create a system that can simulate pressing the PC’s power button using a relay, with commands sent through a web interface. The goal was to make it work both locally and remotely, with a focus on allowing control from anywhere in the world through an internet connection. This gives users the ability to turn their PC on or off without needing to be near it or connected to the same local network.

> 🔧 **Note:** For full remote access over the internet, additional steps are required such as:
> - Using an **always-on device** (e.g., a Raspberry Pi or another device that stays online 24/7 to act as a secure gateway.)
> - Setting up tunneling tools like **Tailscale**, **ZeroTier**, or **Ngrok**

This repository focuses on the development of the core system — the ESP32 Smart PC Power Controller logic that handles the relay, physical button input, and status indication.

If you are ready to set up secure internet-based access, I've prepared a step-by-step tutorial (linked below) to help you configure remote control of your ESP32, even behind CGNAT.

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
| ![ESP32](assets/hardware_requirements_7.jpg) | **ESP32 Dev Board** — The main controller |
| ![Relay](assets/hardware_requirements_8.jpg) | **Relay Module** — triggers the PC Switch |
| ![Push Button](assets/hardware_requirements_4.jpg) | **Push Button** — used for manual test triggering |
| ![LED](assets/hardware_requirements_3.jpg) | **LED** — shows when relay and system is active |
| ![Resistor](assets/hardware_requirements_1.jpg) | **220Ω Resistor** — limits LED current && **10kΩ Resistor** — for pulling down push button |
| ![Jumpers](assets/hardware_requirements_5.jpg) | **Jumper Wires (M-M & M-F)** — essential for ESP32 components |

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

This web-based interface provides a simple, intuitive way to power on your PC without physical access — by just using a browser on any device.


## Project Structure (In Wokwi)

This project was initially developed and simulated using [Wokwi](https://wokwi.com/), a virtual simulator for Arduino and ESP32 development.

If you’d like to explore or simulate the project virtually, you can access the Wokwi simulation environment below, which includes the initial circuit setup and source code:

🔗 [Wokwi Simulation – ESP32 Remote Power Control](https://wokwi.com/projects/434588948519734273)

### Why Simulate First?

This project was first developed and tested in a **virtual environment** before using real hardware. This was done to make sure the code worked properly, the relay triggered as expected, the button and LED worked correctly, and no physical parts—especially those connected to the motherboard's power switch. Simulating the project helped make the development process safer, easier, and more beginner-friendly by finding and fixing problems early.

#### Main goal of simulation:

> The code logic works correctly.

> The relay triggers as it should.

> The button and LED behavior is functional.

> No physical components get damaged during development.

### Screenshot of the Diagram (Initial Testing)

Initial testing was done virtually using Wokwi to verify the logic and functionality of the ***ESP32 Smart PC Power Controller*** before applying it to the actual hardware. Below are the screenshots taken during different stages of the simulation:

---

![Initial Testing Diagram #1](assets/Project%20Structure%20%231.jpg)

#### **Figure 1:* Initial diagram setup in the virtual environment, showing the ESP32 ready to connect to a Wi-Fi network.*

---

![Initial Testing Diagram #2](assets/Project%20Structure%20%232.jpg)

#### **Figure 2:* Successful Wi-Fi connection with the ESP32 displaying its assigned IP address, confirming network connectivity.*

---

![Initial Testing Diagram #3](assets/Project%20Structure%20%233.jpg)

#### **Figure 3:* Simulation of the power control feature — when the push button is pressed, the relay is activated for one second, simulating like a real PC power switch trigger.*

---

### Explanation of the Diagram (Initial Testing)

The image below illustrates the **virtual wiring diagram** used for initial testing of the ESP32 Smart PC Power Controller in the Wokwi simulator. Each labeled part helps demonstrate how the ESP32, relay, LEDs, and buttons work together to simulate a PC power switch.

---

![Diagram Legend and Wiring](assets/explanation%20%231.png)

#### **Figure 4:* Legend and explanation of the components and symbols used in the virtual diagrams above, providing clarity for understanding the simulation setup.*

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

   > ⚠️ Don’t worry about setup steps just yet — we’ll cover all the installation instructions in the **Installation Process** section.

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
const char* ssid = "{Your Wi-fi SSID}";
const char* password = "{Your Wi-Fi Password}";
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
The web interface (HTML, CSS, JavaScript) is not stored inside the `.ino` sketch — it must be uploaded separately using **LittleFS**. 

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
| Push Button       | GPIO 12 (same)           | GPIO 12 (same)             | Same — no change at all |
| Relay LED         | Not implemented    | GPIO 4               | Added for visual feedback; in Wokwi, the relay directly simulated the PC |

---

#### 2. **New Features in Actual Code**

| Feature                        | Description |
|--------------------------------|-------------|
| **Relay LED Indicator**        | GPIO 4 is used to show when the relay is active — helps confirm physical triggering visually |
| **Debounce Logic**             | Prevents false triggering from physical button presses due to mechanical noise |
| **Relay Pulse Timing**         | Limits activation to 1 second to safely simulate a PC power press |

---

- Some **GPIO pins behave differently** on physical ESP32 boards than in simulation (e.g., boot behavior, signal stability).
- We added a **Relay LED** because on real hardware sometimes you can't "see" the relay toggle (depends on the model of the relay) — this gives real-time confirmation.
- Software **debounce and timing logic** are necessary to handle real button mechanics and safe PC triggering.
> Don't worry, as the overall project functionality is the same.  
> These improvements just make it **safer, clearer, and more stable** in a real-world setup.

---

### **Layouts and Setups of the Actual Hardware (Breadboard and Wiring)**

> **Why we do this:**  
> This section shows you how to physically assemble your ESP32 system — including how each component connects on the breadboard, what pins are used, and how to avoid common wiring mistakes.

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

### STEP BY STEP Guide for Accessing your ESP32 Online (Over CGNAT Networks)

> ⚠️ **Important Note:**  


## Setting up Tailscale or Ngrok
> - explanation

---
### Tunneling using Ngrok


---
### Tunneling using Tailscale

---


- 🔁 Automation tips for persistent access

---

Need help with a specific setup? Let me know and I’ll tailor the guide to your hardware or network!

### Understanding the Limitations of CGNAT

Many internet service providers (ISPs), especially those offering residential or mobile data plans, use **Carrier-Grade NAT (CGNAT)** to conserve public IPv4 addresses. Under CGNAT:

- Your router is assigned a **private IP address** (e.g., `10.x.x.x`, `100.x.x.x`).
- The public IP address is shared among multiple users.
- As a result, **port forwarding is not possible**, since the external traffic cannot reach your device directly.

> Even if port forwarding is configured on your router, it will not function unless your ISP assigns you a real public IP — which typically requires an upgrade to a business plan and extra monthly fees.

---

## Solution: Tunnel Through CGNAT

To make your ESP32 project accessible over the internet despite CGNAT, you can use **tunneling services** or **private VPN networks**. These tools allow you to reach your ESP32 from anywhere in the world — without relying on public IP addresses or port forwarding.

The most effective methods are:

---

### Tailscale – Secure, Persistent, and Private

[Tailscale](https://tailscale.com/) is a free, peer-to-peer VPN that creates a private network between your devices using WireGuard. It works perfectly behind CGNAT and firewalls.

- Creates a **private, encrypted tunnel** between your devices
- Automatically reconnects on reboot or network change
- Runs **indefinitely** without timeouts or manual restarts
- Perfect for **secure, long-term access**
- Ideal for projects where **only you or selected devices** need access to the ESP32

---

### Ngrok – Public Access with HTTPS Links

[Ngrok](https://ngrok.com/) is a tunneling tool that creates a **public HTTPS link** to a local server. This allows your ESP32's web interface (hosted on a PC or Raspberry Pi) to be accessed from anywhere — even under CGNAT.

- Provides a **temporary public URL** to any local port (e.g., ESP32 at `192.168.x.x:80`)
- Fully functional even without a public IP
- Can be configured to **restart automatically**, allowing it to run continuously
- Useful for **sharing access**, testing, or remote control via browser or phone

Although free Ngrok sessions expire after ~8 hours, a simple restart script allows it to run as long as your device is powered and connected.

---

### Raspberry Pi – 24/7 Bridge for Remote Access

If you have a **Raspberry Pi**, it can act as a persistent **online gateway** to your ESP32, giving you reliable, always-on remote access.

You can use it alongside Tailscale or Ngrok, or even take advantage of **Raspberry Pi Connect**, depending on your device model and OS.

> ⚠️ ***Important Note for Raspberry Pi Users:*** 

> If your Raspberry Pi has a **LAN port**, you can use it to directly **wake your PC via Wake-on-LAN**, without needing any tunneling at all.  

> This is the most efficient method — skip to the **Wake-on-LAN section** if this applies to you.

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

For older models or headless systems, **Connect Lite** provides basic but effective remote shell access — no desktop required.

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

In short:

- If your **Pi has Ethernet**, Wake-on-LAN may be all you need.
- If not, use **Tailscale** or **Ngrok** to access your ESP32 or PC remotely.
- If you're choosing a **cheap always-on device**, any low-cost Raspberry Pi will work — just make sure it stays powered.


### `Methods of Installation`


---

## 📜 License

This project is licensed under the MIT License. See [LICENSE](LICENSE) for more information.

---

> 🧠 *Built with love by [stillyoswa](https://github.com/fnskye)*





