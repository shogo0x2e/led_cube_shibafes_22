#include <TimedAction.h>

#define SER   10
#define RCLK   9
#define SRCLK  8

uint32_t pattern_2d[] = {
  0xffffff80,
  0xfffff000,
  0xfffe0000,
  0xffc00000,
  0xf8000000
};

void setup() {
  // put your setup code here, to run once:

  pinMode(SER,   OUTPUT);
  pinMode(RCLK,  OUTPUT);
  pinMode(SRCLK, OUTPUT);

  Serial.begin(9600);

}

void loop() {

  // pat を表示
  // test_display(0xffffff80);
  print(pattern_2d);
}

/**
 * 1フレームを描画する
 */
void print(uint32_t pat[5]) {

  static uint8_t layer = 0;

  // pat []を LED CUBE の実態配線と重ね合わせると、
  // pat [] を逆順に読んでかないといけない。
  display_certain_layer(pat[4-layer] | (1 << (layer + 2)));

  // layer を 0 <= layer <= 4 で繰り返す
  if (++layer >= 5) {
    layer = 0;
  }

  Serial.println(layer);
}

/**
 * それぞれのレイヤーを表示する関数
 */
void display_certain_layer(uint32_t pattern) {

  shiftOut(SER, SRCLK, LSBFIRST,  pattern        & 0xfc);
  shiftOut(SER, SRCLK, LSBFIRST, (pattern >>  8) & 0xff);
  shiftOut(SER, SRCLK, LSBFIRST, (pattern >> 16) & 0xff);
  shiftOut(SER, SRCLK, LSBFIRST, (pattern >> 24) & 0xff);
  digitalWrite(RCLK, LOW);
  digitalWrite(RCLK, HIGH);
}
