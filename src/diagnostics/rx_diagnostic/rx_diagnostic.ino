/*
 * Project Airaavat - RX Diagnostic Sketch
 * ---------------------------------------
 * Isolation test for the receiver: Arduino and nRF24 only.
 * The motor driver pins are held LOW so the motor cannot run during the test.
 *
 * Reports: chip detection, packet size, received values, missed packets
 * (counter gaps) and bad packets (wrong ID marker).
 * Serial Monitor: 115200 baud.
 */

#include <SPI.h>
#include <RF24.h>
#include <printf.h>

const int RPWM_PIN = 6;
const int LPWM_PIN = 5;

RF24 radio(9, 10);
const byte ADDRESS[6] = "AIRV2";

struct DiagPacket {
  uint16_t magic;
  uint32_t counter;
  int16_t throttle;
  int16_t steering;
};

DiagPacket pkt;
unsigned long received = 0;
unsigned long badPackets = 0;
unsigned long missed = 0;
uint32_t lastCounter = 0;
unsigned long lastPacketTime = 0;
unsigned long lastWarning = 0;

void setup() {
  pinMode(RPWM_PIN, OUTPUT);
  pinMode(LPWM_PIN, OUTPUT);
  digitalWrite(RPWM_PIN, LOW);
  digitalWrite(LPWM_PIN, LOW);

  Serial.begin(115200);
  printf_begin();
  Serial.println("=== RX DIAGNOSTIC ===");

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
  radio.openReadingPipe(0, ADDRESS);
  radio.startListening();
  radio.printPrettyDetails();
}

void loop() {
  if (radio.available()) {
    radio.read(&pkt, sizeof(pkt));
    received++;
    lastPacketTime = millis();

    if (pkt.magic != 0xA1B2) {
      badPackets++;
      Serial.print("BAD PACKET! magic = ");
      Serial.println(pkt.magic, HEX);
      return;
    }

    if (lastCounter != 0 && pkt.counter > lastCounter + 1) {
      missed += pkt.counter - lastCounter - 1;
    }
    lastCounter = pkt.counter;

    Serial.print("#"); Serial.print(pkt.counter);
    Serial.print("  T: "); Serial.print(pkt.throttle);
    Serial.print("  S: "); Serial.print(pkt.steering);
    Serial.print("  rx: "); Serial.print(received);
    Serial.print("  missed: "); Serial.print(missed);
    Serial.print("  bad: "); Serial.println(badPackets);
  }

  if (millis() - lastPacketTime > 1000 && millis() - lastWarning > 1000) {
    lastWarning = millis();
    Serial.println("NO SIGNAL (nothing received for 1 s)");
  }
}
