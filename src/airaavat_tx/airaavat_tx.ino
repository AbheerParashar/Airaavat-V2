/*
 * Project Airaavat - V2 Transmitter (TX)
 * ---------------------------------------
 * Reads two KY-023 joysticks (throttle, steering) and sends them to the rover
 * over an nRF24L01+PA/LNA radio at ~50 Hz.
 *
 * Link design: one-way (auto-ack disabled). The receiver validates every packet
 * and runs its own signal-loss failsafe. See docs/debugging-case-study.md.
 *
 * Board: Arduino UNO
 * Library: RF24 by TMRh20
 * Status: As-built, ground-tested 7 October 2026
 */

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// ---------- Pins ----------
const int THROTTLE_PIN = A0;
const int STEERING_PIN = A1;
const int CE_PIN = 9;
const int CSN_PIN = 10;

// ---------- Timing ----------
const unsigned long SEND_INTERVAL_MS = 20;    // ~50 packets per second
const unsigned long DEBUG_INTERVAL_MS = 200;  // Serial print rate
const bool DEBUG = true;                      // set false for normal driving

// ---------- Radio ----------
RF24 radio(CE_PIN, CSN_PIN);
const byte ADDRESS[6] = "AIRV2";
const uint16_t MAGIC = 0xA1B2;                // packet ID marker

// Fixed-size types so the packet is identical on UNO and ESP32
struct ControlData {
  uint16_t magic;
  int16_t throttle;
  int16_t steering;
};

ControlData data;
unsigned long lastSend = 0;
unsigned long lastDebug = 0;
unsigned long packetsSent = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("TX START");

  if (!radio.begin()) {
    Serial.println("RADIO NOT FOUND - check nRF24 wiring");
    while (true) {}
  }
  radio.setAutoAck(false);
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(ADDRESS);
  radio.stopListening();

  data.magic = MAGIC;
}

void loop() {
  // Read joysticks and transmit at a fixed rate
  if (millis() - lastSend >= SEND_INTERVAL_MS) {
    lastSend = millis();
    data.throttle = analogRead(THROTTLE_PIN);
    data.steering = analogRead(STEERING_PIN);
    radio.write(&data, sizeof(data));
    packetsSent++;
  }

  // Debug output (non-blocking)
  if (DEBUG && millis() - lastDebug >= DEBUG_INTERVAL_MS) {
    lastDebug = millis();
    Serial.print("T: "); Serial.print(data.throttle);
    Serial.print("  S: "); Serial.print(data.steering);
    Serial.print("  sent: "); Serial.println(packetsSent);
  }
}
