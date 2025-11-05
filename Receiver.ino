
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10);
const uint64_t pipe = 0xE8E8F0F0E1ULL;


const int L_IN1 = 2;
const int L_IN2 = 3;
const int R_IN1 = 7;
const int R_IN2 = 8;


const int US_TRIG = 6;
const int US_ECHO = A0; 

const int DIST_THRESHOLD_CM = 50; 

void setup() {
  Serial.begin(115200);

  pinMode(L_IN1, OUTPUT); pinMode(L_IN2, OUTPUT);
  pinMode(R_IN1, OUTPUT); pinMode(R_IN2, OUTPUT);
  stopMotors();

  pinMode(US_TRIG, OUTPUT);
  pinMode(US_ECHO, INPUT);
  digitalWrite(US_TRIG, LOW);

  if (!radio.begin()) { 
    Serial.println("radio.begin() failed"); 
    while (1); 
  }
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.setAutoAck(true);
  radio.openReadingPipe(0, pipe);
  radio.startListening();
  radio.setPayloadSize(1);

  Serial.println("Receiver ready");
}

void loop() {
  int distance = readUltrasonicCM();
  int flag = 0;

  
  if (distance > 0 && distance < DIST_THRESHOLD_CM) {
    
    flag = 1;
    Serial.print("Obstacle! Dist="); Serial.print(distance); Serial.println(" cm -> STOP");
    delay(100);
  }

  
  if (radio.available()) {
    uint8_t cmd = 0;
    radio.read(&cmd, 1);
    Serial.print("Got cmd="); Serial.println((int)cmd);
    applyCommand(cmd, flag);
  }

  delay(20);
}

long readUltrasonicCM() {
  digitalWrite(US_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(US_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(US_TRIG, LOW);

  unsigned long duration = pulseIn(US_ECHO, HIGH, 30000UL);
  if (duration == 0) return -1;
  return (long)(duration / 58UL);
}

void applyCommand(uint8_t c , int flag) {
  switch (c) {
    case 0: stopMotors(); break;  // Stop
    case 1: motor("Left", "Forward");  motor("Right", "Stop");     break; // Turn left
    case 2: motor("Left", "Stop");     motor("Right", "Forward");  break; // Turn right
    case 3: if(flag == 0) {motor("Left", "Forward");  motor("Right", "Forward");}  break; // Forward
    case 4: motor("Left", "Backward"); motor("Right", "Backward"); break; // Backward
    default: stopMotors(); break;
  }
}

void motor(String side, String dir) {
  if (side == "Left") {
    if (dir == "Forward") {
      digitalWrite(L_IN1, HIGH); digitalWrite(L_IN2, LOW);
    } 
    else if (dir == "Backward") {
      digitalWrite(L_IN1, LOW); digitalWrite(L_IN2, HIGH);
    } 
    else {
      digitalWrite(L_IN1, LOW); digitalWrite(L_IN2, LOW);
    }
  } 
  else if (side == "Right") {
    if (dir == "Forward") {
      digitalWrite(R_IN1, HIGH); digitalWrite(R_IN2, LOW);
    } 
    else if (dir == "Backward") {
      digitalWrite(R_IN1, LOW); digitalWrite(R_IN2, HIGH);
    } 
    else {
      digitalWrite(R_IN1, LOW); digitalWrite(R_IN2, LOW);
    }
  }
}

void stopMotors() {
  digitalWrite(L_IN1, LOW); digitalWrite(L_IN2, LOW);
  digitalWrite(R_IN1, LOW); digitalWrite(R_IN2, LOW);
}