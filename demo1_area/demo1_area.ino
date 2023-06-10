#include <TM1637Display.h>

#define CLK 6
#define DIO 5

TM1637Display display(CLK, DIO);
uint8_t data[] = { 0x00, 0x00, 0x00, 0x00 };

// 주차장 빈 자릿수
volatile int generalArea = 3;
int eletricArea = 1;
int disabledArea = 1;

// 적외선 센서와 각 LED 핀
int pinAO1 = A0;
int blueLedPin = 12;

int outPin = 8;
int redLedPin = 3;
int greenLedPin = 2;

int pinAO2 = A5;
int yellowLedPin = 4;

void setup() {
  Serial.begin(9600);

  pinMode(pinAO1, INPUT);
  pinMode(blueLedPin, OUTPUT);

  pinMode(outPin, INPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);

  pinMode(pinAO2, INPUT);
  pinMode(yellowLedPin, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(redLedPin), countUp, FALLING);
  attachInterrupt(digitalPinToInterrupt(greenLedPin), countDown, FALLING);
  display.setBrightness(8);  // 밝기 설정 범위는 0 ~ 16

  // 주차 구역 빈 자릿수 표시
  displayArea();
}

void countUp() {
  generalArea++;
}

void countDown() {
  generalArea--;
}

void displayArea() {
  // 주차 구역 빈 자릿수 표시
  data[0] = display.encodeDigit(generalArea);
  data[1] = display.encodeDigit(eletricArea);
  data[2] = display.encodeDigit(disabledArea);
  display.setSegments(data);  // 배열 출력
}

void loop() {
  int dist1 = analogRead(pinAO1);
  
  if (dist1 <= 965) {
    // 파란색 LED 끄기
    digitalWrite(blueLedPin, LOW);
    eletricArea = 0;
  } else {
    // 파란색 LED 켜기
    digitalWrite(blueLedPin, HIGH);
    eletricArea = 1;
  }

  int dist2 = digitalRead(outPin);
  if (dist2 == LOW) {
    // 빨간색 LED 켜기, 초록색 LED 끄기
    digitalWrite(redLedPin, HIGH);
    digitalWrite(greenLedPin, LOW);
  } else {
    // 빨간색 LED 끄기, 초록색 LED 켜기
    digitalWrite(redLedPin, LOW);
    digitalWrite(greenLedPin, HIGH);
  }

  int dist3 = analogRead(pinAO2);
  Serial.println(dist3);
  if (dist3 <= 965) {
    // 노란색 LED 끄기
    digitalWrite(yellowLedPin, LOW);
    disabledArea = 0;
  } else {
    // 노란색 LED 켜기
    digitalWrite(yellowLedPin, HIGH);
    disabledArea = 1;
  }

  displayArea();

  delay(200);
}
