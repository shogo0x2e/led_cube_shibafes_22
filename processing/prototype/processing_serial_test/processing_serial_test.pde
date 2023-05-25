import processing.serial.*;

Serial myPort;  // シリアルポート
int[] values = {1000, 2000, 3000, 4000}; // 送信する値の配列
int numValues = values.length; // 送信する値の数

void setup() {
  // シリアルポートの初期化
  myPort = new Serial(this, Serial.list()[0], 9600);
}

void draw() {
  
  for (int i = 0; i < numValues; i++) {
    // 各値を4つのバイトに分割
    byte[] bytes = new byte[4];
    bytes[0] = (byte) (values[i] >> 24);
    bytes[1] = (byte) (values[i] >> 16);
    bytes[2] = (byte) (values[i] >> 8);
    bytes[3] = (byte) values[i];

    // 4つのバイトをシリアルに書き込む
    
    for (int j = 0; j < 4; j++) {
      myPort.write(bytes[j]);
      delay(100);
    }
  }
  
  while(true);

}
