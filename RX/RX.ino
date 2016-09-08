#include <cc1101.h>

#define pksize 61

unsigned long rxlast;

byte rxbuf[pksize] = {0};
byte rxsize;

char strbuf[pksize + 1] = {'\0'};

unsigned int i;

void setup() {
  // start computer communication
  Serial.begin(9600);

  // wait for everything to become ready
  delay(100);

  // initialize radio communication
  Radio.Init();
  Radio.SetDataRate(4);
  Radio.SetLogicalChannel(1);

  // remember current processor time
  rxlast = millis();

  // turn on radio
  Radio.RxOn();
}

void loop() {
  // if we received radio data
  if(Radio.CheckReceiveFlag()) {
    // mark this as the last received time
    rxlast = millis();

    // receive data into buffer
    rxsize = Radio.ReceiveData(rxbuf);

    // copy buffer as a string
    for (i = 0; i < rxsize; i++)
      strbuf[i] = (char)rxbuf[i];
    strbuf[i] = '\0';

    // print a timestamp header
    Serial.print("[");
    Serial.print(rxlast);
    Serial.print("] ");

    // print whole buffer as a string
    Serial.println(strbuf);

    // turn radio back on
    Radio.RxOn();
  }
}
