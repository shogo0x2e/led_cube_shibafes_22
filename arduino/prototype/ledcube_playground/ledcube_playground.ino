//
// 2022 Sep 30; Written by Shogo Kitada
//

#define SER   10
#define RCLK   9
#define SRCLK  8

int cathode[] = {
  0x40 ,0x20, 0x10, 0x08, 0x04
};

int pattern[][4] = {
  {0x00, 0x00, 0x00, 0x80}
};

uint32_t pattern_2d[] = {
  0xffffff80,
  0xfffff000,
  0xfffe0000,
  0xffc00000,
  0xf8000000
  // 0xfffff000,
  // 0xffff,
  // 0x00000000,
  // 0x00018000
};

void setup()
{  
  pinMode(SER,   OUTPUT);
  pinMode(RCLK,  OUTPUT);
  pinMode(SRCLK, OUTPUT);

  shiftOut(SER, SRCLK, LSBFIRST, 0xff);
  shiftOut(SER, SRCLK, LSBFIRST, 0xff);
  shiftOut(SER, SRCLK, LSBFIRST, 0xff);
  shiftOut(SER, SRCLK, LSBFIRST, 0xff);
  
  digitalWrite(RCLK, LOW);
  digitalWrite(RCLK, HIGH);

  // delay(5000);

  Serial.begin(9600);
}

void loop() {
  
  // static uint8_t pat = 0;
  // static auto now = millis();

  print(pattern_2d);


  // test_display();

  // display(
  //   pattern[pat][0], pattern[pat][1], pattern[pat][2], pattern[pat][3]
  // );
  // // test_display();
  // // delay(500);

  // if (millis() - now >= 500) {
  //   now = millis();
  //   if (++pat >= sizeof(pattern) / sizeof(int[4])) {
  //     pat = 0;
  //   }
  // }

}

void display(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {

  static uint8_t iter = 0;

  shiftOut(SER, SRCLK, LSBFIRST, cathode[iter] | d);
  shiftOut(SER, SRCLK, LSBFIRST, c);
  shiftOut(SER, SRCLK, LSBFIRST, b);
  shiftOut(SER, SRCLK, LSBFIRST, a);
  digitalWrite(RCLK, LOW);
  digitalWrite(RCLK, HIGH);

  if (++iter >= sizeof(cathode) / sizeof(int)) {
    iter = 0;
  }

  Serial.println(iter);
}

void print(uint32_t frame[5]) {

  static uint8_t iter = 0;
  static uint8_t current = 0;

  shiftOut(SER, SRCLK, LSBFIRST, cathode[iter] | (frame[current] & 0x80));
  shiftOut(SER, SRCLK, LSBFIRST, (frame[current] >>  8) & 0xff);
  shiftOut(SER, SRCLK, LSBFIRST, (frame[current] >> 16) & 0xff);
  shiftOut(SER, SRCLK, LSBFIRST, (frame[current] >> 24) & 0xff);
  digitalWrite(RCLK, LOW);
  digitalWrite(RCLK, HIGH);

  if (++iter >= sizeof(cathode) / sizeof(int)) {
    iter = 0;
  }

  if (++current >= 5) {
    current = 0;
  }

  // Serial.println(iter);
  Serial.println(current);
}

void test_display() {

  static uint8_t iter = 0;

  shiftOut(SER, SRCLK, LSBFIRST, cathode[iter] | 0x80);
  shiftOut(SER, SRCLK, LSBFIRST, 0xff);
  shiftOut(SER, SRCLK, LSBFIRST, 0xff);
  shiftOut(SER, SRCLK, LSBFIRST, 0xff);
  digitalWrite(RCLK, LOW);
  digitalWrite(RCLK, HIGH);

  if (++iter >= sizeof(cathode) / sizeof(int)) {
    iter = 0;
  }

  // Serial.println(iter);
  delay(500);
}


