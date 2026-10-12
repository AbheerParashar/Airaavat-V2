# 🧭 Design Decisions: Airaavat-V2

Each decision below records **what was chosen, why, and what it costs**.

---

## 1. Single-motor chain drive (vs hub motors or multiple motors)

**Chosen:** one RS-775 driving the rear axle through a 2-stage jackshaft chain reduction (20:1).

**Why**
- **Simplicity:** one motor, one driver, one control loop to tune.
- **Maintainability:** chains and sprockets are cheap, widely available and repairable with basic tools.
- **Tunable:** changing one sprocket changes the speed/torque trade-off.
- **Proven at V1:** a 13:1 composite gear drive was also tested. It had lower current draw, but the mild-steel chain was chosen for durability.

**Trade-offs**
- **Single point of failure:** if the motor or driver fails, the rover cannot move. A multi-motor rover can sometimes keep going.
- **An open chain is exposed** to dust and mud on rough ground. It needs lubricating, tensioning and alignment checks.
- **Transmission losses** (about 5% per chain stage, assumed).

## 2. Rear-wheel drive with a solid axle

**Chosen:** both rear wheels on one driven shaft.

**Why:** the simplest way to drive two wheels from one motor.

**Trade-offs**
- **No differential:** in turns, the inner and outer wheels are forced to the same speed, so one wheel scrubs. On smooth floors with high torque, this shows up as rear-wheel drift (observed on 7 October 2026).
- **Traction** depends only on the rear wheels.

**Planned improvement:** a differential, or a one-way-bearing arrangement on one rear wheel.

## 3. Ackermann steering (vs skid-steer)

**Chosen:** servo-driven Ackermann geometry on the front wheels.

**Why**
- **Low tyre scrub and low turning power:** the wheels roll through turns instead of skidding.
- **Steering is independent of propulsion:** a steering fault doesn't disable the drive, and a drive fault doesn't disable steering.
- **Vehicle-like handling** at speed.

**Trade-offs**
- **Larger turning radius** than skid-steer, which can turn on the spot.
- The servo and linkage are extra parts to maintain. The cycle-spoke linkage is planned to be upgraded to threaded rod with rod-end ball joints.

## 4. Plywood chassis (for now)

**Chosen:** 13 mm plywood, bolt-through mounting at load points.

**Why:** available at no cost; much stronger and better at holding screws than V1's MDF; fast to modify.

**Trade-offs:** creeps under sustained bolt loads, cracks at cut corners, sensitive to moisture. **A metal chassis is planned for Phase 2.**

## 5. One-way radio link with failsafe

**Chosen:** auto-acknowledgement off, packet validation, 500 ms receiver failsafe.

**Why:** removes the receiver's transmit current bursts (see [`debugging-case-study.md`](debugging-case-study.md)). Most RC links are one-way, with safety handled by the receiver.

**Trade-off:** the transmitter no longer knows whether packets arrive. Planned telemetry (Phase 2) will bring back a return channel, designed with proper power supply.

---

## Concept under study: on-demand front-axle drive (planned, V2.5/V3)

**Idea:** keep the RWD chain drive and Ackermann steering, and add a **geared motor on each front steering knuckle** that pivots with the wheel. The front axle engages only when needed: rough terrain, rear-wheel slip, or as a backup.

**Why it is interesting**
- Similar in philosophy to **on-demand AWD**: rear-biased drive, with front drive added when needed.
- Independently controlled front motors allow **torque vectoring**: driving the left and right front wheels at different speeds, based on steering angle, to help the rover turn and grip.
- Avoids the CV/universal joints otherwise needed to drive steered wheels from a central motor.

**Challenges to solve**
1. Matching front wheel speed to the rear drivetrain (~450–500 wheel RPM, calculated).
2. Heavier steered mass, so a stronger servo or linkage is needed.
3. Motor cables flexing through the steering range.
4. A second motor driver and firmware for steering-angle-based speed splitting.
5. A rigid (metal) chassis to take the knuckle loads.
