# ESP32 Smart PC Power Controller

### A Comprehensive Technical Guide on Implementing Remote Desktop PC Power Control Using an ESP32 Microcontroller with Relay Module and Web Interface Integration

This project presents a complete, technically detailed guide for developing a system that enables remote control of a desktop computer’s power state using an **ESP32 microcontroller**. By integrating a **relay module**, the ESP32 is capable of simulating a physical press of the PC’s power switch. This allows users to power their PC **locally** or **remotely via the internet**, with control issued through a responsive, browser-based user interface.

The project is initially designed for **local network access**, the system leverages the ESP32’s onboard Wi-Fi capabilities to host a web server that responds to user commands. The documentation also includes step-by-step instructions for extending the project’s functionality to support **remote internet access**, even in environments constrained by **Carrier-Grade NAT (CGNAT)** or lacking a dedicated public IP address.

Key areas of focus covered in this project include:
- Proper hardware setup and component wiring
- GPIO pin assignment and safe relay triggering logic
- Integration of a physical push-button for manual override
- LED status indicators for power state feedback
- Deployment of a lightweight web server hosted directly on the ESP32
- Solutions for secure, remote access using Tailscale, Ngrok, or Raspberry Pi bridges

This implementation is particularly suitable for users who seek **reliable remote PC access** without depending on ***Wake-on-LAN functionality or public IP address availability***. By directly simulating the PC’s power button, this method offers compatibility across most standard desktop systems.

**Note:** For users who already possess a **Raspberry Pi** with an Ethernet port, you can proceed with **Wake-on-LAN (WoL)** methods for remote PC activation. If this applies, you may choose to bypass the relay-based approach and proceed directly to the WoL configuration section.  
If you are using a ***lower-cost Raspberry Pi model*** or a device **without LAN support**, using an ESP32 and hosting through Ngrok or Tailscale remain viable and effective alternatives — provided that an **always-on host device** is available to relay commands to the ESP32.

---

**Important:**  
It is strongly recommended that users review all relevant sections of this guide before starting. This ensures compatibility with your hardware and network environment, and provides the context necessary for a secure and stable deployment.  
The instructions and design principles in this repository aim to support both beginner and intermediate developers working on personal automation, smart home integration, or remote IT management tasks.

---

## Table of Contents

