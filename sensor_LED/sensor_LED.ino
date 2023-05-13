int inputPin = 4;   // define ultrasonic signal receiver pin ECHO to D4
int outputPin = 5;  // define ultrasonic signal transmitter pin TRIG to D5
int redLEDPin = 13;
int greenLEDPin = 2;

void setup() {

  Serial.begin(9600);
  pinMode(inputPin, INPUT);
  pinMode(outputPin, OUTPUT);
}

void loop() {
  // 센서의 펄스 시간을 읽어서 거리로 변환
  digitalWrite(outputPin, LOW);
  delayMicroseconds(2);
  digitalWrite(outputPin, HIGH);  // Pulse for 10μ s to trigger ultrasonic detection
  delayMicroseconds(10);
  digitalWrite(outputPin, LOW);

  int distance = pulseIn(inputPin, HIGH);
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

  Serial.println(distance);
  delay(100);
}