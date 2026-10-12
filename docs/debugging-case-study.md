# 🔍 Case Study: The Rover That Drove Itself

**Problem:** Airaavat-V2 ran its motor nonstop in one direction, and the steering servo twitched on its own, as soon as the transmitter was switched on.
**Duration:** 4–7 October 2026
**Outcome:** Root cause isolated and fixed in firmware. First wireless ground trial on 7 October 2026.

---

## 1. Symptoms

| Condition | Behaviour |
| --- | --- |
| Receiver on, transmitter off | Calm; servo centres once at power-up |
| Receiver on, transmitter on | Motor runs in one direction; servo moves erratically |

## 2. Method: isolate one variable at a time

Rather than changing several things at once, each layer of the system was tested on its own with the rest held constant.

| # | Test | What it isolated | Result |
| --- | --- | --- | --- |
| 1 | IBT-2 inputs tied to GND, motor battery connected | Motor driver | ✅ Motor stays still: driver healthy |
| 2 | Receiver diagnostic sketch, motor pins held LOW | Receiver Arduino + nRF24 | ✅ Chip detected, loop running |
| 3 | Same sketch, motor battery connected | Power-on behaviour | ✅ Motor still: wiring correct |
| 4 | Main code, TX and RX Serial Monitors | Data being sent | ❌ TX sending `0, 0` constantly |
| 5 | Analog-pin scan on TX (A0–A5) | Joystick hardware | ✅ Joysticks reading correctly |

**Finding 1: a firmware bug.** The TX read the joysticks into new local variables (`int throttle = analogRead(...)`) instead of the packet struct (`data.throttle = analogRead(...)`). The packet stayed at 0/0, which the rover correctly obeyed as full reverse and full left lock. Fixed, and wheels-off tests passed.

The instability later returned, so the investigation continued:

| # | Test | What it isolated | Result |
| --- | --- | --- | --- |
| 6 | TX pack voltage | Transmitter supply | ⚠️ 10.06 V (nearly flat): replaced |
| 7 | TX diagnostic sketch | Joysticks + TX radio | ✅ Steady values, chip connected |
| 8 | RX diagnostic sketch | Receiver radio | ✅ Chip connected, listening |
| 9 | Both diagnostics together (packet counter + ID marker) | End-to-end link | ✅ Every packet acknowledged, 0 bad, 0 missed |
| 10 | Diagnostics on the real battery packs | Power under idle load | ✅ Still clean |
| 11 | Main firmware reviewed line by line | Logic | ✅ Correct: at-rest data gives zero speed and centred steering |

## 3. The clue

The diagnostic sketches **passed** and the main firmware **failed**. The only radio setting that differed:

| | Diagnostics (passed) | Main firmware (failed) |
| --- | --- | --- |
| Power level | `RF24_PA_LOW` | `RF24_PA_HIGH` |

With auto-acknowledgement on, the receiver **transmits** an acknowledgement for every packet it gets: about 50 times a second. On a PA/LNA module at high power, each acknowledgement is a sharp current burst drawn through the UNO's small 3.3 V regulator. That fits every symptom:
- **TX off:** no packets arrive, so the receiver sends nothing, draws no bursts, and stays calm.
- **TX on:** about 50 current bursts per second disturb the receiver's supply. The Arduino glitches or resets, its motor pins float briefly, and the IBT-2 reads that as "go".

## 4. Fix

| Change | Purpose |
| --- | --- |
| `setAutoAck(false)` on both sides | The receiver never transmits, so the bursts are gone |
| ID marker (`0xA1B2`) and range check on every packet | Garbage or foreign data can never move the rover |
| Fixed-size packet types (`uint16_t`, `int16_t`) | Same packet layout on UNO and ESP32 |
| Servo written only when the angle changes | Less servo chatter |
| Non-blocking debug output at 115200 baud | The old 9600-baud prints were slowing the TX loop |

**Result:** the rover drove cleanly on the first ground trial after the change.

## 5. Honest note on certainty
The fix worked, and the behaviour matches the acknowledgement-burst explanation. The supply dips themselves were **not** captured on an oscilloscope, so this is the most probable root cause, confirmed by the result rather than by direct measurement.

## 6. Lessons
1. **Isolate variables.** Each test removed one suspect. Without that, a "random" fault could have been anything.
2. **A known-good baseline is powerful.** The diagnostic sketch that passed made the difference with the main firmware stand out.
3. **The code can be right and the system still wrong.** Power integrity is part of embedded design.
4. **Design for failure.** Packet validation and the failsafe now protect against whole classes of faults.

## 7. Follow-up hardware hardening (planned)
- 10 kΩ pull-downs on RPWM and LPWM
- 0.1 µF suppression capacitors on the RS-775
- nRF24 adapter board or a dedicated 3.3 V regulator, so `PA_HIGH` can be used safely for longer range
- Separate buck converters for the Arduino and the servo, with a star ground