- [Why I Made This](#why-i-made-this)
  - [Important Notes for Raspberry Pi Users](#already-have-a-raspberry-pi)
- [Features of the Project](#features)
- [Hardware Requirements](#hardware-requirements)
  - [IComponent/Hardware References](#component-references)
- [Wiring Diagram (Simplified)](#wiring-diagram-simplified)
- [Web Interface](#web-interface)
- [Project Structure (In Wokwi)](#project-structure-in-wokwi)
  - [Why Simulate First?](#why-simulate-first)
      - [Main goal of simulation:](#main-goal-of-simulation)
  - [Screenshot of the Diagram (Initial Testing)](#screenshot-of-the-diagram-initial-testing)
      - [Figure 1: Initial diagram setup](#figure-1-initial-diagram-setup-in-the-virtual-environment-showing-the-esp32-ready-to-connect-to-a-wi-fi-network)   
      - [Figure 2: Successful Wi-Fi connection](#figure-2-successful-wi-fi-connection-with-the-esp32-displaying-its-assigned-ip-address-confirming-network-connectivity)   
      - [Figure 3: Simulation of the power control feature](#figure-3-simulation-of-the-power-control-feature--when-the-push-button-is-pressed-the-relay-is-activated-for-one-second-mimicking-a-real-pc-power-switch-trigger)   
      - [Figure 4: Legend and explanation of the components and symbols](#figure-4-legend-and-explanation-of-the-components-and-symbols-used-in-the-virtual-diagrams-above-providing-clarity-for-understanding-the-simulation-setup)   
- [Actual Testing (Hardware Deployment)](#-summary)
  - [Pre-Requisites Checklist](#-summary)
- [Summary and Device Recommendations](#-summary)
- [Summary and Device Recommendations](#-summary)
- [Summary and Device Recommendations](#-summary)

- [Methods of Installation](#methods-of-installation)
- [Wake-on-LAN Setup](#wake-on-lan-setup)
- [ESP32 Power Control Logic](#esp32-power-control-logic)
- [Security and Best Practices](#-security-considerations)
- [Solution: Tunnel Through CGNAT](#-solution-tunnel-through-cgnat)
  - [Tailscale – Private, Secure Access](#-tailscale--secure-persistent-and-private)
  - [Ngrok – Public Access via HTTPS](#-ngrok--public-access-with-https-links)
  - [Raspberry Pi – 24/7 Bridge](#-raspberry-pi--247-bridge-for-remote-access)
    - [Raspberry Pi Connect](#-raspberry-pi-connect--full-desktop-remote-access)
    - [Raspberry Pi Connect Lite](#-raspberry-pi-connect-lite--terminal-only-access)
---

### Why I Made This

I created this project because I wanted a way to **turn my PC on and off using the ESP32**, both **locally** and **over the internet**. While local control was straightforward, **remote control via the internet** presented challenges — especially due to **CGNAT (Carrier-Grade NAT)** and the lack of a public IP.

There’s an existing method called **Wake-on-LAN (WoL)** that lets you wake a PC on the local network. If you only need **local access**, I recommend using that instead — it's much simpler to set up.  
🔍 [GitHub Search: Wake-on-LAN Projects](https://github.com/search?q=wake+on+lan++pc)

However, **WoL typically doesn't work over the internet without complicated setup**, so for users who need **true remote access**, I began developing this project as an alternative solution.

### Already have a Raspberry Pi?

>
> If you already own a **Raspberry Pi** and it has an **Ethernet port**, you can use it to **send Wake-on-LAN packets directly to your PC** — even from outside your network — using tools like **Tailscale**, **Ngrok**, or **Raspberry Pi Connect**.  
> This allows you to **skip the relay-based ESP32 system entirely** if your PC supports WoL.
>
> ✅ If your Pi meets these requirements:
> - Has a **LAN port** (for reliable WoL signaling)
> - Runs **64-bit Raspberry Pi OS Bookworm**
> - (Optional) Supports **Raspberry Pi Connect** for full desktop access
>
> Then you can skip straight to the **Wake-on-LAN section** in this guide.

Still, if you’re looking for a **cheap method**, you can:
- Use **any low-cost Pi** (like Pi Zero, Pi 3, etc.)
- Combine it with **Tailscale** or **Ngrok**
- And still follow the rest of this guide to remotely trigger your ESP32 relay

---

***My primary objective*** was to create a system that can simulate pressing the PC’s power button using a relay, with commands sent through a web interface. The goal was to make it work both locally and remotely, with a focus on allowing control from anywhere in the world through an internet connection. This gives users the ability to turn their PC on or off without needing to be near it or connected to the same local network.

> 🔧 **Note:** For full remote access over the internet, additional steps are required such as:
> - Using an **always-on device** (e.g., a Raspberry Pi or another device that stays online 24/7 to act as a secure gateway.)
> - Setting up tunneling tools like **Tailscale**, **ZeroTier**, or **Ngrok**

This repository focuses on the development of the core system — the ESP32 Smart PC Power Controller logic that handles the relay, physical button input, and status indication.

If you are ready to set up secure internet-based access, I've prepared a step-by-step tutorial (linked below) to help you configure remote control of your ESP32, even behind CGNAT.

---

## **Features**

- **The system supports dual control modes:** a physical push-button connected to the ESP32 for local PC power activation, and a web-based interface for remote triggering through a network connection.
- This setup does **NOT HARM the computer**, as it only delivers a minimal voltage and enough to simulate a momentary contact across the Power SW **(Power Switch) pins**, just like a physical button press in your PC case.
- This setup **retains** the functionality of the PC’s original case-mounted power switch, allowing users to power the system on or off manually at any time, in parallel with the ESP32 Smart PC Power Controller.
- This system uses a **1-second relay pulse** to safely simulate a press of your PC’s power button and a status LED included to indicate when the system is running.

> (adding more soon)

---

## **Hardware Requirements**

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

## **Wiring Diagram (Simplified)**

| ESP32 Pin | Connected To              |
|-----------|---------------------------|
| GPIO 18   | Relay IN                  |
| GPIO 5    | Status LED (through 220Ω) |
| GPIO 12   | Push Button (to GND)      |
| GND       | Relay GND + Button GND    |
| 5V        | Relay VCC (if required)   |

We will use a Relay **NO (Normally Open)** and **COM** to connect to your PC's power switch header.

---

## **Web Interface**

- Hosted on the ESP32 using LittleFS
- Pages:
  - `/pc.html`: control panel
  - `/style.css`: basic styling
- Trigger endpoint: `POST /trigger`

---

## **Project Structure (In Wokwi)**

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

#### *Figure 2: Successful Wi-Fi connection with the ESP32 displaying its assigned IP address, confirming network connectivity.*

---

![Initial Testing Diagram #3](assets/Project%20Structure%20%233.jpg)

#### *Figure 3: Simulation of the power control feature — when the push button is pressed, the relay is activated for one second, mimicking a real PC power switch trigger.*

---

### Explanation of the Diagram (Initial Testing)

The image below illustrates the **virtual wiring diagram** used for initial testing of the ESP32 Smart PC Power Controller in the Wokwi simulator. Each labeled part helps demonstrate how the ESP32, relay, LEDs, and buttons work together to simulate a PC power switch.

---

![Diagram Legend and Wiring](assets/explanation%20%231.png)

#### *Figure 4: Legend and explanation of the components and symbols used in the virtual diagrams above, providing clarity for understanding the simulation setup.*

**Legend & Functionality Overview:**

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

**Behavior in Simulation:**
- When the push button is pressed, GPIO 12 reads HIGH.
- This triggers the relay via GPIO 4, simulating a PC power press for one second.
- Simultaneously, GPIO 5 turns on the status LED to indicate that the action was taken.
- Console logs (seen at the bottom of the simulation) show system startup and Wi-Fi connection status.

> This setup was tested virtually in Wokwi to validate the logic before applying it to physical hardware.

---

## **Actual Testing (Hardware Deployment)**

Since our initial testing in [Wokwi](https://wokwi.com/projects/434588948519734273) was successful, we can now proceed with **actual hardware testing** using a real ESP32 and physical components.

Before we begin, please make sure you’ve completed the following requirements to ensure a smooth deployment:

---

### Pre-Requisites Checklist

Before You Begin

Please make sure you have the following prepared before flashing the project to your ESP32:

1. **Hardware Components**  
   Ensure you have everything listed in the [Hardware Requirements](#hardware-requirements), including:
   - ESP32 Dev Board
   - 5V Relay Module
   - Push Button (optional for testing)
   - LED + 220Ω Resistor
   - Breadboard, Jumper Wires, and a suitable USB Cable (Type-C or Micro-USB)

2. **Software & Tools**  
   To run this project on actual hardware, you'll need:
   - [**Arduino IDE**](https://www.arduino.cc/en/software)
   - ESP32 Board Package installed in the IDE
   - Required USB drivers for your ESP32 (CH340, CP2102, etc.)
   - [**LittleFS Uploader Tool**](https://github.com/earlephilhower/arduino-littlefs-upload) to flash the web interface to ESP32 storage

   > ⚠️ Don’t worry about setup steps just yet — we’ll cover all the installation instructions in the **Installation Process** section.

3. **Cables and Power**  
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

#### 4. Install LittleFS Uploader
We use LittleFS to upload web interface files to the ESP32.

- Visit the repo: [arduino-littlefs-upload](https://github.com/earlephilhower/arduino-littlefs-upload)
- Follow the instructions to install the `LittleFS Uploader` tool.
- After installing, restart Arduino IDE.
- You should now see **"ESP32 Sketch Data Upload"** under the **Tools** menu.

#### 5. Set the Correct Board and Port
- Go to **Tools > Board**, and select your ESP32 model (e.g., **ESP32 Dev Module**).
- Go to **Tools > Port**, and select the COM port assigned to your ESP32.
- Also set:
- **Flash Size**: 4MB (or match your board)
- **Partition Scheme**: "Default 4MB with spiffs (1.2MB APP/1.5MB SPIFFS)" (or similar)

---

## **Uploading the Code**

You can view or download the main sketch file here:  
🔗 [sketch_esp32.ino](https://github.com/fnskye/ESP32PCRemote/blob/main/sketch_esp32.ino)

1. Open the `.ino` file in Arduino IDE.
2. Connect your ESP32 to your PC using a **data-capable USB cable**.
3. Confirm:
   - Correct board is selected under **Tools > Board**
   - Correct port is selected under **Tools > Port**
4. Click the **Upload** button (checkmark icon).
5. Wait for the "Done Uploading" message.

> If you see "Connecting....", hold the **BOOT** button on the ESP32 until upload starts (for some boards).

---

## **Uploading Web Files (LittleFS)**

Your ESP32 will serve the web interface from internal storage using **LittleFS**.

1. Create a folder named `/data` in the same directory as your `.ino` file.
2. Place your web files (HTML, JS, CSS) inside `/data`.
3. Go to **Tools > ESP32 Sketch Data Upload**.
4. Wait for upload to complete. It should say something like: [SPIFFS] upload : 100% complete


> If you don’t see this option, double-check your LittleFS tool installation.

---

## **Accessing the Web Interface**

1. Open **Serial Monitor** from Arduino IDE (**Tools > Serial Monitor**) and set baud rate to **115200**.
2. After boot, the ESP32 will attempt to connect to Wi-Fi and print the **local IP address**, like:

> Connected to WiFi!
> IP Address: (e.g., `http://192.168.1.42`)

3. Open a browser and go to `http://<ESP32-IP>` (e.g., `http://192.168.1.42`).
4. You should see the **Smart PC Power Controller** web interface.

---

## **Why We Are Uploading Code and Web Files?**

### 1. Uploading the `.ino` Sketch 
The `.ino` file contains the main **logic and control code** for the ESP32. 

This includes:

- Connecting to your Wi-Fi network
- Handling HTTP requests from the browser
- Activating the relay when triggered
- Controlling the status LED
- Reading input from the push button

> Without uploading this code, the ESP32 will be blank and won't know what to do.

### 2. Uploading Web Files (Interface via LittleFS)
The web interface (HTML, CSS, JavaScript) is not stored inside the `.ino` sketch — it must be uploaded separately using **LittleFS**. 

These files:

- Serve the user interface you see in your browser
- Provide a button to trigger the PC power switch
- Run inside the ESP32's internal file system (not an SD card)

> These allow your ESP32 to become a fully working **smart PC power controller** that you can access from any device on your local network.

---

## **Differences Between Simulation and Actual Code**

While the Wokwi simulation helps prototype the logic, actual hardware development often involves tweaking pins and adding physical indicators for real-world interaction and stability. Here we can see the key changes in the final version.

---

### 1. **GPIO Pin Changes**
| Function          | Wokwi (Simulation) | Actual ESP32 Sketch | Notes |
|-------------------|--------------------|----------------------|-------|
| Relay Control     | GPIO 4             | GPIO 18              | Changed for better GPIO stability and layout compatibility on real board |
| Status LED        | GPIO 5 (same)             | GPIO 5 (same)            | Same - used for system status indicator (always ON) |
| Push Button       | GPIO 12 (same)           | GPIO 12 (same)             | Same — no change at all |
| Relay LED         | Not implemented    | GPIO 4               | Added for visual feedback; in Wokwi, the relay directly simulated the PC |

---

### 2. **New Features in Actual Code**

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

## **Layouts and Setups of the Actual Hardware (Breadboard and Wiring)**

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

![Breadboard Diagram](assets/Project%20Structure%20#1.jpg)

_(Replace this with your final wiring diagram if updated)_

---

### Pin-to-Pin Wiring Table

| ESP32 Pin | Connects To           | Description                         |
|-----------|------------------------|-------------------------------------|
| GPIO 18   | Relay IN               | Relay control (Active LOW)          |
| GPIO 5    | Status LED (+220Ω)     | Indicates system power              |
| GPIO 4    | Relay LED (+220Ω)      | Lights up when relay is triggered   |
| GPIO 12   | Push Button (to GND)   | Manual trigger                      |
| GND       | Relay GND, Button GND, LED GND | Common ground for all components |
| 3.3V/5V   | Relay VCC, LED VCC     | Depending on component requirement  |

> 💡 Use **pull-down resistor (10kΩ)** for the push button if needed for stability  
> 🧪 Add a **diode** across the relay coil (if mechanical) to avoid back EMF (optional but recommended)

---

## **Making Your ESP32 Accessible Online (Over CGNAT Networks)**

### Understanding the Limitations of CGNAT

Many internet service providers (ISPs), particularly those offering residential or mobile data plans, use **Carrier-Grade Network Address Translation (CGNAT)** to conserve public IPv4 addresses. Under CGNAT:

- Your router is assigned a **private IP address** (e.g., `10.x.x.x`, `100.x.x.x`).
- The public IP address is shared among many subscribers.
- As a result, **port forwarding is not possible**, since the external traffic cannot be routed directly to your device.

> Even if port forwarding is configured correctly on your router, it will not function under CGNAT unless you are assigned a true public IP address.

---

### 📦 Can You Request a Public IP?

Yes — in some cases, a public IP address can be requested from your ISP. However:

- This often incurs an **additional monthly fee**.
- It may require upgrading to a **business plan** or submitting a formal request.
- Some ISPs do **not offer public IP addresses** for residential customers at all.

Due to these limitations, it is typically more practical to use alternative methods to make your ESP32 accessible remotely.

---

## Making Your ESP32 Accessible Online (Over CGNAT Networks)

> **Important Note:**  
> If you already have a **Raspberry Pi** with a **LAN (Ethernet) port**, you can use it to **send Wake-on-LAN (WoL) packets** to power up your PC remotely.  
> In that case, you may **skip the tunneling section** and proceed directly to the **Wake-on-LAN setup** later in this guide.
>
> However, if you're looking for a cheaper option or don't have an Ethernet-capable Pi, you can still:
> - Use **Ngrok** or **Tailscale** to control your ESP32 or PC over the internet
> - Choose any **low-cost, always-on device** (like Pi Zero, Pi 3, or old laptops)
>
> Just make sure that whichever method you choose, **the device stays online** to keep your tunnel or network active.

---

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




