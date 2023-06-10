#include <TM1637Display.h>

#define CLK 13
#define DIO 12

// 디스플레이 설정
TM1637Display display(CLK, DIO);
uint8_t blank[] = { 0x00, 0x00, 0x00, 0x00 };

// 시간 변수
long prevTime = 0;
long currentTime = 0;
int seconds = 0;
int minutes = 0;

// 핀 번호
int blueLedPin = 11;
int pinAO = A0;
int buzzerPin = 9;

void setup() {
  Serial.begin(9600);

  pinMode(blueLedPin, OUTPUT);
  pinMode(pinAO, INPUT);
  pinMode(buzzerPin, OUTPUT);

  display.setBrightness(8);    // 밝기 설정 범위는 0 ~ 16
  display.setSegments(blank);  // 배열 출력 (빈칸 출력)
}

void loop() {
  int distance = analogRead(pinAO);
  // Serial.println(distance);
  // 차가 주차하면
  if (distance <= 980) {
    // 파란 LED OFF
    digitalWrite(blueLedPin, LOW);

    // 시간 카운트 시작
    long currentTime = millis();

    // 1초 마다 함수 수행
    if (currentTime - prevTime >= 1000) {
      seconds++;  // 1초 마다 1초 증가

      // 1분이 되기 30초 전에 부저 1초 울리기
      if (minutes == 0 && seconds == 30) {
        digitalWrite(buzzerPin, HIGH);
        Serial.println("주차 시간이 30초 남았습니다.");
      }
      if (minutes == 0 && seconds == 31) {
        digitalWrite(buzzerPin, LOW);
      }

      // 60초가 지나면 1분 증가
      if (seconds % 60 == 0) {
        minutes++;
        seconds = 0;
      }

      // 24분 지나면 시간 초기화
      if (minutes == 24) {
        minutes = 0;
        seconds = 0;
      }

      // 주차 가능한 시간(1분)이 모두 지나면 부저 1초 울리기
      if (minutes == 1 && seconds == 0) {
        digitalWrite(buzzerPin, HIGH);
        Serial.println("주차 시간 1분이 초과하여 과태료가 부과됩니다.");
      }
      if (minutes == 1 && seconds == 1) {
        digitalWrite(buzzerPin, LOW);
      }

      prevTime = currentTime;
    }
  } else {
    // 차가 나가면
    // 파란 LED ON
    digitalWrite(blueLedPin, HIGH);
    // 시간 초기화
    minutes = 0;
    seconds = 0;
  }

  // 시간을 display에 표시 (분 : 초)
  display.showNumberDec(minutes * 100 + seconds, false);

  delay(1000);
}
