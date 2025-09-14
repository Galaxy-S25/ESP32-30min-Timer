#include "config.h" // 모든 설정과 변수가 들어있는 파일을 포함합니다.

void setup() {
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
  
  delay(50);
}