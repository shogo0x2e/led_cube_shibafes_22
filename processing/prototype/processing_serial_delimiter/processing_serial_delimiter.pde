import processing.serial.*;

// アニメーションの定義
// Arduino 側と SUM_OF_FRAMES, NUM_LEDS
// は同期する必要あり！！
final int SUM_OF_FRAMES = 4;
final int NUM_LEDS = 5;

int [][] pattern_2d = {
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
    0x00080000,
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

Serial myPort;

void setup() {
  myPort = new Serial(this, Serial.list()[0], 9600);
  println("Connected to" + Serial.list()[0]);
}

void draw() {
  
  // ウインドウをクリックしたら
  if (mousePressed) {
    writeInt(myPort, 0xAAAAAAAA); // Start of Frame
    
    // 配列の中身を送信
    for(int i = 0; i < SUM_OF_FRAMES; i++) {
      for(int j = 0; j < NUM_LEDS; j++) {
        writeInt(myPort, pattern_2d[i][j]);
      }
    }

    writeInt(myPort, 0xBBBBBBBB); // End of Frame
  }
}

// int を送信する
void writeInt(Serial port, int value) {
  port.write((value >> 24) & 0xFF);
  port.write((value >> 16) & 0xFF);
  port.write((value >> 8) & 0xFF);
  port.write(value & 0xFF);
  delay(500);
}
