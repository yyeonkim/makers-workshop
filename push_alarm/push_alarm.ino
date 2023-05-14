#include <TM1637Display.h>
#include <SoftwareSerial.h>

#define CLK 12
#define DIO 13

TM1637Display display(CLK, DIO);
SoftwareSerial esp8266(0, 1);  // RX, TX

uint8_t blank[] = { 0x00, 0x00, 0x00, 0x00 };

long prevMillis = 0;
long currentMillis = 0;
int seconds = 0;
int minutes = 0;

void httpRequest();
void printWifiStatus();

void setup() {
  Serial.begin(9600);
  esp8266.begin(115200);  // 통신 속도 설정

  display.setBrightness(8);    // 밝기 설정 범위는 0 ~ 16
  display.setSegments(blank);  // 배열 출력

  // Connect to Wi-Fi network
  connectWiFi();
  // Send push notification via IFTTT webhooks
  // sendPushNotification();
}

void connectWiFi() {
  String ssid = "SK_WiFiGIGA3548";
  String password = "2002011320";

  Serial.print("Connecting to ");
  Serial.println(ssid);

  esp8266.println("AT+CWJAP=\"" + ssid + "\",\"" + password + "\"");

  delay(4000);

  if (esp8266.find("OK")) {
    Serial.println("Connected!");
  }
  else {
    Serial.println("Failed to connect.");
  }
}

void sendPushNotification() {
  // Set your IFTTT API key and event name
  String apiKey = "YOUR_IFTTT_API_KEY";
  String eventName = "YOUR_EVENT_NAME";

  // Send an HTTP POST request to the IFTTT webhooks service
  esp8266.print("POST /trigger/" + eventName + "/with/key/" + apiKey);
  esp8266.println(" HTTP/1.1");
  esp8266.println("Host: maker.ifttt.com");
  esp8266.println("Connection: close\r\n");

  Serial.println("Push notification sent!");
}

void loop() {
  // 시간 카운트 시작
  long currentMillis = millis();

  // 1초 마다 함수 수행
  if (currentMillis - prevMillis >= 1000) {
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
    prevMillis = currentMillis;
  }

  // 시간을 display에 표시
  display.showNumberDec(minutes * 100 + seconds, false);

  delay(100);
}