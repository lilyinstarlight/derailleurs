#include <string.h>

#include <SPI.h>

byte chr;
char buf[100];
byte idx = 0;

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

  // get character
  chr = SPI.transfer('\0');

  if (chr == '\0')
    // ignore null bytes
    return;
  else if (chr == '\n') {
    // terminate buffer
    buf[idx] = '\0';

    // reset buffer index
    idx = 0;

    // send acknowledgement
    SPI.transfer('a');

    // follow command
    if (strcmp(buf, "blink") == 0) {
      digitalWrite(RED_LED, HIGH);
      delay(100);
      digitalWrite(RED_LED, LOW);
    }
  }
  else {
    // store character
    buf[idx] = chr;
    idx++;
  }

  // disable slave
  digitalWrite(SS, HIGH);
}
