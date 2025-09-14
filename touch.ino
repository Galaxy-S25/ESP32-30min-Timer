void handleTouch() {
  if (touchscreen.tirqTouched() && touchscreen.touched()) {
    TS_Point p = touchscreen.getPoint();
    int x = map(p.x, 200, 3700, 1, SCREEN_WIDTH);
    int y = map(p.y, 240, 3800, 1, SCREEN_HEIGHT);
    int quadrant;

    switch (currentState) {
      case STATE_IDLE:
        quadrant = getQuadrant(x, y);
        if (quadrant == 1) {
          displayTimerScreen();
        } else if (quadrant == 2) {
          previousState = STATE_IDLE;
          displayScoreScreen();
        } else if (quadrant == 3) {
          displayBreakTimerScreen();
        } else if (quadrant == 4) {
          displaySettingsScreen();
        }
        break;

      case RUNNING:
      case BREAK_RUNNING:
        if (x >= DEAD_ZONE_X && x < (DEAD_ZONE_X + DEAD_ZONE_W) && y >= DEAD_ZONE_Y && y < (DEAD_ZONE_Y + DEAD_ZONE_H)) {
          break;
        }

        quadrant = getQuadrant(x, y);

        if (quadrant == 4) {
          if (!isPaused) {
            isPaused = true;
            pauseStartTime = millis();
          }
          previousState = currentState;
          displayConfirmExitScreen();
          break;
        }

        if (currentState == RUNNING) {
          if (quadrant == 1) {
            if (!isPaused) {
              flashButton(1);
              totalTime += THIRTY_MINUTES;
            }
          } else if (quadrant == 2) {
            if (!isPaused) {
              // 터치가 시작된 시간을 기록
              unsigned long pressStartTime = millis();

              // 사용자가 손을 뗄 때까지 기다림
              while (touchscreen.touched()) {
                delay(20);  // CPU 부담을 줄이기 위한 짧은 딜레이
              }

              // 손을 뗀 후, 총 눌린 시간을 계산
              unsigned long pressDuration = millis() - pressStartTime;

              // 길게 누르는 기준 시간 (ms 단위, 700ms = 0.7초)
              const unsigned long LONG_PRESS_THRESHOLD = 700;

              flashButton(2);  // 버튼 누름 효과 표시

              // 길게 눌렀는지 확인
              if (pressDuration > LONG_PRESS_THRESHOLD) {
                // 길게 눌렀을 때: 남은 시간을 1초로 설정
                totalTime = 1000;
              } else {
                // 짧게 눌렀을 때: 남은 시간이 30분 이상일 경우에만 30분 감소
                if (totalTime > THIRTY_MINUTES) {
                  totalTime -= THIRTY_MINUTES;
                }
                // 참고: 짧게 눌렀을 때 30분 미만이면 아무 동작도 하지 않습니다.
              }
            }
          }

          else if (quadrant == 3) {
            isPaused = !isPaused;
            tft.fillRect(40, 180, 100, 25, TFT_NAVY);
            tft.setTextColor(TFT_WHITE, TFT_NAVY);
            tft.setTextSize(2);
            tft.setCursor(40, 180);
            if (isPaused) {
              pauseStartTime = millis();
              tft.print("Resume");
            } else {
              startTime += (millis() - pauseStartTime);
              tft.print("Pause");
            }
            updateTimerDisplay(true);
          }
        } else {  // BREAK_RUNNING
          if (quadrant == 1) {
            if (!isPaused && totalTime < (10UL * 60UL * 1000UL)) { totalTime += FIVE_MINUTES; }
          } else if (quadrant == 2) {
            if (!isPaused && totalTime > FIVE_MINUTES) { totalTime -= FIVE_MINUTES; }
          } else if (quadrant == 3) {
            isPaused = !isPaused;
            tft.fillRect(40, 180, 100, 25, TFT_BLUE);
            tft.setTextColor(TFT_WHITE, TFT_BLUE);
            tft.setTextSize(2);
            tft.setCursor(40, 180);
            if (isPaused) {
              pauseStartTime = millis();
              tft.print("Resume");
            } else {
              startTime += (millis() - pauseStartTime);
              tft.print("Pause");
            }
            updateBreakTimerDisplay(true);
          }
        }
        break;

      case SHOW_SCORE:
        if (previousState == FEEDBACK) {
          displayFeedbackScreen();
        } else {
          displayIdleScreen();
        }
        break;

      case STATE_SETTINGS:
        quadrant = getQuadrant(x, y);
        if (quadrant == 1) {
          if (brightnessLevel < 4) {
            brightnessLevel++;
            ledcWrite(TFT_BL_PIN, map(brightnessLevel, 0, 4, 55, 255));
            drawBrightnessIndicator();
          }
        } else if (quadrant == 3) {
          if (brightnessLevel > 0) {
            brightnessLevel--;
            ledcWrite(TFT_BL_PIN, map(brightnessLevel, 0, 4, 55, 255));
            drawBrightnessIndicator();
          }
        } else if (quadrant == 4) {
          displayIdleScreen();
        }
        break;

      case STATE_CONFIRM_EXIT:
        if (x > 60 && x < 150 && y > 120 && y < 160) {
          redrawTimerScreen(previousState == BREAK_RUNNING);
        } else if (x > 170 && x < 260 && y > 120 && y < 160) {
          isPaused = false;
          displayIdleScreen();
        }
        break;

      case FEEDBACK:
        quadrant = getQuadrant(x, y);
        if (quadrant == 0) break;
        if (quadrant == 2) {
          previousState = FEEDBACK;
          displayScoreScreen();
          break;
        }
        if (quadrant == selectedQuadrant) {
          if (quadrant == 1 || quadrant == 3) {
            drawFeedbackButton(quadrant, false);
            delay(50);
            drawFeedbackText(quadrant, TFT_MAROON);
            delay(200);
            drawFeedbackText(quadrant, TFT_WHITE);
            delay(200);
            drawFeedbackText(quadrant, TFT_MAROON);
            delay(200);
            drawFeedbackText(quadrant, TFT_WHITE);
            delay(200);
            drawFeedbackText(quadrant, TFT_MAROON);
            delay(200);
            drawFeedbackText(quadrant, TFT_WHITE);
            delay(200);
            drawFeedbackText(quadrant, TFT_MAROON);
            delay(100);
            if (quadrant == 1) goodCount++;
            else badCount++;
            displayIdleScreen();
          } else if (quadrant == 4) {
            displayIdleScreen();
          }
        } else {
          if (selectedQuadrant != 0) {
            drawFeedbackButton(selectedQuadrant, false);
          }
          selectedQuadrant = quadrant;
          drawFeedbackButton(selectedQuadrant, true);
        }
        break;
    }
    delay(200);
    while (touchscreen.touched())
      ;
  }
}

int getQuadrant(int x, int y) {
  int midScreenX = SCREEN_WIDTH / 2;
  int midScreenY = SCREEN_HEIGHT / 2;
  if (x < midScreenX && y < midScreenY) return 1;
  if (x >= midScreenX && y < midScreenY) return 2;
  if (x < midScreenX && y >= midScreenY) return 3;
  if (x >= midScreenX && y >= midScreenY) return 4;
  return 0;
}