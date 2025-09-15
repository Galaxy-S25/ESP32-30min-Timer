#include "config.h" // 모든 설정과 변수가 들어있는 파일을 포함합니다.

void setup() {

  // Preferences 초기화 및 저장된 스코어 불러오기
  preferences.begin("pomodoro", false); // "pomodoro"라는 이름으로 저장 공간 사용
  goodCount = preferences.getInt("good", 0); // "good" 키로 저장된 값을 불러옴, 없으면 0
  badCount = preferences.getInt("bad", 0);   // "bad" 키로 저장된 값을 불러옴, 없으면 0
 
  Serial.begin(115200);
  tft.init();
  tft.setRotation(1);

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
  handleTouch(); // 터치 입력을 계속 확인

  // 현재 상태에 따라 필요한 업데이트 함수를 호출
  if (currentState == RUNNING && !isPaused) {
    updateTimerDisplay();
  }
  if (currentState == BREAK_RUNNING && !isPaused) {
    updateBreakTimerDisplay();
  }

  // 결과창 또는 점수창에서 3분간 입력이 없으면 홈으로 이동
  if (currentState == SHOW_SCORE || currentState == FEEDBACK) {
    if (millis() - lastTouchTime > AUTO_HOME_TIMEOUT) {
      displayIdleScreen();
    }
  }

  // 메인 화면에서 20분간 입력이 없으면 절전 모드 진입
  if (currentState == STATE_IDLE) {
    if (millis() - lastTouchTime > DEEP_SLEEP_TIMEOUT) {
      // 1. 화면 백라이트를 끈다.
      ledcWrite(TFT_BL_PIN, 0); 
      delay(100); // 백라이트가 꺼질 시간을 줌

      // 2. 터치스크린의 IRQ 핀(GPIO 36)에서 신호가 감지되면 깨어나도록 설정
      esp_sleep_enable_ext0_wakeup(GPIO_NUM_36, 0); // 핀이 LOW 상태일 때 깨어남

      // 3. 절전 모드 시작
      esp_deep_sleep_start();
    }
  }
  delay(50);
}