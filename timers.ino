#include "config.h"
void updateTimerDisplay(bool forceUpdate) {
  static unsigned long lastUpdateTime = 0;
  if (!forceUpdate && (millis() - lastUpdateTime < 1000)) return;
  lastUpdateTime = millis();
  unsigned long elapsed = millis() - startTime;
  if (elapsed >= totalTime) {
      displayFeedbackScreen();
      return;
  }
  unsigned long remaining = totalTime - elapsed;
  int minutes = remaining / 60000;
  int seconds = (remaining % 60000) / 1000;
  // tft.fillRect(60, 100, 200, 50, TFT_NAVY);
  tft.setTextSize(5);
  if (isPaused) {
    tft.setTextColor(TFT_RED, TFT_NAVY);
  } else {
    tft.setTextColor(TFT_WHITE, TFT_NAVY);
  }
  tft.setCursor(80, 110);
  tft.printf("%02d:%02d", minutes, seconds);
}

void updateBreakTimerDisplay(bool forceUpdate) {
  static unsigned long lastUpdateTime = 0;
  if (!forceUpdate && (millis() - lastUpdateTime < 1000)) return;
  lastUpdateTime = millis();
  unsigned long elapsed = millis() - startTime;
  if (elapsed >= totalTime) {
      displayIdleScreen();
      return;
  }
  unsigned long remaining = totalTime - elapsed;
  int minutes = remaining / 60000;
  int seconds = (remaining % 60000) / 1000;
  // tft.fillRect(60, 100, 200, 50, TFT_BLUE);
  tft.setTextSize(5);
  if (isPaused) {
    tft.setTextColor(TFT_RED, TFT_BLUE);
  } else {
    tft.setTextColor(TFT_WHITE, TFT_BLUE);
  }
  tft.setCursor(80, 110);
  tft.printf("%02d:%02d", minutes, seconds);
}

void flashButton(int quadrant) {
  if (quadrant != 1 && quadrant != 2) return;
  int x_rect, y_rect = 45, w = 100, h = 30;
  int x_text, y_text = 50;
  const char* text;
  if (quadrant == 1) { x_rect = 25; x_text = 30; text = "+30 Min"; } 
  else { x_rect = 195; x_text = 200; text = "-30 Min"; }
  tft.fillRect(x_rect, y_rect, w, h, TFT_CYAN);
  tft.setTextColor(TFT_BLACK, TFT_CYAN);
  tft.setTextSize(2);
  tft.setCursor(x_text, y_text);
  tft.print(text);
  delay(100);
  tft.fillRect(x_rect, y_rect, w, h, TFT_NAVY);
  tft.setTextColor(TFT_WHITE, TFT_NAVY);
  tft.setTextSize(2);
  tft.setCursor(x_text, y_text);
  tft.print(text);
}