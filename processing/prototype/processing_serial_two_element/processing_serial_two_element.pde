import processing.serial.*;

Serial myPort;

int[] pattern_2d = {
  0xffffff80,
  0xfcfcfc80,
  0xffffff80,
  0xfcfcfc80,
  0xffffff80
};

// うごいた
void setup() {
  myPort = new Serial(this, Serial.list()[0], 9600);
}

void draw() {
  if (mousePressed) {
    
    for (int i = 0; i < 5; i++) {
      writeInt(myPort, pattern_2d[i]);
    }
  }
}

void writeInt(Serial port, int value) {
  port.write((value >> 24) & 0xFF);
  port.write((value >> 16) & 0xFF);
  port.write((value >> 8) & 0xFF);
  port.write(value & 0xFF);
  
  delay(500);
}
