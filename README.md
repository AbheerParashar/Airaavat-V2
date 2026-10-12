# 🐘 Project Airaavat — V2

**A hardware-first Unmanned Ground Vehicle (UGV), rebuilt from Airaavat-V1. Designed, fabricated, wired and programmed by two diploma students at D.N. Polytechnic, Meerut, with no lab and no institutional funding.**

> **7 October 2026:** Airaavat-V2 completed its **first wireless ground trial**, driving under its own power through a radio link and firmware built during the week of 1–7 October.

📹 **Ground-trial video:** [First wireless trial, 7 Oct 2026](https://drive.google.com/file/d/1QRCBFcqSwdifOWkJ_7g2vigKBfFnSC9H/view) · [Instagram @project_airaavat](https://instagram.com/project_airaavat)
<!-- Replace the line above with a direct link to the ground-trial reel or LinkedIn post. -->

⬅️ **Previous version:** [Airaavat-V1](https://github.com/AbheerParashar/Airaavat-V1)

---

## 📌 Status at a glance

| Item | Status |
| --- | --- |
| Airaavat-V1 (manual RC UGV) | ✅ As-built, March–August 2026 · [repo](https://github.com/AbheerParashar/Airaavat-V1) |
| V2 chassis, drivetrain, steering | ✅ As-built, 1–4 October 2026 |
| V2 TX/RX firmware (deadband, ramp, failsafe) | ✅ As-built and ground-tested, 7 October 2026 |
| V2 sensor fusion and telemetry (ESP32) | 🔧 In development, started October 2026 |
| GarudAshwa (UGV + UAV launch deck) | 🗺️ Planned |

### ⚠️ How to read this repository

Every technical claim is labelled:
- **As-built:** exists and has been tested.
- **Calculated:** derived from design values, not yet measured.
- **Estimated:** observed roughly, not yet measured precisely.
- **Planned:** a development goal, not a current capability.

---

## 🆕 What V2 adds over V1

| Area | Airaavat-V1 | Airaavat-V2 |
| --- | --- | --- |
| Chassis | MDF | 13 mm plywood, bolt-through mounting at load points |
| Firmware | As-built record, adapted from existing code | Rewritten and debugged line by line during the build |
| Signal-loss safety | No RF-loss failsafe | **500 ms failsafe:** motor cut, steering re-centred |
| Packet integrity | Unvalidated | ID marker and range check on every packet |
| Throttle response | Linear | Deadband, squared curve, soft-start ramp |
| Radio link | Two-way, with acknowledgements | One-way validated link (see the case study) |
| Debugging method | Trial and error | Systematic isolation with dedicated diagnostic firmware |

---

## ⚙️ System overview

```
 [ Transmitter ]                                   [ Rover ]
 2× KY-023 joysticks                               Arduino UNO (receiver)
        │                                                 │
 Arduino UNO ──► nRF24L01+PA/LNA ))) 2.4 GHz ))) nRF24L01+PA/LNA
 3S Li-ion + buck                                         │
                                       ┌──────────────────┼───────────────────┐
                                       ▼                  ▼                   ▼
                                 IBT-2 (BTS7960)    DS-3225 servo        Failsafe logic
                                       │                  │
                                 RS-775 motor     Ackermann steering
                                       │
                         2-stage chain drive, 20:1
                                       │
                                  Rear axle (RWD)
```

| Subsystem | Specification | Label |
| --- | --- | --- |
| Propulsion | RS-775 brushed DC motor | As-built |
| Reduction | 2-stage jackshaft sprocket-chain: 12T→60T (5:1), 14T→56T (4:1), **20:1 total** | As-built |
| Drive layout | Rear-wheel drive, solid rear axle (no differential) | As-built |
| Wheels | 4 × 20 cm diameter | As-built |
| Steering | DS-3225 servo, Ackermann geometry | As-built |
| Chassis | 13 mm plywood, pillow-block bearings | As-built |
| Motor driver | IBT-2 (BTS7960) H-bridge | As-built |
| Control link | Arduino UNO TX/RX, nRF24L01+PA/LNA, 2.4 GHz, 250 kbps, ~50 Hz | As-built |
| Rover power | 4S4P Li-ion pack, 4S BMS, LM2596 buck converters | As-built |
| Transmitter power | 3S3P Li-ion pack with 3S BMS (built by Chandan) | As-built |
| Top speed | 17–19 km/h no-load upper limit (20 cm wheels, 9,000–10,000 RPM motor assumed) | **Calculated** |
| Top speed | ~15–20 km/h on the second trial at 16.4 V | **Estimated** (timed run pending) |

Details: [`hardware/`](hardware/) · [`docs/design-decisions.md`](docs/design-decisions.md)

---

## 💻 Firmware

Written in Arduino C++, typed and debugged line by line during the V2 build week.

| Feature | What it does |
| --- | --- |
| Joystick deadband (±30) | The rover never creeps when the sticks are at rest |
| Squared throttle curve | Fine control at low speed, full power at full stick |
| Soft-start ramp (~0.5 s, 0→255 PWM) | Protects the chain drive from shock loads and limits inrush current |
| Packet validation | ID marker and range checks; invalid packets are ignored |
| 500 ms signal-loss failsafe | Motor cut instantly and steering re-centred if the link drops |
| One-way link (auto-ack off) | Removes the receiver's transmit current bursts, the root cause of the V2 instability |

Code: [`src/`](src/)

---

## 🔍 Engineering highlight: the debugging case study

For days, the rover ran away in one direction and the steering twitched on its own. Instead of guessing, every layer was tested in isolation with dedicated diagnostic firmware: joysticks, radio link, packet integrity, receiver, motor driver, and power under load.

**Most probable root cause:** current bursts from the receiver's radio acknowledgements disturbing the receiver's supply. **Fix:** a one-way validated link with a receiver-side failsafe.

Full write-up: [`docs/debugging-case-study.md`](docs/debugging-case-study.md)

---

## 🗺️ Roadmap

```
Airaavat-V1 ──► Airaavat-V2 ──► V2 sensor fusion + telemetry ──► GarudAshwa-V1 ──► Autonomous MLCV
 (manual RC)    (wireless RC,     (ESP32, IMU, magnetometer,      (V2 + UAV          (UGV launches,
   ✅           custom firmware)   GPS, ToF/LiDAR)                launch deck)        controls and
                     ✅                  🔧                          🗺️               recovers a UAV) 🗺️
```

Phases and funding: [`docs/roadmap.md`](docs/roadmap.md)

---

## 📁 Repository structure

```
Airaavat-V2/
├── README.md
├── src/
│   ├── airaavat_tx/airaavat_tx.ino      Transmitter firmware
│   ├── airaavat_rx/airaavat_rx.ino      Receiver (rover) firmware
│   └── diagnostics/                     Isolation-test sketches
├── hardware/
│   ├── BOM.md                           Bill of materials
│   ├── wiring.md                        Pin maps and wiring rules
│   └── drivetrain.md                    20:1 reduction and speed calculation
├── docs/
│   ├── build-log.md                     Dated build log
│   ├── debugging-case-study.md          The V2 instability investigation
│   ├── design-decisions.md              Why chain drive, RWD and Ackermann
│   └── roadmap.md                       Phases and milestones
├── logbook/
│   └── logbook-template.md              Daily engineering logbook format
└── media/                               Photos, scans and video links
```

---

## 👥 Team

- **Abheer Parashar**: design, fabrication, electronics, firmware
- **Chandan Gupta**: fabrication, wiring, 3S3P transmitter battery pack

Diploma (1st-Year) in Electrical Engineering, D.N. Polytechnic, Meerut.

<!--
## 🙏 Acknowledgements
Uncomment this section and add mentors and supporters who are happy to be credited publicly.
-->

## 🤝 Connect

· GitHub: [@AbheerParashar](https://github.com/AbheerParashar) · LinkedIn: [@Abheer Parashar](https://www.linkedin.com/in/abheer-parashar-752b9338a/)
· Instagram: [@project_airaavat](https://instagram.com/project_airaavat)


> "Service before self." 🇮🇳
