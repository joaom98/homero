void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
}

void loop() {
  float c = 12.34;
  Serial2.println(c);
  delay(100);
}
