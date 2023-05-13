#include <TM1637Display.h>

#define CLK 12
#define DIO 13

TM1637Display display(CLK, DIO);

volatile int generalArea = 3;
int eletricArea = 1;
int disabledArea = 1;

uint8_t data[] = { 0x00, 0x00, 0x00, 0x00 };

int edhoInputPin = 4;   // define ultrasonic signal receiver pin ECHO to D4
int trigOutputPin = 5;  // define ultrasonic signal transmitter pin TRIG to D5
int redLEDPin = 3;
int greenLEDPin = 2;

void setup() {
  Serial.begin(9600);
  pinMode(edhoInputPin, INPUT);
  pinMode(trigOutputPin, OUTPUT);
  pinMode(redLEDPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(redLEDPin), countDown, FALLING);
  attachInterrupt(digitalPinToInterrupt(greenLEDPin), countUp, FALLING);
  display.setBrightness(8);  // 밝기 설정 범위는 0 ~ 16

  // 주차 구역 빈 자릿수 표시
  data[0] = display.encodeDigit(generalArea);
  data[1] = display.encodeDigit(eletricArea);
  data[2] = display.encodeDigit(disabledArea);
  display.setSegments(data);  // 배열 출력
}

void countUp() {
  generalArea++;
}

void countDown() {
  generalArea--;
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

  // 주차 구역 빈 자릿수 표시
  data[0] = display.encodeDigit(generalArea);
  display.setSegments(data);  // 배열 출력

  delay(100);
}