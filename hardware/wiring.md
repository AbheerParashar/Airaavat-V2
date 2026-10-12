# 🔌 Wiring: Airaavat-V2

## Transmitter (Arduino UNO)

| From | To (UNO) | Notes |
| --- | --- | --- |
| Throttle joystick signal (VRx or VRy) | A0 | Use the axis that matches the push direction |
| Steering joystick signal (VRx or VRy) | A1 | |
| Both joysticks +5V | 5V | Not 3.3 V, not Vin |
| Both joysticks GND | GND | |
| nRF24 CE | D9 | |
| nRF24 CSN | D10 | |
| nRF24 SCK | D13 | Fixed SPI pin |
| nRF24 MOSI | D11 | Fixed SPI pin |
| nRF24 MISO | D12 | Fixed SPI pin |
| nRF24 VCC | 3.3V | **Never 5 V** (or 5 V into an adapter board with its own regulator) |
| nRF24 GND | GND | |
| nRF24 IRQ | not connected | |
| 3S pack → LM2596 → UNO | 5V pin or barrel jack | Never connect 12.6 V directly to the 5V pin |

## Receiver / rover (Arduino UNO)

| From | To (UNO) | Notes |
| --- | --- | --- |
| nRF24 (all pins) | Same as transmitter | CE D9, CSN D10, SCK D13, MOSI D11, MISO D12, VCC 3.3V |
| Steering servo signal | D3 | Servo + and − from its **own** buck converter |
| IBT-2 RPWM | D6 | Forward |
| IBT-2 LPWM | D5 | Reverse |
| IBT-2 R_EN | 5V | Must be high or the motor will not run |
| IBT-2 L_EN | 5V | |
| IBT-2 VCC | 5V | Logic supply |
| IBT-2 GND | GND | **Common ground is essential** |
| IBT-2 B+ / B− | 4S pack | Motor power |
| IBT-2 M+ / M− | RS-775 | Swap these (or RPWM/LPWM in firmware) to reverse direction |

## nRF24L01 header (2×4)

```
          GND row      VCC row
End 1  →  GND  (1)     VCC  (2)
          CE   (3)     CSN  (4)
          SCK  (5)     MOSI (6)
End 2  →  MISO (7)     IRQ  (8)
```
GND usually has a **square pad**. Check the silkscreen on the underside before powering.

## Wiring rules learned the hard way

| Rule | Why | Status |
| --- | --- | --- |
| One common ground for the Arduino, IBT-2, servo buck and nRF24 | Without it, signals have no reference and drivers misread inputs | As-built |
| Servo on its own buck converter | The DS-3225 draws enough current to reset the Arduino | As-built |
| Capacitor at the nRF24 power pins | The PA/LNA module draws current bursts the UNO's 3.3 V pin cannot supply cleanly | As-built |
| Solder or hot-glue jumper connections | Jumper wires broke during transport and testing | As-built (partially) |
| 10 kΩ pull-down resistors, RPWM and LPWM to GND | Holds the motor off while the Arduino boots or resets | **Planned** |
| Three 0.1 µF ceramic capacitors on the RS-775 (across terminals, each terminal to case) | Suppresses brushed-motor electrical noise | **Planned** |
| Twisted motor leads, routed away from signal wires | Reduces noise coupling | **Planned** |
