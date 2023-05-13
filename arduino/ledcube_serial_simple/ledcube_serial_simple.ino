#include <TimedAction.h>

#define SUM_OF_FRAMES 4
#define NUM_LEDS 5

#define SER   10
#define RCLK   9
#define SRCLK  8

bool has_written = false;
uint32_t pattern_2d[SUM_OF_FRAMES][NUM_LEDS] = {0};

// うごいた
void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  if (Serial.available() > sizeof(uint32_t)) {
    uint32_t data = readInt();
    if (data == 0xffffff80) {
      // 受信に成功
    }
  }
}

/**
 * Serial で uint32_t を読み込む
 */
uint32_t readInt() {
  uint32_t value = 0;
  value |= ((uint32_t)Serial.read()) << 24;
  value |= ((uint32_t)Serial.read()) << 16;
  value |= ((uint32_t)Serial.read()) << 8;
  value |= (uint32_t)Serial.read();
  return value;
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

  // Serial.println(layer);
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
