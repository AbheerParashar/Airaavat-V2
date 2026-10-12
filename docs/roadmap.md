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

What the rover needs to run reliably and be built properly.

| Item | Purpose |
| --- | --- |
| 34× genuine Samsung 18650 cells (2500 mAh+) | New 4S4P rover pack, 3S3P transmitter pack, 3S3P backup pack |
| 1× 4S and 2× 3S BMS (20–40 A) | Pack protection |
| 4S and 3S Li-ion chargers (5 A) | Proper charging |
| Li-ion spot welder | Safe pack building (soldering damages cells) |
| Temperature-controlled soldering station and gear | Reliable wiring |
| Spare TX/RX set: Arduino UNOs, nRF24L01+PA/LNA, KY-023 joysticks, LM2596 bucks | Modules worn out from testing |

_Itemised costs to be added in the campaign budget._

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
