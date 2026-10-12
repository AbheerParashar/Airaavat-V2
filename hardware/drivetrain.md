# ⚙️ Drivetrain: Airaavat-V2

## Layout (as-built)

```
RS-775 ──12T──chain──60T──[jackshaft]──14T──chain──56T──[rear axle]──► both rear wheels
         stage 1: 5:1                    stage 2: 4:1
```

- Rear-wheel drive, single motor
- Solid rear axle (no differential)
- Shafts carried on pillow-block bearings bolted through a 13 mm plywood chassis

## Reduction ratio (calculated)

```
Stage 1 = 60 / 12 = 5
Stage 2 = 56 / 14 = 4
Total   = 5 × 4  = 20 : 1
```

The wheels turn 20 times slower than the motor, and wheel torque is about 20 times the motor torque (minus chain losses).

## Wheel torque (formula)

```
T_wheel = T_motor × 20 × η
```
η is the drivetrain efficiency. A typical roller-chain stage is roughly 0.95 efficient, so two stages give η ≈ 0.9 (**assumed**, not measured).

## Top speed (calculated)

```
Wheel RPM  = Motor RPM / 20
Speed (m/s) = π × D × Wheel RPM / 60
Speed (km/h) = Speed (m/s) × 3.6
```

The RS-775 no-load speed depends on the variant and the voltage. At about 16 V, **9,000–10,000 RPM is assumed** here. Measure or confirm it from the motor's datasheet.

| Wheel diameter D | Motor 9,000 RPM | Motor 10,000 RPM |
| --- | --- | --- |
| 15 cm | 12.7 km/h | 14.1 km/h |
| 18 cm | 15.3 km/h | 17.0 km/h |
| 20 cm | 17.0 km/h | 18.8 km/h |

These are **no-load upper limits**. Under load the motor slows down, so the real top speed is lower.

### Airaavat-V2: wheel diameter D = 20 cm (measured)

```
Wheel RPM    = 9,000–10,000 / 20 = 450–500 RPM
Speed (m/s)  = π × 0.20 × 450…500 / 60 = 4.7–5.2 m/s
Speed (km/h) ≈ 17.0–18.8 km/h  (calculated, no-load upper limit)
```

The estimated 15–20 km/h from the second V2 trial is **consistent** with this calculation. A timed 10 m run will turn it into a measured figure.

**Compare with the observed speeds:**

| Trial | Pack voltage | Observed speed | Label |
| --- | --- | --- | --- |
| V1 trials | ~16.8 V nominal | ~8–12 km/h | Estimated |
| V2 trial 2, 7 Oct 2026 | 16.4 V | ~15–20 km/h, with rear-wheel drift | Estimated |
| V2 measured run | _fill in_ | _fill in_ | Measured |

### How to measure speed
Marked **10 m** on the ground, filmed a full-throttle run through it, and read the time from the video:

```
Speed (km/h) = 36 / time (s)
```
For example, 10 m in 2.0 s gives 18 km/h.

## Why a chain drive?
See [`../docs/design-decisions.md`](../docs/design-decisions.md).

## Maintenance checklist
- [ ] Chain tension: about 1 cm of slack at the midpoint of each run
- [ ] Sprocket alignment: both sprockets of each stage in one plane
- [ ] Lubricate the chains after dusty or wet runs
- [ ] Check the bearing-block bolts and motor mount after high-speed runs (plywood loosens under vibration)
- [ ] Check the motor and IBT-2 temperature after sustained full throttle
