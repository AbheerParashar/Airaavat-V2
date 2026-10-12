# 🧾 Bill of Materials: Airaavat-V2 (as-built, October 2026)

Quantities are for the complete rover and transmitter as tested on 7 October 2026.

## Rover: mechanical

| Part | Qty | Notes |
| --- | --- | --- |
| RS-775 brushed DC motor | 1 | Main propulsion |
| Sprocket, 12T | 1 | Motor pinion (stage 1 driver) |
| Sprocket, 60T | 1 | Jackshaft (stage 1 driven) |
| Sprocket, 14T | 1 | Jackshaft (stage 2 driver) |
| Sprocket, 56T | 1 | Rear axle (stage 2 driven) |
| Roller chain | 2 runs | One per reduction stage |
| Steel shafts | 2 | Jackshaft and rear axle |
| Pillow-block bearings | 4 | Two per shaft |
| Wheels | 4 | 20 cm diameter (measured) |
| Plywood, 13 mm | 1 sheet | Chassis |
| Steering linkage (tie rods, stub axles) | 1 set | Ackermann geometry |
| Nuts, bolts, washers | — | Bolt-through mounting at load points |

## Rover: electrical

| Part | Qty | Notes |
| --- | --- | --- |
| Arduino UNO | 1 | Receiver controller |
| nRF24L01+PA/LNA module | 1 | 2.4 GHz radio |
| Electrolytic capacitor, 10–100 µF | 1+ | Across nRF24 VCC/GND |
| IBT-2 (BTS7960) motor driver | 1 | H-bridge for the RS-775 |
| DS-3225 servo | 1 | Steering |
| LM2596 buck converter | 3 | Logic, servo and auxiliary rails |
| Li-ion pack, 4S4P | 1 | Rover propulsion and logic |
| 4S BMS | 1 | Pack protection |

## Transmitter

| Part | Qty | Notes |
| --- | --- | --- |
| Arduino UNO | 1 | Transmitter controller |
| nRF24L01+PA/LNA module | 1 | 2.4 GHz radio |
| Electrolytic capacitor, 10–100 µF | 1+ | Across nRF24 VCC/GND |
| KY-023 joystick module | 2 | Throttle and steering |
| LM2596 buck converter | 1–2 | Logic rail |
| Li-ion pack, 3S3P | 1 | Built by Chandan |
| 3S BMS | 1 | Pack protection |

## Known issues with current parts
- The **Li-ion cells in both packs are low grade** and near end of life. They sag under load. Replacement with genuine cells is the top item in Phase 1 funding ([`../docs/roadmap.md`](../docs/roadmap.md)).
- Several modules have been through heavy testing since V1 and are worn. Spares are planned.
