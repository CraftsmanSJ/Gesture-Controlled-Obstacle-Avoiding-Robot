#include <Wire.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); 
const uint64_t pipe = 0xE8E8F0F0E1ULL; 
const int MPU_addr = 0x68;
const float TILT_THRESHOLD = 12.0; 
const unsigned long SEND_INTERVAL = 100; 
unsigned long lastSend = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  delay(100);

  // MPU6050 wake
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  delay(50);

  if (!radio.begin()) {
    Serial.println("radio.begin() FAILED - wiring/power?");
    while (1);
  }
  Serial.print("Chip connected? "); Serial.println(radio.isChipConnected() ? "YES":"NO");

  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.setAutoAck(true);
  radio.setRetries(5, 15);
  radio.openWritingPipe(pipe);
  radio.stopListening();

  
  radio.setPayloadSize(1);

  Serial.println("Sender ready");
}

void loop() {
  if (millis() - lastSend < SEND_INTERVAL) return;
  lastSend = millis();

  
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 6, true);

  int16_t raw_ax = (Wire.read() << 8) | Wire.read();
  int16_t raw_ay = (Wire.read() << 8) | Wire.read();
  int16_t raw_az = (Wire.read() << 8) | Wire.read();

  float ax = raw_ax / 16384.0;
  float ay = raw_ay / 16384.0;
  float az = raw_az / 16384.0;

  float pitch = atan2(ay, sqrt(ax*ax + az*az)) * 180.0 / PI;
  float roll  = atan2(-ax, az) * 180.0 / PI;

  uint8_t cmd = 0;
  if (fabs(pitch) > fabs(roll)) {
    if (pitch > TILT_THRESHOLD) cmd = 1;       // FORWARD
    else if (pitch < -TILT_THRESHOLD) cmd = 2; // BACK
  } else {
    if (roll > TILT_THRESHOLD) cmd = 3;        // LEFT
    else if (roll < -TILT_THRESHOLD) cmd = 4;  // RIGHT
  }

  bool ok = radio.write(&cmd, 1);
  Serial.print("SEND -> cmd="); Serial.print(cmd);
  Serial.print("  pitch="); Serial.print(pitch,2);
  Serial.print(" roll="); Serial.print(roll,2);
  Serial.print("  write_ok="); Serial.println(ok ? "YES":"NO");
}