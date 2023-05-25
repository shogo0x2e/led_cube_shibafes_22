const int numValues = 4; // 受信する値の数
int values[numValues]; // 受信する値の配列
bool allReceived = false;

int okPattern[numValues] = {1000, 2000, 3000, 4000};

void setup() {
  // シリアル通信の開始
  Serial.begin(9600);
  
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  if (Serial.available() >= 4 * numValues) {
    for (int i = 0; i < numValues; i++) {

      // 4バイト読み込む
      byte bytes[4];
      for (int j = 0; j < 4; j++) {
        bytes[j] = Serial.read();
      }

      // 4つのバイトを一つのintに結合
      values[i] = ((int)bytes[0] << 24) |
                  ((int)bytes[1] << 16) |
                  ((int)bytes[2] << 8) |
                  (int)bytes[3];
    }
  }

  for (int i = 0; i < numValues; i++) {
    if (values[i] != okPattern[i]) {
      break;
    }
    allReceived = true;
  }

  if (allReceived) {
    digitalWrite(LED_BUILTIN, HIGH);
  }
}
