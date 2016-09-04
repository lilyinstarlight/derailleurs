#include <SPI.h>

byte ack;

void setup() {
  // initialize slave select
  pinMode(SS, OUTPUT);
  digitalWrite(SS, HIGH);

  // initialize SPI
  SPI.begin();
}

void loop() {
  // enable slave
  digitalWrite(SS, LOW);

  // send "blink" payload
  for (const char * p = "blink"; *p != '\0'; p++)
    SPI.transfer(*p);

  // send line feed
  SPI.transfer('\n');

  // send null byte to test for acknowledgement
  ack = SPI.transfer('\0');

  // disable slave
  digitalWrite(SS, HIGH);

  // note acknowledgement
  if (ack == 'a')
    digitalWrite(RED_LED, HIGH);
  else
    digitalWrite(RED_LED, LOW);

  // wait between next message
  delay(1000);
}
