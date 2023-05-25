
#define LAYER_COUNT 5

#define MAX_LEN 64

char pattern_2d[4][LAYER_COUNT];

uint32_t check[][LAYER_COUNT] = {
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

// 受け取ったデータを保存する配列
char received[MAX_LEN];
// 次に文字を保存するインデックス
int idx = 0; 

int a2d_frame = 0;
int a2d_layer = 0;

void record_to_2darray() {
  
  if (++a2d_layer / LAYER_COUNT >= 1) {
    ++a2d_frame;
    a2d_layer = 0;
  }
  else {
    a2d_layer++;
  }
}

uint32_t hexStringToUInt32(String hexString) {
  char charBuf[12];
  hexString.toCharArray(charBuf, 12);
  return strtoul(charBuf, NULL, 16);
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // Serialデータが利用可能な場合
  while(Serial.available() > 0) {
    // 1文字読み込む
    char c = Serial.read();
    // 改行を受け取ったとき（つまりデータの終了）
    if (c == '\n') {

      // 文字列の終わりを示すNULL文字をセット
      received[idx] = '\0'; 

      pattern_2d[a2d_layer][a2d_frame] = hexStringToUInt32(received);
      record_to_2darray();

      idx = 0; // インデックスをリセット

      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < LAYER_COUNT; j++) {
          if (check[i][j] != pattern_2d[i][j]) {
            return;
          }          
        }
      }

    } else if (idx < MAX_LEN - 1) { // 配列の境界を超えないようにする
      received[idx++] = c; // 読み込んだ文字を配列に追加
    }
  }
}
