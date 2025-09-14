// 1. 메인 화면
void displayIdleScreen() {
  currentState = STATE_IDLE;
  selectedQuadrant = 0;
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  tft.drawFastVLine(SCREEN_WIDTH / 2, 0, SCREEN_HEIGHT, TFT_WHITE);
  tft.drawFastHLine(0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, TFT_WHITE);
  
  tft.setTextSize(3);
  
  int textY1 = (SCREEN_HEIGHT / 4) - 12;
  int textY2 = (SCREEN_HEIGHT * 3 / 4) - 12;

  int timerTextWidth = tft.textWidth("TIMER");
  tft.setCursor((SCREEN_WIDTH / 4) - (timerTextWidth / 2), textY1);
  tft.print("TIMER");

  int scoreTextWidth = tft.textWidth("SCORE");
  tft.setCursor((SCREEN_WIDTH * 3 / 4) - (scoreTextWidth / 2), textY1);
  tft.print("SCORE");

  int breakTextWidth = tft.textWidth("Break");
  tft.setCursor((SCREEN_WIDTH / 4) - (breakTextWidth / 2), textY2);
  tft.print("Break");

  int setTextWidth = tft.textWidth("Setting");
  tft.setCursor((SCREEN_WIDTH * 3 / 4) - (setTextWidth / 2), textY2);
  tft.print("Setting");
}

// 2. 타이머 화면
void displayTimerScreen() {
  currentState = RUNNING;
  isPaused = false;
  startTime = millis();
  totalTime = THIRTY_MINUTES;
  tft.fillScreen(TFT_NAVY);
  tft.setTextColor(TFT_WHITE, TFT_NAVY);
  tft.setTextSize(2);
  tft.setCursor(30, 50); tft.print("+30 Min");
  tft.setCursor(200, 50); tft.print("-30 Min");
  tft.setCursor(40, 180); tft.print("Pause");
  tft.setCursor(220, 180); tft.print("Exit");
  updateTimerDisplay(true);
}

// 3. 점수판 화면
void displayScoreScreen() {
  currentState = SHOW_SCORE;
  tft.fillScreen(TFT_DARKGREEN);
  tft.setTextColor(TFT_WHITE, TFT_DARKGREEN);
  tft.setTextSize(3);
  tft.setCursor(50, 60); tft.printf("Good: %d", goodCount);
  tft.setCursor(50, 120); tft.printf("Bad : %d", badCount);
  tft.setTextSize(2);
  if (previousState == FEEDBACK) {
    tft.setCursor(20, 200); tft.print("Touch to return to selection");
  } else {
    tft.setCursor(60, 200); tft.print("Touch to return");
  }
}

// 4. 피드백 화면
void displayFeedbackScreen() {
  currentState = FEEDBACK;
  selectedQuadrant = 0;
  tft.fillScreen(TFT_MAROON);
  tft.setTextColor(TFT_WHITE, TFT_MAROON);
  tft.setTextSize(3);
  int timeUpTextWidth = tft.textWidth("Time's Up!");
  tft.setCursor((SCREEN_WIDTH - timeUpTextWidth) / 2, 20);
  tft.print("Time's Up!");
  int splitYStart = 60; 
  tft.drawFastVLine(SCREEN_WIDTH / 2, splitYStart, SCREEN_HEIGHT - splitYStart, TFT_WHITE);
  tft.drawFastHLine(0, (SCREEN_HEIGHT + splitYStart) / 2, SCREEN_WIDTH, TFT_WHITE);
  drawFeedbackButton(1, false);
  drawFeedbackButton(2, false);
  drawFeedbackButton(3, false);
  drawFeedbackButton(4, false);
}

// 5. 쉬는 시간 화면
void displayBreakTimerScreen() {
  currentState = BREAK_RUNNING;
  isPaused = false;
  startTime = millis();
  totalTime = FIVE_MINUTES;
  tft.fillScreen(TFT_BLUE);
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  tft.setTextSize(2);
  tft.setCursor(40, 50); tft.print("+5 Min");
  tft.setCursor(210, 50); tft.print("-5 Min");
  tft.setCursor(40, 180); tft.print("Pause");
  tft.setCursor(220, 180); tft.print("Exit");
  updateBreakTimerDisplay(true);
}

// 6. 설정 화면
void displaySettingsScreen() {
  currentState = STATE_SETTINGS;
  tft.fillScreen(TFT_DARKGREY);
  tft.setTextColor(TFT_WHITE, TFT_DARKGREY);
  tft.setTextSize(3);
  tft.setCursor(80, 20);
  tft.print("Brightness");
  tft.drawFastVLine(SCREEN_WIDTH / 2, 60, SCREEN_HEIGHT - 60, TFT_WHITE);
  tft.drawFastHLine(0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, TFT_WHITE);
  tft.setTextSize(5);
  tft.setCursor(65, 75); tft.print("+");
  tft.setCursor(70, 165); tft.print("-");
  tft.setTextSize(3);
  tft.setCursor(210, 170); tft.print("Back");
  drawBrightnessIndicator();
}

