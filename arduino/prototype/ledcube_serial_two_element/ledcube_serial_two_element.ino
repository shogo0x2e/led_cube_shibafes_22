
uint32_t pattern_2d[5] = { 0, 0, 0, 0, 0 };

uint32_t check[5] = {
  
  0xffffff80,
  0xfcfcfc80,
  0xffffff80,
  0xfcfcfc80,
  0xffffff80
};

// うごいた
void setup() {
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {

  static int idx = 0;

  if (Serial.available() >= sizeof(uint32_t)) {

    if (idx >= 5) {
      blink_led(100);
    }

    pattern_2d[idx++] = readInt();

    for (int i = 0; i < 5; i++) {
      if (pattern_2d[i] != check[i]) {
        break;
      }
      blink_led(500);
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

void blink_led(uint32_t duration) {
  
  while (1) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(duration);
    digitalWrite(LED_BUILTIN, LOW);
    delay(duration);
  }
}