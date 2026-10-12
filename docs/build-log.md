# 📓 Build Log: Airaavat-V2

Newest entries at the bottom. Each entry records what was done, what was found, and what changed.

---

### March–August 2026: Airaavat-V1
- Designed and built the first UGV: RS-775 chain drivetrain (20:1), DS-3225 Ackermann steering, MDF chassis, Arduino UNO + nRF24L01 manual RC control, 4S4P Li-ion rover pack.
- V1 drove under manual RC. Observed speed about 8–12 km/h (estimated).

### 30 September 2026: V2 kickoff
- V2 engineering started with Chandan.
- Decision: build V2's chassis, drivetrain and steering first and get it moving; ESP32, sensors and CAD come after.

### 1 October 2026: chassis fabrication begins
- V2 chassis cut from 13 mm plywood. Plywood chosen over MDF for screw-holding strength and better resistance to vibration and moisture.

### 2 October 2026: drivetrain runs on the bench
- 2-stage jackshaft chain drive assembled on pillow-block bearings and run under power.
- The angle grinder failed during fabrication; remaining cuts done by hand and at a hardware store.

### 3 October 2026: rover assembled, firmware rewrite begins
- Wheels and the Ackermann steering assembly fitted; chassis sections joined.
- Existing V1 firmware found corrupted. Decision: write new firmware instead of reusing old code.
- First sketch: joystick reading to Serial Monitor. Calibration: throttle centre 521, steering centre 508, maximum 1020.

### 4 October 2026: receiver firmware, first wheels-off success
- Full TX/RX firmware written: radio packets, joystick deadband, `map()` scaling and a 500 ms signal-loss failsafe.
- **Fault 1:** wires broken during transport. TX and RX rewired.
- **Fault 2:** the motor ran nonstop and the servo locked to one side. Isolation tests showed the IBT-2, receiver Arduino, servo and radio were all healthy. Root cause: the TX code stored joystick readings in new local variables instead of the packet struct, so it always sent 0/0 (full reverse, full left). Fixed.
- Chandan built a 3S3P Li-ion transmitter pack with a 3S BMS.
- **Wheels-off test successful.** Servo integrated into the Ackermann linkage.
- Added a squared throttle curve, a soft-start ramp and full 255 PWM.

### 5 October 2026: instability returns
- Erratic behaviour again: motor running away, servo moving on its own.
- The transmitter pack measured 10.06 V (≈3.35 V per cell), nearly flat. Identified as one contributing factor.

### 6 October 2026: systematic isolation
- New KY-023 joysticks fitted and fixed permanently; new 3S pack fitted (11.1 V).
- Dedicated diagnostic firmware written for TX and RX (packet counter, ID marker, chip detection).
- Results: joysticks steady, radio link healthy, zero bad or missed packets, on USB power.

### 7 October 2026: root cause found, first ground trials ✅
- Diagnostic sketches run on the real battery packs: link still healthy. So the packs could power the electronics cleanly when the actuators were idle.
- Key difference found: diagnostics used low radio power; the main firmware used `RF24_PA_HIGH` with auto-acknowledgement, so the receiver transmitted current bursts about 50 times a second.
- **Fix:** one-way link (auto-ack disabled), packet validation (ID marker and range check), fixed-size packet types, non-blocking debug output.
- **First wireless ground trial successful (morning).** The rover drove, turned and returned under its own power.
- **Second trial (evening)** with the 4S pack at 16.4 V: strong low-end torque, rear-wheel drift on a smooth floor, estimated 15–20 km/h (measurement pending).
- Full write-up: [`debugging-case-study.md`](debugging-case-study.md)

### 8 October 2026: concept work
- Public feedback suggested 4WD. Concept developed: **on-demand front-axle drive** with geared motors on the steering knuckles and steering-angle-based torque vectoring, inspired by on-demand AWD systems. Status: **planned** (V2.5/V3, after a metal chassis). See [`design-decisions.md`](design-decisions.md).

### 9 October 2026: documentation and next stage
- Repository documentation created.
- Sensor fusion and telemetry work begins with owned components (ESP32, MPU-6050, HMC5883L, NEO-6M, VL53L0X, LiDAR, HC-SR04).

---

_Next entries: see [`../logbook/`](../logbook/)._
