//
// 2022 Sep 30; Written by Shogo Kitada
//

/**
 * 流れ:
 * uint8_t → shiftOut 用に分割
 * uint8_t anime[][5][5] = {
  {
    // 1フレーム目
    {
      0b11111000,
      0b11111000,
      0b11111000,
      0b11111000,
      0b11111000
    },
    {
      0b11111000,
      0b11111000,
      0b11111000,
      0b11111000,
      0b11111000
    },
    {
      0b11111000,
      0b11111000,
      0b11111000,
      0b11111000,
      0b11111000
    },
    {
      0b11111000,
      0b11111000,
      0b11111000,
      0b11111000,
      0b11111000
    },
    {
      0b11111000,
      0b11111000,
      0b11111000,
      0b11111000,
      0b11111000
    }
  }
  //2フレーム目以降 …
};
 */

#define SER   10
#define RCLK   9
#define SRCLK  8

uint8_t anime[][5][5] = {
  {
    // 1フレーム目
    {
      0b11111000,
      0b11111000,
      0b11111000,
      0b11111000,
      0b11111000
    },
    {
      0b11111000,
      0b11111000,
      0b11111000,
      0b11111000,
      0b11111000
    },
    {
      0b11111000,
      0b11111000,
      0b11111000,
      0b11111000,
      0b11111000
    },
    {
      0b11111000,
      0b11111000,
      0b11111000,
      0b11111000,
      0b11111000
    },
    {
      0b11111000,
      0b11111000,
      0b11111000,
      0b11111000,
      0b11111000
    }
  }
  //2フレーム目以降 …
};

/**
 * レイヤーi層分のデータを受け取って
 * ShiftOut ようのビット列にして、制御回路に送信
 * 上位ビットと下位ビットが反転する
 *  
 */
void shiftOutByte(uint8_t each_layer[5], int num_layer) {
  uint32_t ctrl = 0;

  ctrl  = each_layer[0] << 24;
  ctrl |= each_layer[1] << 19;
  ctrl |= each_layer[2] << 14;
  ctrl |= each_layer[3] << 9;
  ctrl |= each_layer[4] << 4;

  shiftOut(SER, SRCLK, LSBFIRST, (0xff & ctrl) | 1 << (1 + num_layer));
  
  for (int i = 3; i >= 1; i--) {
    shiftOut(SER, SRCLK, LSBFIRST, 0xff & (ctrl >> 8*i));
  }
  delay(1);
  digitalWrite(RCLK, LOW);
  digitalWrite(RCLK, HIGH);
  digitalWrite(RCLK, LOW);
}
int a[] = {
  0x40,0x20, 0x10, 0x08, 0x04, 0x04, 0x08, 0x10, 0x20, 0x40
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
  
/*
  shiftOut(SER, SRCLK, LSBFIRST, 0b11111000);
  shiftOut(SER, SRCLK, LSBFIRST, 0b10101010);
  shiftOut(SER, SRCLK, LSBFIRST, 0b01010101);
  shiftOut(SER, SRCLK, LSBFIRST, 0x10101010);
  */
  digitalWrite(RCLK, LOW);
  digitalWrite(RCLK, HIGH);
  
  // while(1);
}

void loop() {
/*
  uint8_t layer[5] = {
    0b10000000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b10000000
  };

  for (int i = 0; i < 5; i++) {
    layer[i] = (layer[i] == 0 ? 0b00001000 : layer[i] << 1);
  }

  shiftOutByte(layer, 1);
  delay(500); 
  
  */

  uint8_t pat = 0;
  
  for (int i = 0; i < sizeof(a) / sizeof(int); i++) {
    shiftOut(SER, SRCLK, LSBFIRST, a[i] | 0x80);
    shiftOut(SER, SRCLK, LSBFIRST, 0xff);
    shiftOut(SER, SRCLK, LSBFIRST, 0xff);
    shiftOut(SER, SRCLK, LSBFIRST, 0xff);
    digitalWrite(RCLK, LOW);
    digitalWrite(RCLK, HIGH);
    delay(500);
  }

  pat = pat == 0 ? 0b00001000 : pat << 1;

}
