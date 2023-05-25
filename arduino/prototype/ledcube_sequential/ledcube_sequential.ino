#include <TimedAction.h>

#define SER   10
#define RCLK   9
#define SRCLK  8

void led_cube_toggle();

TimedAction iterate_pattern = TimedAction(50, led_cube_toggle);

void setup() {
  // put your setup code here, to run once:

  pinMode(SER,   OUTPUT);
  pinMode(RCLK,  OUTPUT);
  pinMode(SRCLK, OUTPUT);

  Serial.begin(9600);

}

uint8_t cnt = 7;
uint8_t layer = 2;

void loop() {

  // 時間経過してたら pat を更新
  iterate_pattern.check();

  // pat を表示
  test_display((uint32_t)1 << cnt | 1 << layer);
}

// pattern を更新
void led_cube_toggle() {

  if (++cnt >= 32) {
    cnt = 7;
    if (++layer >= 7) {
      layer = 3;
    }
  }
}

void test_display(uint32_t pattern) {

  shiftOut(SER, SRCLK, LSBFIRST, (pattern        & 0xfc));
  shiftOut(SER, SRCLK, LSBFIRST, (pattern >>  8) & 0xff);
  shiftOut(SER, SRCLK, LSBFIRST, (pattern >> 16) & 0xff);
  shiftOut(SER, SRCLK, LSBFIRST, (pattern >> 24) & 0xff);
  digitalWrite(RCLK, LOW);
  digitalWrite(RCLK, HIGH);
}
