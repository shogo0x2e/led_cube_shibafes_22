import processing.serial.*;

Serial myPort;

final int SUM_OF_FRAMES = 4;
final int NUM_LEDS = 5;

int [][] pattern = {
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


// うごいた
void setup() {
  myPort = new Serial(this, Serial.list()[0], 9600);
}

void draw() {
  if (mousePressed) {
    writeInt(myPort, 0xffffff80);
    myPort.write(0xaa);  // データを送信
  }
}

void writeInt(Serial port, int value) {
  port.write((value >> 24) & 0xFF);
  port.write((value >> 16) & 0xFF);
  port.write((value >> 8) & 0xFF);
  port.write(value & 0xFF);
}
