/*
 * Project Airaavat - V2 Receiver (RX, on the rover)
 * -------------------------------------------------
 * Receives joystick packets, validates them, and drives:
 *   - RS-775 motor through an IBT-2 (BTS7960) H-bridge
 *   - DS-3225 steering servo (Ackermann linkage)
 *
 * Features: packet validation, joystick deadband, squared throttle curve,
 * soft-start ramp, 500 ms signal-loss failsafe.
 *
 * Board: Arduino UNO
 * Libraries: RF24 by TMRh20, Servo
 * Status: As-built, ground-tested 7 October 2026
 *
 * Calibration values below are for this specific build. Re-measure the joystick
 * centres and servo limits after any rewiring or mechanical change.
 */

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

// ---------- Pins ----------
const int CE_PIN = 9;
const int CSN_PIN = 10;
const int STEERING_SERVO_PIN = 3;
const int RPWM_PIN = 6;   // IBT-2 forward
const int LPWM_PIN = 5;   // IBT-2 reverse

// ---------- Calibration ----------
const int THROTTLE_CENTRE = 501;   // measured joystick rest value
const int STEERING_CENTRE = 509;   // measured joystick rest value
const int JOY_MAX = 1022;          // measured joystick maximum
const int DEADBAND = 30;           // +/- zone treated as "stop"
const int MAX_PWM = 255;
const int RAMP_STEP = 5;                     // PWM change per ramp tick
const unsigned long RAMP_INTERVAL_MS = 10;   // 0 -> 255 in ~0.5 s

const int SERVO_LEFT = 60;      // full left lock (measured, no binding)
const int SERVO_CENTRE = 90;    // wheels straight
const int SERVO_RIGHT = 120;    // full right lock (measured, no binding)

const unsigned long FAILSAFE_MS = 500;

// ---------- Debug ----------
const bool DEBUG = true;                      // set false for normal driving
const unsigned long DEBUG_INTERVAL_MS = 200;

// ---------- Radio ----------
RF24 radio(CE_PIN, CSN_PIN);
const byte ADDRESS[6] = "AIRV2";
const uint16_t MAGIC = 0xA1B2;

struct ControlData {
  uint16_t magic;
  int16_t throttle;
  int16_t steering;
};

ControlData data;
Servo steeringServo;

unsigned long lastReceived = 0;
unsigned long lastRampUpdate = 0;
unsigned long lastDebug = 0;
unsigned long goodPackets = 0;
unsigned long badPackets = 0;
int targetSpeed = 0;    // signed: + forward, - reverse
int currentSpeed = 0;   // ramped value actually applied
int currentAngle = SERVO_CENTRE;
bool signalOK = false;

void setup() {
  // Motor outputs first, held off before anything else happens
  pinMode(RPWM_PIN, OUTPUT);
  pinMode(LPWM_PIN, OUTPUT);
  stopMotor();

  Serial.begin(115200);
  Serial.println("RX START");

  steeringServo.attach(STEERING_SERVO_PIN);
  setServo(SERVO_CENTRE);

  if (!radio.begin()) {
    Serial.println("RADIO NOT FOUND - check nRF24 wiring");
    while (true) {}
  }
  radio.setAutoAck(false);          // receiver never transmits
  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(0, ADDRESS);
  radio.startListening();
}

void loop() {
  // 1. Receive and validate
  if (radio.available()) {
    ControlData incoming;
    radio.read(&incoming, sizeof(incoming));
    if (isValid(incoming)) {
      data = incoming;
      goodPackets++;
      lastReceived = millis();
      signalOK = true;
      targetSpeed = throttleToSpeed(data.throttle);
      steer(data.steering);
    } else {
      badPackets++;
    }
  }

  // 2. Failsafe: no valid packet for FAILSAFE_MS -> stop and centre
  if (millis() - lastReceived > FAILSAFE_MS) {
    signalOK = false;
    targetSpeed = 0;
    currentSpeed = 0;   // skip the ramp: safety stops are instant
    stopMotor();
    setServo(SERVO_CENTRE);
  }

  // 3. Soft-start ramp toward the target speed
  if (millis() - lastRampUpdate >= RAMP_INTERVAL_MS) {
    lastRampUpdate = millis();
    if (currentSpeed < targetSpeed) {
      currentSpeed = min(currentSpeed + RAMP_STEP, targetSpeed);
    } else if (currentSpeed > targetSpeed) {
      currentSpeed = max(currentSpeed - RAMP_STEP, targetSpeed);
    }
    applySpeed(currentSpeed);
  }

  // 4. Debug output (non-blocking)
  if (DEBUG && millis() - lastDebug >= DEBUG_INTERVAL_MS) {
    lastDebug = millis();
    Serial.print(signalOK ? "LINK OK  " : "NO LINK  ");
    Serial.print("T: "); Serial.print(data.throttle);
    Serial.print("  S: "); Serial.print(data.steering);
    Serial.print("  target: "); Serial.print(targetSpeed);
    Serial.print("  current: "); Serial.print(currentSpeed);
    Serial.print("  angle: "); Serial.print(currentAngle);
    Serial.print("  good/bad: "); Serial.print(goodPackets);
    Serial.print("/"); Serial.println(badPackets);
  }
}

// Reject packets that are not from our TX or carry impossible values
bool isValid(ControlData p) {
  if (p.magic != MAGIC) return false;
  if (p.throttle < 0 || p.throttle > 1023) return false;
  if (p.steering < 0 || p.steering > 1023) return false;
  return true;
}

// Joystick reading -> signed motor speed (-255..255) with deadband and squared curve
int throttleToSpeed(int reading) {
  int upper = THROTTLE_CENTRE + DEADBAND;
  int lower = THROTTLE_CENTRE - DEADBAND;
  float x;

  if (reading > upper) {
    x = (float)(reading - upper) / (JOY_MAX - upper);
  } else if (reading < lower) {
    x = -(float)(lower - reading) / lower;
  } else {
    return 0;
  }

  x = constrain(x, -1.0, 1.0);
  float curved = x * abs(x);   // squared curve, sign preserved
  return (int)(curved * MAX_PWM);
}

// Signed speed -> IBT-2 PWM pins
void applySpeed(int speed) {
  if (speed > 0) {
    analogWrite(LPWM_PIN, 0);
    analogWrite(RPWM_PIN, speed);
  } else if (speed < 0) {
    analogWrite(RPWM_PIN, 0);
    analogWrite(LPWM_PIN, -speed);
  } else {
    stopMotor();
  }
}

// Joystick reading -> servo angle within the measured lock limits
void steer(int reading) {
  int angle;
  if (reading >= STEERING_CENTRE) {
    angle = map(reading, STEERING_CENTRE, JOY_MAX, SERVO_CENTRE, SERVO_RIGHT);
  } else {
    angle = map(reading, 0, STEERING_CENTRE, SERVO_LEFT, SERVO_CENTRE);
  }
  angle = constrain(angle, min(SERVO_LEFT, SERVO_RIGHT), max(SERVO_LEFT, SERVO_RIGHT));
  setServo(angle);
}

// Only write to the servo when the angle actually changes
void setServo(int angle) {
  if (angle != currentAngle) {
    currentAngle = angle;
    steeringServo.write(angle);
  }
}

void stopMotor() {
  analogWrite(RPWM_PIN, 0);
  analogWrite(LPWM_PIN, 0);
}
