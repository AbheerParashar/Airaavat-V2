# 💻 Firmware (`src/`)

| Folder | Board | Purpose |
| --- | --- | --- |
| [`airaavat_tx/`](airaavat_tx/) | Arduino UNO | Transmitter: reads joysticks, sends packets at ~50 Hz |
| [`airaavat_rx/`](airaavat_rx/) | Arduino UNO | Receiver on the rover: drives motor and steering |
| [`diagnostics/`](diagnostics/) | Arduino UNO | Isolation-test sketches for the radio link |

## Requirements
- Arduino IDE
- **RF24** library by TMRh20 (Library Manager)
- **Servo** library (built in)

## Packet format (main firmware)

| Field | Type | Meaning |
| --- | --- | --- |
| `magic` | `uint16_t` | ID marker `0xA1B2`; packets without it are rejected |
| `throttle` | `int16_t` | Raw joystick reading, 0–1023 |
| `steering` | `int16_t` | Raw joystick reading, 0–1023 |

Fixed-size types (`uint16_t`, `int16_t`) are used so the packet is the same size on an Arduino UNO and an ESP32 (where a plain `int` is 32-bit). This prepares the link for the ESP32 migration.

## Radio settings (both sides must match)
- Address: `"AIRV2"`
- Data rate: 250 kbps
- Auto-acknowledgement: **off** (one-way link; see [`../docs/debugging-case-study.md`](../docs/debugging-case-study.md))

## Calibration
The receiver constants (`THROTTLE_CENTRE`, `STEERING_CENTRE`, `JOY_MAX`, `SERVO_LEFT/CENTRE/RIGHT`) are measured for this build. Re-measure them after rewiring or mechanical changes:
1. Upload the TX firmware with `DEBUG = true` and note the joystick values at rest and at full travel.
2. With the front wheels lifted, find the servo angles for straight ahead and for full lock each way without the linkage binding.

## Debug mode
Set `DEBUG = true` for Serial output at **115200 baud**, 5 lines per second. Set `DEBUG = false` for normal driving.
