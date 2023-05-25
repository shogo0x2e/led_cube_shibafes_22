import processing.serial.*;

Serial myPort;

// うごいた
void setup() {
  myPort = new Serial(this, Serial.list()[0], 9600);
}

void draw() {
  if (mousePressed) {
    
    writeInt(myPort, 0xffffff80);
  }
}

void writeInt(Serial port, int value) {
  port.write((value >> 24) & 0xFF);
  port.write((value >> 16) & 0xFF);
  port.write((value >> 8) & 0xFF);
  port.write(value & 0xFF);
}
