
#define LED P1_3
#define Sensor P1_4

float reading;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  pinMode(LED, OUTPUT);

  pinMode(Sensor, INPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly: 

  // Turn on LED
  digitalWrite(LED, HIGH);

  // Read sensor
  int i;
  for (i=1;i<10;i++) {
    reading = analogRead(Sensor);
    Serial.print(reading);
    Serial.print(",\n");
    delay(100);
  }

  // Turn on LED
  digitalWrite(LED, LOW);

  for (i=1;i<10;i++) {
    reading = analogRead(Sensor);
    Serial.print(reading);
    Serial.print(",\n");
    delay(100);
  }
  
}
