/*
 * Project Airaavat - TX Diagnostic Sketch
 * ---------------------------------------
 * Isolation test for the transmitter: Arduino, joysticks and nRF24 only.
 * Sends a packet with an ID marker and a running counter so the RX diagnostic
 * can detect corrupted and missed packets.
 *
 * Auto-ack is ON here on purpose: "ACK: OK" proves the full link end to end.
 * Expected with RX off: ACK FAIL on every packet (nobody is listening).
 * Serial Monitor: 115200 baud.
 */

#include <SPI.h>
#include <RF24.h>
#include <printf.h>

const int THROTTLE_PIN = A0;
const int STEERING_PIN = A1;

RF24 radio(9, 10);
const byte ADDRESS[6] = "AIRV2";

struct DiagPacket {
  uint16_t magic;
  uint32_t counter;
  int16_t throttle;
  int16_t steering;
};

DiagPacket pkt;
unsigned long sentOK = 0;
unsigned long sentFail = 0;

void setup() {
  Serial.begin(115200);
  printf_begin();
  Serial.println("=== TX DIAGNOSTIC ===");

  if (!radio.begin()) {
    Serial.println("FAIL: nRF24 not responding. Check wiring and 3.3V.");
    while (true) {}
  }
  Serial.print("Chip connected: ");
  Serial.println(radio.isChipConnected() ? "YES" : "NO");
  Serial.print("Packet size: ");
  Serial.println(sizeof(pkt));

  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(ADDRESS);
  radio.stopListening();
  radio.printPrettyDetails();

  pkt.magic = 0xA1B2;
  pkt.counter = 0;
}

void loop() {
  pkt.throttle = analogRead(THROTTLE_PIN);
  pkt.steering = analogRead(STEERING_PIN);
  pkt.counter++;

  bool ok = radio.write(&pkt, sizeof(pkt));
  if (ok) {
    sentOK++;
  } else {
    sentFail++;
  }

  Serial.print("#"); Serial.print(pkt.counter);
  Serial.print("  T: "); Serial.print(pkt.throttle);
  Serial.print("  S: "); Serial.print(pkt.steering);
  Serial.print("  ACK: "); Serial.print(ok ? "OK  " : "FAIL");
  Serial.print("  totals OK/FAIL: ");
  Serial.print(sentOK); Serial.print("/"); Serial.println(sentFail);

  delay(100);
}
