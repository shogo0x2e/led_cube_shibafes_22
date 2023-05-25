#include <TimedAction.h>

#define SUM_OF_FRAMES 4
#define NUM_LEDS 5

#define SER   10
#define RCLK   9
#define SRCLK  8

bool has_written = false;
uint32_t pattern_2d[SUM_OF_FRAMES][NUM_LEDS] = {0};// = {
//   { 
//     0xffffff80,
//     0xffffff80,
//     0xffffff80,
//     0xffffff80,
//     0xffffff80
//   },
//   { 
//     0x00000000,
//     0x039ce000,
//     0x039ce000,
//     0x039ce000,
//     0x00000000
//   },
//   { 
//     0x00000000,
//     0x00000000,
//     0x00080000, // まんなかだけ
//     0x00000000,
//     0x00000000
//   },
//   { 
//     0x00000000,
//     0x039ce000,
//     0x039ce000,
//     0x039ce000,
//     0x00000000
//   }
// };

// うごいた
// void setup() {
//   Serial.begin(9600);
//   pinMode(LED_BUILTIN, OUTPUT);
// }

// void loop() {
//   if (Serial.available() > 0) {
//     byte data = Serial.read();
//     if (data == 0xaa) {
//       while(1) {
//         digitalWrite(LED_BUILTIN, HIGH);  // LEDを点灯
//         delay(500);
//         digitalWrite(LED_BUILTIN, LOW);  // LEDを点灯
//         delay(500);
//       }
//     }
//   }
// }

uint16_t current_frame = 0;

void cube_invalidate() {

  if (++current_frame >= 4) {
    current_frame = 0;
  }
}

auto invalidator = TimedAction(500, cube_invalidate);

void setup() {
  // put your setup code here, to run once:

  pinMode(SER,   OUTPUT);
  pinMode(RCLK,  OUTPUT);
  pinMode(SRCLK, OUTPUT);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.begin(9600);
}

void loop() {

  static int frameIndex = 0;
  static int ledIndex = 0;

  if (Serial.available() >= sizeof(uint32_t)) {
    while (1) {
      digitalWrite(LED_BUILTIN, HIGH);
    }
  }

  if (!has_written && Serial.available() >= sizeof(uint32_t)) {

    pattern_2d[frameIndex][ledIndex] = readInt() & 0xffffff80;
    
    ledIndex++;
    if (ledIndex >= NUM_LEDS) {
      ledIndex = 0;
      frameIndex++;
      if (frameIndex >= SUM_OF_FRAMES) {
        Serial.println("ok");  // 全データの受信が完了したことを通知
        has_written = true;
      }
    }
  }

  if (has_written) {
    invalidator.check();
    print(pattern_2d[current_frame]);
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
