long long prev = 0;
long long num = 1;
long long temp = 0;

void setup() {
  pinMode(RED_LED, OUTPUT);     
}

void loop() {
  for (int blink = 0; blink < num; blink++) {
    digitalWrite(RED_LED, HIGH);
    delay(250);
    digitalWrite(RED_LED, LOW);
    delay(250);
  }

  temp = num;
  num = num + prev;
  prev = temp;

  delay(1000);
}
