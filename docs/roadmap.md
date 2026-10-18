# 🗺️ Roadmap: Project Airaavat

## Where things stand (October 2026)
- ✅ **Airaavat-V2 drives wirelessly** under its own power with custom firmware.
- 🔧 **Sensor fusion and telemetry** started, using components already owned.
- ⚠️ **Main bottleneck:** power. Both Li-ion packs use low-grade cells near end of life.

---

## Stage A: sensor fusion and telemetry (in progress, already resourced)

Components owned: ESP32, ESP32-CAM, MPU-6050 (IMU), HMC5883L (magnetometer), NEO-6M (GPS), VL53L0X (ToF), LiDAR module, HC-SR04, Micro-SD module, LM35.

| Goal | Pass criterion |
| --- | --- |
| ESP32 controls drive and steering | Same RC feel as the UNO receiver |
| Heading | IMU + magnetometer give a stable, calibrated heading |
| Position | GPS fix logged outdoors |
| Obstacle sensing | Distance readings and an obstacle-ahead flag |
| Telemetry | Battery voltage, heading, position and obstacle data streamed to the operator or logged to SD |
| Video | ESP32-CAM stream from the rover |

## Phase 1 funding: power, tools and spares

What the rover needs to run reliably and be built properly. Funded in three milestones, one after another. **Phase 1 total: ₹32,400.**

📄 **Campaign page (full itemised budget and UPI):** [PUBLIC PAGE LINK]

### Milestone 1: build the packs the right way (₹15,000) · funding now

| Item | Purpose |
| --- | --- |
| 34× second-life LG/BAK 21700 NMC cells (~4,200–4,500 mAh, graded and IR-matched, LithiumworkX) + pure nickel strip | New 4S4P rover pack, 3S3P transmitter pack, 3S3P backup pack |
| AWithZ UK1 battery spot welder | Safe pack building (soldering damages cells) |
| 2× 4S 40A and 3× 3S 40A BMS with balancing | Pack protection, with spares |
| 4× ZB2L3 capacity tester with load resistors | Test every cell before it goes into a pack |
| 21700 barley-paper rings, cell spacers, goggles and gloves | Insulation, rigidity, safety |

### Milestone 2: charge, protect and measure (₹8,200)

| Item | Purpose |
| --- | --- |
| 4S Li-ion charger, 16.8 V 5 A | Proper charging of the ~17 Ah rover pack |
| 150 A watt meter and power analyser | Measure the RS-775's real current draw |
| Amass XT90-S anti-spark connectors, ATC fuse holder and fuses | Main disconnect and main fuse |
| Yihua 926LED-III soldering station, chisel tip, solder, flux | Reliable wiring |
| Wiring kit (12 AWG / 22 AWG silicone wire, heat-shrink, ferrules, XT60s) | Rewire the rover properly |

### Milestone 3: keep the build moving (₹9,200)

| Item | Purpose |
| --- | --- |
| 3S Li-ion charger, 12.6 V 5 A | Charging the transmitter and backup packs |
| Spare TX/RX set: 2× Arduino UNO, 4× nRF24L01+PA/LNA, 4× KY-023, 4× LM2596 | Modules worn out from testing |
| Bosch GWS 600 angle grinder and discs | Cutting brackets, axles and stock |
| 13 mm plywood, MS angle, aluminium flat bar, fasteners | Modular, bolted chassis for the next build |

Every contribution and every bill will be recorded in a ledger in this repository. At the supplier's request, the per-cell price is not published; the cells and nickel strip appear as one line.

## Phase 2 funding: structure and mobility

| Item | Purpose |
| --- | --- |
| Metal chassis material (mild steel or aluminium) and fasteners | Rigid, durable frame for sensors and a UAV deck |
| Ackermann upgrade: threaded rod, rod-end ball joints | Stiffer, adjustable steering linkage |
| Rear differential or one-way bearing | Removes rear-wheel scrub in turns |
| Front-axle drive motors and driver (concept) | On-demand 4WD with torque vectoring |

## Beyond: GarudAshwa
A proven V2 UGV carrying a **UAV launch deck** and a quadrotor for aerial reconnaissance, leading to a **mobile launch and control vehicle** where the UGV launches, controls and recovers the UAV.

```
V2 (drives) ✅ → V2 + sensors 🔧 → metal chassis + 4WD concept 🗺️ → GarudAshwa-V1 🗺️ → autonomous MLCV 🗺️
```
