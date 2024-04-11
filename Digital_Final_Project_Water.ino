int solenoidPin = 35;

void setup() {
  pinMode(solenoidPin, OUTPUT);
}

void loop() {
  digitalWrite(solenoidPin, HIGH);
  delay(500);
  digitalWrite(solenoidPin, LOW);
  delay(500);
}
