#include<IRremote.h>

int edhoInputPin = 4;   // define ultrasonic signal receiver pin ECHO to D4
int trigOutputPin = 5;  // define ultrasonic signal transmitter pin TRIG to D5
int redLEDPin = 13;
int greenLEDPin = 2;
int blueLEDPin = 12;
int irInputPin = 7;

void setup() {
  Serial.begin(9600);
  pinMode(edhoInputPin, INPUT);
  pinMode(trigOutputPin, OUTPUT);
  pinMode(irInputPin, INPUT);
  pinMode(redLEDPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);
  pinMode(blueLEDPin, OUTPUT);
}

void loop() {
  // 센서의 펄스 시간을 읽어서 거리로 변환
  digitalWrite(trigOutputPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigOutputPin, HIGH);  // Pulse for 10μ s to trigger ultrasonic detection
  delayMicroseconds(10);
  digitalWrite(trigOutputPin, LOW);

  int distance = pulseIn(edhoInputPin, HIGH);
  distance = distance / 58;

  if (distance <= 10) {
    // 빨간 LED OFF 초록 LED ON
    digitalWrite(redLEDPin, LOW);
    digitalWrite(greenLEDPin, HIGH);
  } else {
    // 빨간 LED ON 초록 LED OFF
    digitalWrite(redLEDPin, HIGH);
    digitalWrite(greenLEDPin, LOW);
  }

  // 전기차가 있으면 LED OFF
  // 없으면 LED ON
  if (digitalRead(irInputPin) == LOW) {
    digitalWrite(blueLEDPin, LOW);
  } else {
    digitalWrite(blueLEDPin, HIGH);
  }

  delay(100);
}