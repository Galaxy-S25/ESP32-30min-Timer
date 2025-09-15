#include "config.h"
/**
 * @brief DAC를 이용해 '똑' 소리를 재생하는 최종 함수 (사인파 방식)
 * @param frequency 재생할 소리의 주파수 (Hz)
 * @param duration 재생 시간 (ms)
 * @param volume 소리 크기 (float, 소수점 가능)
 */
void tick(int frequency, int duration, float volume) {
  long endTime = millis() + duration;
  int sampleRate = 8000; // 음질 설정 (높을수록 부드러움)
  
  // 이 루프는 duration 시간 동안만 실행됩니다.
  while (millis() < endTime) {
    // 아주 짧은 시간 동안만 사인파를 재생합니다.
    // for 루프는 음질을 위한 것이며, 전체 소리 길이는 duration에 의해 결정됩니다.
    for (int i = 0; i < 100; i++) { 
      // 사인파(-1 ~ 1) 값에 볼륨(진폭)을 곱해 소리 크기를 조절
      int dacValue = 128 + volume * sin(2.0 * PI * frequency * (i / (float)sampleRate));
      
      dacWrite(DAC_PIN, dacValue);
      delayMicroseconds(1000000 / sampleRate);
    }
  }
  
  // 소리 재생 후 DAC 출력 끄기
  dacWrite(DAC_PIN, 0);
}