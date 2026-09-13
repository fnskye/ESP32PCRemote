# Wake-on-LAN with Raspberry Pi (companion guide)

This is a companion guide for users who have a Raspberry Pi on the same
LAN as their PC and want to power the PC on remotely without the
ESP32 + relay build. It lives outside the main README because it is
an alternative approach, not part of the ESP32 project.

The main README's Highlights block points to this file from two
places (the "Which path is right for you" section and the "Already
Have a Raspberry Pi?" section). See `README.md` for the link.

## Why this is separate

This project is built around an ESP32 + relay that simulates a
physical press of the PC's power button. That works on any desktop
with a power switch header, no BIOS or NIC requirements. The
Wake-on-LAN + Raspberry Pi path is simpler in some setups but only
works when your PC's NIC and motherboard both support WoL and the
PC is connected via Ethernet.

If you have a Raspberry Pi with Ethernet and your PC supports WoL,
the Pi path is usually the easier route. If you don't have either,
or you specifically want the relay-based build, use the main
ESP32PCRemote project.

## Three paths to choose from

1. **Wake the PC from another room on the same network.**
   Use Wake-on-LAN with any always-on device on your LAN (Pi, old
   laptop, phone, etc.). The magic packet is broadcast on the LAN
   and the PC wakes up.

2. **Wake the PC from outside your home network.**
   Combine the always-on device with Tailscale so the device gets
   a private IP reachable from anywhere. The always-on device then
   sends the WoL packet over your LAN, which the PC picks up even
   though the wake request originated from across the internet.

3. **No Raspberry Pi, no WoL support, or you want the relay build.**
   Use the main ESP32PCRemote project (see `README.md`). The ESP32
   simulates a physical press of the PC's power button via a relay,
   so it works on any desktop.

## Step 1: Enable Wake-on-LAN in the PC's BIOS

Restart the PC and enter the BIOS/UEFI setup (commonly `Del`, `F2`,
or `F10` on boot). Look for one of these labels, usually under
**Power Management** or **Advanced**:

- `Wake on LAN`
- `Power On by PCI-E`
- `ErP Ready` (must be **disabled**, since ErP cuts standby power
  and disables WoL)

Set WoL to **Enabled** and save + exit. The exact menu name depends
on your motherboard manufacturer (ASUS, MSI, Gigabyte, ASRock all
use slightly different labels).

## Step 2: Enable Wake-on-LAN in Windows

After the PC boots back into Windows:

1. Open **Device Manager** (right-click the Start button, then
   Device Manager).
2. Expand **Network adapters**, right-click your **Ethernet
   adapter**, then **Properties**.
3. Open the **Power Management** tab and check:
   - **Allow this device to wake the computer**
   - **Only allow a magic packet to wake the computer** (recommended,
     since it prevents accidental wakes from regular network traffic)
4. Open the **Advanced** tab and set **Wake on Magic Packet** to
   **Enabled**. Disable **Wake on Pattern Match** unless you
   specifically need it.
5. Click **OK** and close Device Manager.

If you don't see the Power Management tab, your adapter driver may
be hiding it. Update the driver from the manufacturer's site (Intel,
Realtek, Killer) and try again.

## Step 3: Find the PC's MAC address and IP

You'll need both to send the WoL packet:

- **MAC address:** in Windows, open a Command Prompt and run
  `ipconfig /all`. Look for **Physical Address** under your Ethernet
  adapter (format: `AA-BB-CC-DD-EE-FF`).
- **Local IP:** same `ipconfig /all` output, look for **IPv4
  Address** under your Ethernet adapter (usually `192.168.x.y`).

## Step 4: Send the WoL packet from the Raspberry Pi

On a Raspberry Pi (or any Linux box on the same LAN), install one of
these tools:

- **`wakeonlan`** is a simple Perl script:

  ```bash
  sudo apt install wakeonlan
  wakeonlan AA:BB:CC:DD:EE:FF
  ```

- **`etherwake`** is a lower-level alternative:

  ```bash
  sudo apt install etherwake
  sudo etherwake AA:BB:CC:DD:EE:FF
  ```

Either command broadcasts a magic packet to the LAN. If everything
is configured correctly (BIOS enabled, Windows settings enabled,
Ethernet cable connected, PC in soft-off state), the PC will power
on within a few seconds.

## Step 5 (optional): Make the Pi reachable from outside your LAN

If you want to wake the PC from outside your home network, install
Tailscale on the Pi and on the device you'll be waking from (phone,
laptop, etc.). Both devices get a stable private IP (e.g.
`100.x.y.z`) and can talk to each other directly, no port forwarding
needed.

1. On the Pi:

   ```bash
   curl -fsSL https://tailscale.com/install.sh | sh
   sudo tailscale up
   ```

2. Install Tailscale on the phone or laptop you want to wake from:
   https://tailscale.com/download/.

3. From the phone or laptop, SSH into the Pi using its Tailscale IP:

   ```bash
   ssh pi@100.x.y.z
   ```

4. Run `wakeonlan` from the Pi shell (use the PC's MAC address from
   Step 3).

The Tailscale private network keeps the WoL packet sender reachable
from anywhere with internet access, even behind CGNAT.

## Troubleshooting

- **PC doesn't wake at all.** Double-check the BIOS setting first,
  then verify the Ethernet link LED on the PC's NIC stays lit when
  the PC is off. Some PCs disable NIC power in soft-off unless ErP
  is disabled in BIOS.
- **WoL only works over Ethernet.** Confirm the PC is plugged into
  the router with a cable, not Wi-Fi.
- **Magic packet arrives but PC stays off.** Check that the Wake on
  Magic Packet setting is Enabled in Device Manager, not just Wake
  on Link Change or similar.
- **Tailscale peer not visible.** Run `tailscale status` on both
  devices and confirm they're signed into the same Tailscale
  account.

## When to come back to the main project

If any of the above steps don't work for your setup, or you decide
you want a relay-based build that works on any desktop regardless
of NIC or BIOS support, come back to the main README and follow the
ESP32 path:

- `README.md`, Highlights block, "Which path is right for you"
- `README.md`, "Already Have a Raspberry Pi?" section
