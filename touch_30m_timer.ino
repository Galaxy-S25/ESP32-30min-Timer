#include "config.h"  // 모든 설정과 변수가 들어있는 파일을 포함합니다.

void setup() {

  Serial.begin(115200);

  preferences.begin("pomodoro", false);
  goodCount = preferences.getInt("good", 0);
  badCount = preferences.getInt("bad", 0);

  tft.init();
  tft.setRotation(1);

  WiFi.mode(WIFI_STA);  // 와이파이 모드를 Station으로 설정

  WiFiManager wm;
  wm.setConnectTimeout(10);  // 연결 시도 타임아웃을 10초로 설정

  // autoConnect: 저장된 정보로 연결을 시도하고, 실패하면 설정 포털을 엶
  if (!wm.autoConnect("ESP32-Timer-Setup")) {
    Serial.println("Failed to connect and hit timeout");
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_RED);
    tft.setCursor(20, 100);
    tft.setTextSize(2);
    tft.print("WiFi Setup Failed!");
    delay(3000);
    ESP.restart();  // 설정 실패 시 재부팅
  }

  Serial.println("\nWiFi CONNECTED!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // 인터넷 시간 설정
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  Serial.print("Syncing time");
  struct tm timeinfo;
  int retry = 0;
  while (!getLocalTime(&timeinfo) && retry < 20) {
    Serial.print(".");
    delay(500);
    retry++;
  }


  Serial.println("Time synced. Disconnecting WiFi...");
  WiFi.disconnect(true);  // 와이파이 연결을 끊고 모듈 전원도 끔

  // 백라이트 PWM 설정
  ledcAttach(TFT_BL_PIN, PWM_FREQ, PWM_RESOLUTION);
  ledcWrite(TFT_BL_PIN, map(brightnessLevel, 0, 4, 55, 255));

  // 터치스크린 설정
  touchscreenSPI.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  touchscreen.begin(touchscreenSPI);
  touchscreen.setRotation(1);

  // 시작 화면 표시
  displayIdleScreen();
}

void loop() {
  handleTouch();  // 터치 입력은 항상 확인

  // 1. 타이머가 실행 중일 때의 로직
  if (currentState == RUNNING && !isPaused) {
    updateTimerDisplay();
  } else if (currentState == BREAK_RUNNING && !isPaused) {
    updateBreakTimerDisplay();
  }

  // 2. 시계 화면일 때의 로직
  if (currentState == STATE_CLOCK) {
    static unsigned long lastClockUpdateTime = 0;
    if (millis() - lastClockUpdateTime > 1000) {
      lastClockUpdateTime = millis();
      updateClockDisplay();
    }
  }

  // 3. 타이머나 시계가 아닌 모든 화면에서의 공통 로직 (유휴 상태 체크)
  if (currentState != RUNNING && currentState != BREAK_RUNNING && currentState != STATE_CLOCK) {
    if (millis() - lastTouchTime > CLOCK_MODE_TIMEOUT) {
      displayClockScreen();
    }
  }

  delay(50);
}