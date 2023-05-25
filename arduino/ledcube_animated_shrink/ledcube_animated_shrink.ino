#include <TimedAction.h>

#define NUM_LEDS 5

#define SER   10
#define RCLK   9
#define SRCLK  8

uint32_t pattern_2d[][NUM_LEDS] = {
  { 
    0xffffff80,
    0xffffff80,
    0xffffff80,
    0xffffff80,
    0xffffff80
  },
  { 
    0x00000000,
    0x039ce000,
    0x039ce000,
    0x039ce000,
    0x00000000
  },
  { 
    0x00000000,
    0x00000000,
    0x00080000, // まんなかだけ
    0x00000000,
    0x00000000
  },
  { 
    0x00000000,
    0x039ce000,
    0x039ce000,
    0x039ce000,
    0x00000000
  }
};

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

  Serial.begin(9600);

  
  shiftOut(SER, SRCLK, LSBFIRST, 0xff);
  shiftOut(SER, SRCLK, LSBFIRST, 0xff);
  shiftOut(SER, SRCLK, LSBFIRST, 0xff);
  shiftOut(SER, SRCLK, LSBFIRST, 0xff);
  digitalWrite(RCLK, LOW);
  digitalWrite(RCLK, HIGH);

  delay(5000);

}

void loop() {

  // pat を表示
  // test_display(0xffffff80);
  invalidator.check();
  print(pattern_2d[current_frame]);
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
