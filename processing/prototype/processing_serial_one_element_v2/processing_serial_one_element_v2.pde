import processing.serial.*;

final int kFrameCount = 4;
final int kLayerCount = 5;

String[][] pattern2d = {
  {
    "0xffffff80",
    "0xffffff80",
    "0xffffff80",
    "0xffffff80",
    "0xffffff80"
  },
  { 
    "0x00000000",
    "0x039ce000",
    "0x039ce000",
    "0x039ce000",
    "0x00000000"
  },
  { 
    "0x00000000",
    "0x00000000",
    "0x00080000", // まんなかだけ
    "0x00000000",
    "0x00000000"
  },
  { 
    "0x00000000",
    "0x039ce000",
    "0x039ce000",
    "0x039ce000",
    "0x00000000"
  }
};

Serial myPort;

void setup()
{
  // Serial portのリストを出力する
  printArray(Serial.list());

  // 使用するSerial portを指定
  String portName = Serial.list()[0]; // 変更が必要な場合はここを適切に設定

  // 新たなSerial通信を開始
  myPort = new Serial(this, portName, 9600);
}

void draw()
{
  
  for (int i = 0; i < kFrameCount; i++) {
    for (int j = 0; j < kLayerCount; j++) {
      for (int k = 0; k < 10; k++) {
        myPort.write(pattern2d[i][j].charAt(k));
      }
      myPort.write("\n");
      delay(1);
    }
  }
}
