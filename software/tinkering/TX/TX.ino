#include <cc1101.h>

#define pksize 61

byte txbuf[pksize] = {0};
byte txsize = 0;

unsigned long txnum = 0;

int i = 0;

byte transmit = 0;

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
}

void loop() {
  // if character available, parse it
  if (Serial.available()) {
    // read serial byte
    txbuf[i] = (byte)Serial.read();

    // send message if byte is a newline or a full packet
    if (txbuf[i] == (byte)'\n' || i == pksize)
      transmit = 1;
    else
      i++;

    // increment transmission size
    txsize++;
  }

  // if we need to transmit
  if (transmit) {
    // send payload
    Radio.SendData(txbuf, txsize);

    // tell computer we transmitted
    Serial.print("Transmit #");
    Serial.print(txnum);
    Serial.print(": ");
    Serial.write(txbuf, txsize);

    // increment number
    txnum++;

    // wait for next transmission
    delay(100);

    // reset variables
    i = 0;
    txsize = 0;
    transmit = 0;
  }
}
