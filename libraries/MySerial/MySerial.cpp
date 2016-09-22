#include "MySerial.h"

MySerial::MySerial(int rx_pin, int tx_pin) : rx(rx_pin), tx(tx_pin) {
	pinMode(rx, INPUT_PULLUP);
	pinMode(tx, OUTPUT);

	digitalWrite(tx, HIGH);
}


void MySerial::begin(unsigned long speed) {
	clock_ms = speed;
}

bool MySerial::available() {
	return digitalRead(rx) == LOW;
}

byte MySerial::parity(byte data) {
	byte bit = 0;
	byte parity = 0;

	for (bit = 0; bit < 7; bit++)
		parity += (data & (1 << bit)) % 2;

	return parity;
}

bool MySerial::check(byte data) {
	return parity(data) == (data & (1 << 7));
}

byte MySerial::read() {
	byte bit = 0;
	byte data = 0;

	digitalWrite(tx, LOW);

	for (bit = 0; bit < 7; bit++) {
		delay(clock_ms);
		digitalWrite(tx, HIGH);
		delay(clock_ms);
		data |= (digitalRead(rx) << bit);
		digitalWrite(tx, LOW);
	}

	return data;
}

void MySerial::write(byte data) {
	byte bit = 0;
	byte low = 0;

	if (parity(data))
		data |= (1 << 7);
	else
		data ^= (1 << 7);

	digitalWrite(tx, LOW);

	while (bit < 8) {
		if (digitalRead(rx) == LOW) {
			low = 1;
		}
		else if (low && digitalRead(rx) == HIGH) {
			digitalWrite(tx, (data & (1 << bit)));
			bit++;

			low = 0;
		}

		delay(clock_ms);
	}
}
