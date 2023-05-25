// うごいた
void setup() {
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  if (Serial.available() >= sizeof(uint32_t)) {
    uint32_t data = readInt();
    if (data == 0xffffff80) {
      digitalWrite(LED_BUILTIN, HIGH);
      while(1);
    }
  }
}

/**
 * Serial で uint32_t を読み込む
 */
uint32_t readInt() {
  uint32_t value = 0;
  value |= ((uint32_t)Serial.read()) << 24;
  value |= ((uint32_t)Serial.read()) << 16;
  value |= ((uint32_t)Serial.read()) << 8;
  value |= (uint32_t)Serial.read();
  return value;
}