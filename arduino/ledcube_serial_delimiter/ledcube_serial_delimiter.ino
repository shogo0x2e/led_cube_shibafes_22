#include <TimedAction.h>

#define SUM_OF_FRAMES 4
#define NUM_LEDS 5

// シフトレジスタのピン
#define SER   10
#define RCLK   9
#define SRCLK  8

// 配列はデータで埋まっているか？
bool has_written = false;

// LED CUBE のアニメーション用
int current_frame = 0;
void invalidate() {
  if (++current_frame >= SUM_OF_FRAMES) {
    current_frame = 0;
  }
}

auto invalidator = TimedAction(500, invalidate);

// アニメーションの配列
uint32_t pattern_2d[SUM_OF_FRAMES][NUM_LEDS];

// 受信時の pattern_2d の先頭となる添え字
int frameIndex = 0;
int ledIndex = 0;

void setup() {
  Serial.begin(9600);

  // デバッグ用
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {

  // End of Frame が送られていたら
  // LED CUBE の描画処理
  if (has_written) {
    
    invalidator.check();
    print(pattern_2d[current_frame]);
    return;
  }

  // Serial のバッファに uint32_t のバイト数 (4 Bytes)
  // だけデータがたまっていたら
  if (Serial.available() > sizeof(uint32_t)) {

    uint32_t data = readInt();

    if (data == 0xAAAAAAAA) { // Start of Frame
      frameIndex = 0;
      ledIndex = 0;
    }
    else if (data == 0xBBBBBBBB) { // End of Frame
      digitalWrite(LED_BUILTIN, LOW);
      has_written = true;
    } 
    else {
      // 配列に読み込んで、index をそれぞれ増分
      pattern_2d[frameIndex][ledIndex] = data;
      ledIndex++;
      if (ledIndex >= NUM_LEDS) {
        ledIndex = 0;
        frameIndex++;
      }
    }
  }
}

/**
 * uint32_t として Serial のバッファを読み出す
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
 * ステート付きで LED CUBE のすべてのレイヤーに
 * ダイナミック点灯する
 */
void print(uint32_t pat[NUM_LEDS]) {
  static uint8_t layer = 0;

  display_certain_layer(pat[4-layer] | (1 << (layer + 2)));

  if (++layer >= NUM_LEDS) {
    layer = 0;
  }
}

/**
 * 一つのレイヤーを表示する
 */
void display_certain_layer(uint32_t pattern) {
  shiftOut(SER, SRCLK, LSBFIRST,  pattern        & 0xfc);
  shiftOut(SER, SRCLK, LSBFIRST, (pattern >>  8) & 0xff);
  shiftOut(SER, SRCLK, LSBFIRST, (pattern >> 16) & 0xff);
  shiftOut(SER, SRCLK, LSBFIRST, (pattern >> 24) & 0xff);
  digitalWrite(RCLK, LOW);
  digitalWrite(RCLK, HIGH);
}

