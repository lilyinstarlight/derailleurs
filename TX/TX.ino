#include <cc1101.h>

#define pksize 61

const char * strbuf = "Test Message";

byte txbuf[pksize] = {0};
byte txsize;

unsigned long txnum = 0;

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
  Radio.SetMaxPacketLength(pksize);
  Radio.SetTxPower(0);

  // put strbuf into txbuf
  for (i = 0; i < strbuf[i] != '\0'; i++)
    txbuf[i] = (byte)strbuf[i];
  txsize = i;
}

void loop() {
  // send payload
  Radio.SendData(txbuf, txsize);

  // tell computer we transmitted
  Serial.print("Transmit #");
  Serial.print(txnum);
  Serial.println(":");
  Serial.println(strbuf);

  // increment number
  txnum++;

  // wait for next transmission
  delay(100);
}
