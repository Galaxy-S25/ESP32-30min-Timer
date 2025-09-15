#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <FS.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include <Preferences.h>
#include "time.h"

// 와이파이 및 웹서버 관련 라이브러리 (의존성 순서대로 정렬)

#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <WiFiManager.h>

// ----------------- TFT 및 터치 객체 생성 -----------------
TFT_eSPI tft = TFT_eSPI();

Preferences preferences;

// 터치스크린 핀 정의
#define XPT2046_IRQ 36
#define XPT2046_MOSI 32
#define XPT2046_MISO 39
#define XPT2046_CLK 25
#define XPT2046_CS 33

// 터치스크린용 별도 SPI 버스 설정
SPIClass touchscreenSPI = SPIClass(VSPI);
XPT2046_Touchscreen touchscreen(XPT2046_CS, XPT2046_IRQ);

// 화면 해상도 및 시간 상수
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define THIRTY_MINUTES (30UL * 60UL * 1000UL)
#define FIVE_MINUTES (5UL * 60UL * 1000UL)


// 타이머 중앙 터치 무시 영역
#define DEAD_ZONE_X 60
#define DEAD_ZONE_Y 100
#define DEAD_ZONE_W 200
#define DEAD_ZONE_H 50


// 밝기 조절 설정
#define TFT_BL_PIN 21
const int PWM_FREQ = 5000;
const int PWM_RESOLUTION = 8;

// ----------------- 전역 변수 -----------------
unsigned long startTime = 0;
unsigned long totalTime = 0;
unsigned long pauseStartTime = 0;
bool isPaused = false;
int goodCount = 0;
int badCount = 0;
int brightnessLevel = 1;
int selectedQuadrant = 0;
unsigned long lastTouchTime = 0;
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 9*3600;
const int   daylightOffset_sec = 0;


// 프로그램 상태 정의
enum TimerState { STATE_IDLE, RUNNING, SHOW_SCORE, FEEDBACK, BREAK_RUNNING, STATE_SETTINGS, STATE_CONFIRM_EXIT, STATE_CLOCK };
TimerState currentState = STATE_IDLE;
TimerState previousState = STATE_IDLE;

// 자동 복귀 타임아웃 (1분)
#define AUTO_HOME_TIMEOUT (1UL * 60UL * 1000UL)

#define CLOCK_MODE_TIMEOUT (1UL * 60UL * 1000UL)

// ----------------- 모든 함수 선언 -----------------
void displayIdleScreen();
void displayTimerScreen();
void displayScoreScreen();
void displayFeedbackScreen();
void displayBreakTimerScreen();
void displaySettingsScreen();
void displayConfirmExitScreen();
void redrawTimerScreen(bool isBreakTimer);
void drawBrightnessIndicator();
void drawFeedbackButton(int quadrant, bool isHighlighted);
void drawFeedbackText(int quadrant, uint16_t textColor);
void updateTimerDisplay(bool forceUpdate = false);
void updateBreakTimerDisplay(bool forceUpdate = false);
void handleTouch();
void flashButton(int quadrant);
int getQuadrant(int x, int y);
void updateClockDisplay(bool forceUpdate = false);
void displayClockScreen();
#endif