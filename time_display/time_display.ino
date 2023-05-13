#include <TM1637Display.h>

#define CLK 12
#define DIO 13

TM1637Display display(CLK, DIO);

uint8_t blank[] = { 0x00, 0x00, 0x00, 0x00 };

int echoInputPin = 4;   // define ultrasonic signal receiver pin ECHO to D4
int trigOutputPin = 5;  // define ultrasonic signal transmitter pin TRIG to D5
int blueLEDPin = 2;

long prevTime = 0;
long currentTime = 0;
int seconds = 0;
int minutes = 0;

void setup() {
  Serial.begin(9600);
  pinMode(echoInputPin, INPUT);
  pinMode(trigOutputPin, OUTPUT);
  pinMode(blueLEDPin, OUTPUT);

  display.setBrightness(8);    // 밝기 설정 범위는 0 ~ 16
  display.setSegments(blank);  // 배열 출력
}

void loop() {
  // 센서의 펄스 시간을 읽어서 거리로 변환
  digitalWrite(trigOutputPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigOutputPin, HIGH);  // Pulse for 10μ s to trigger ultrasonic detection
  delayMicroseconds(10);
  digitalWrite(trigOutputPin, LOW);

  int distance = pulseIn(echoInputPin, HIGH);
  distance = distance / 58;

  // 차가 주차하면
  if (distance <= 10) {
    // 파란 LED OFF
    digitalWrite(blueLEDPin, LOW);

    // 시간 카운트 시작
    long currentTime = millis();

    // 1초 마다 함수 수행
    if (currentTime - prevTime >= 1000) {
      seconds++;

      // 60초가 지나면 1분 증가
      if (seconds % 60 == 0) {
        minutes++;
        seconds = 0;
      }

      // 24분 지나면 초기화
      if (minutes == 24) {
        minutes = 0;
        seconds = 0;
      }
      prevTime = currentTime;
    }
  } else {
    // 차가 나가면
    // 파란 LED ON
    digitalWrite(blueLEDPin, HIGH);
    // 시간 초기화
    minutes = 0;
    seconds = 0;
  }

  // 시간을 display에 표시
  display.showNumberDec(minutes * 100 + seconds, false);

  delay(100);
}