// 7. 나가기 확인 창
void displayConfirmExitScreen() {
  currentState = STATE_CONFIRM_EXIT;
  tft.fillRect(50, 60, 220, 120, TFT_DARKGREY);
  tft.drawRect(50, 60, 220, 120, TFT_WHITE);

  tft.setTextColor(TFT_WHITE, TFT_DARKGREY);
  tft.setTextSize(2);
  tft.setCursor(80, 80);
  tft.print("나가시겠습니까?");

  tft.fillRect(60, 120, 90, 40, TFT_DARKGREEN);
  tft.setCursor(85, 132);
  tft.print("아니오");

  tft.fillRect(170, 120, 90, 40, TFT_MAROON);
  tft.setCursor(210, 132);
  tft.print("예");
}

// 8. 타이머 화면 복구
void redrawTimerScreen(bool isBreakTimer) {
  uint16_t bgColor = isBreakTimer ? TFT_BLUE : TFT_NAVY;
  const char* timeUnit = isBreakTimer ? "5" : "30";
  
  tft.fillScreen(bgColor);
  tft.setTextColor(TFT_WHITE, bgColor);
  tft.setTextSize(2);
  tft.setCursor(isBreakTimer ? 40 : 30, 50); tft.printf("+%s Min", timeUnit);
  tft.setCursor(isBreakTimer ? 210 : 200, 50); tft.printf("-%s Min", timeUnit);
  tft.setCursor(40, 180); tft.print(isPaused ? "Resume" : "Pause");
  tft.setCursor(220, 180); tft.print("Exit");

  currentState = previousState;
  
  if (isBreakTimer) {
    updateBreakTimerDisplay(true);
  } else {
    updateTimerDisplay(true);
  }
}

// 9. 밝기 표시기
void drawBrightnessIndicator() {
  int barWidth = 20;
  int barHeight = 20;
  int spacing = 5;
  int startX = 180;
  int startY = 85;
  tft.fillRect(startX - 2, startY - 2, (barWidth + spacing) * 5, barHeight + 4, TFT_DARKGREY);
  for (int i = 0; i < 5; i++) {
    if (i <= brightnessLevel) {
      tft.fillRect(startX + i * (barWidth + spacing), startY, barWidth, barHeight, TFT_YELLOW);
    } else {
      tft.drawRect(startX + i * (barWidth + spacing), startY, barWidth, barHeight, TFT_WHITE);
    }
  }
}

// 10. 피드백 버튼 그리기
void drawFeedbackButton(int quadrant, bool isHighlighted) {
  uint16_t bgColor = isHighlighted ? TFT_CYAN : TFT_MAROON;
  int splitYStart = 60;
  int midY = (SCREEN_HEIGHT + splitYStart) / 2;
  int rectX, rectY, rectW, rectH;
  rectW = SCREEN_WIDTH / 2;
  rectH = (SCREEN_HEIGHT - splitYStart) / 2;
  if (quadrant == 1) { rectX = 0; rectY = splitYStart; }
  else if (quadrant == 2) { rectX = SCREEN_WIDTH / 2; rectY = splitYStart; }
  else if (quadrant == 3) { rectX = 0; rectY = midY; }
  else { rectX = SCREEN_WIDTH / 2; rectY = midY; }
  tft.fillRect(rectX + 2, rectY + 2, rectW - 4, rectH - 4, bgColor);
  drawFeedbackText(quadrant, isHighlighted ? TFT_BLACK : TFT_WHITE);
}

// 11. 피드백 텍스트 그리기
void drawFeedbackText(int quadrant, uint16_t textColor) {
  const char* text;
  switch (quadrant) {
    case 1: text = "GOOD"; break;
    case 2: text = "SCORE"; break;
    case 3: text = "BAD"; break;
    case 4: text = "EXIT"; break;
    default: return;
  }
  uint16_t bgColor = TFT_MAROON;
  if(selectedQuadrant == quadrant && textColor != TFT_WHITE) {
      bgColor = TFT_CYAN;
  }
  int splitYStart = 60;
  int midY = (SCREEN_HEIGHT + splitYStart) / 2;
  int rectX, rectY, rectW, rectH;
  rectW = SCREEN_WIDTH / 2;
  rectH = (SCREEN_HEIGHT - splitYStart) / 2;
  if (quadrant == 1) { rectX = 0; rectY = splitYStart; }
  else if (quadrant == 2) { rectX = SCREEN_WIDTH / 2; rectY = splitYStart; }
  else if (quadrant == 3) { rectX = 0; rectY = midY; }
  else { rectX = SCREEN_WIDTH / 2; rectY = midY; }
  tft.setTextColor(textColor, bgColor);
  tft.setTextSize(3);
  int textWidth = tft.textWidth(text);
  int textX = rectX + (rectW - textWidth) / 2;
  int textY = rectY + (rectH - 24) / 2;
  tft.setCursor(textX, textY);
  tft.print(text);
}