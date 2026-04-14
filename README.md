# LuminaCare: The Adaptive Eye-Health Companion

![LuminaCare Banner](ProjectBanner.png)

[![Platform](https://img.shields.io/badge/Platform-ESP8266-0A7EA4?style=for-the-badge)](https://www.espressif.com/en/products/socs/esp8266)
[![Language](https://img.shields.io/badge/Firmware-Arduino%20C++-00979D?style=for-the-badge)](https://www.arduino.cc/)
[![Mode](https://img.shields.io/badge/Connectivity-Local%20WiFi%20AP-2E7D32?style=for-the-badge)](#installation)
[![Focus](https://img.shields.io/badge/Use%20Case-Student%20Wellness-F57C00?style=for-the-badge)](#societal-impact)

An empathetic, embedded-first wellness companion that helps students protect eye health and sustain focus through adaptive lighting, guided breaks, and ambient-aware feedback.

## The Problem Statement

Across classrooms, libraries, and hostels, students now spend long hours on laptops and phones under inconsistent lighting conditions. This shift has accelerated two linked challenges:

- Digital eye strain from prolonged near-work and suboptimal brightness contrast.
- Rising myopia risk associated with continuous close-focus habits and inadequate visual breaks.

LuminaCare is built as a practical intervention: a low-cost, real-time system that nudges healthier study behavior without disrupting flow.

## The Solution: Hardware-Software Synergy

LuminaCare combines sensing, stateful automation, and physical feedback in one loop:

- NodeMCU (ESP8266) runs a finite state machine with three modes: IDLE, STUDY, BREAK.
- LDR input is filtered and mapped to adaptive lamp brightness to reduce harsh contrast.
- A servo performs a physical nudge by raising a visible rest/health flag at break time.
- RGB status cues communicate focus state instantly.
- A local WiFi dashboard lets the user switch timing profiles from any phone.

### How the 20-20-20 principle is implemented

The control logic is designed around timed focus-to-rest transitions:

- Study session timer starts on button press.
- At session completion, the system transitions to BREAK, triggers buzzer alerts, turns off the study lamp, and raises a servo flag.
- After break timeout, the system resets to IDLE and is ready for the next cycle.

Current firmware includes a rapid demo profile and a 20-minute study profile via dashboard routes, making it easy to test and then tune toward a full 20-20-20 cycle.

## Key Features

- 🌗 Adaptive Ambient Compensation: LDR-driven lamp dimming automatically balances perceived brightness.
- 🧭 Physical Posture/Rest Nudges: Servo-based mechanical cue makes break transitions hard to ignore.
- 🔴🟢🔵 Flow State RGB Signaling: Clear color semantics for focus, transition, and recovery states.
- 📶 Local IoT Dashboard: Phone-accessible AP dashboard for mode visibility and session preset changes.

## Gallery

| Hardware Layout | Dashboard Interface | Live Demo |
|---|---|---|
| ![Hardware Layout](Hardware_Layout.jpg) | ![Dashboard Interface](Dashboard_Interface.jpg) | ![Project Demo](Project_Demo.gif) |

## Tech Stack

### Hardware

- NodeMCU ESP8266
- LDR (analog light sensing on A0)
- Servo motor (rest flag actuation)
- RGB LEDs (status indicators)
- White LED lamp (study illumination)
- Buzzer (audible notifications)
- Push button (single-click session control)

### Software and Firmware

- Arduino C++ on ESP8266
- Deterministic state machine: IDLE -> STUDY -> BREAK
- Exponential moving average filtering for light smoothing
- Analog brightness mapping and constraint logic
- WiFi SoftAP mode with local HTTP dashboard (ESP8266WebServer)

## Installation

### 1) Arduino IDE Setup

1. Install Arduino IDE (or Arduino CLI if preferred).
2. Add ESP8266 board support URL in Preferences:
	- http://arduino.esp8266.com/stable/package_esp8266com_index.json
3. Open Boards Manager and install ESP8266 by ESP8266 Community.
4. Select board: NodeMCU 1.0 (ESP-12E Module).
5. Install required libraries:
	- ESP8266WiFi
	- ESP8266WebServer
	- Servo

### 2) Flash the Firmware

1. Open main.ino.
2. Connect NodeMCU over USB.
3. Select the correct COM/serial port.
4. Upload the sketch.
5. Open Serial Monitor at 115200 baud to verify startup log.

### 3) Connect to LuminaCare Dashboard

1. From phone/laptop WiFi, connect to:
	- SSID: Smart-Study-Lamp
	- Password: password123
2. Open browser and navigate to:
	- http://192.168.4.1
3. Use dashboard buttons to switch between quick demo and long study profile.

## System Behavior Snapshot

- Button press in IDLE starts STUDY mode.
- Red LED breathes during deep-focus period.
- Lamp brightness adapts continuously to ambient light.
- Session timeout triggers BREAK mode:
  - Green status LED
  - Buzzer alert pattern
  - Lamp off for visual rest
  - Servo flag raised as physical reminder
- Break timeout returns to IDLE.

## Societal Impact

LuminaCare demonstrates how affordable embedded systems can translate public-health guidelines into daily behavior change. By combining human-centered nudges with adaptive sensing, this approach can scale across schools, libraries, and low-resource learning environments, helping students build healthier digital habits and reduce preventable eye strain at population level.

## Future Directions

- Calibrated 20-minute study + 20-second distance-focus cycle as default profile.
- Dashboard controls for fully custom study/break durations.
- Data logging for adherence and wellness analytics.
- Multi-user classroom deployment with centralized reporting.