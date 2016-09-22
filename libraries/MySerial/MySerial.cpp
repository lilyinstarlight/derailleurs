#include "MySerial.h"

void MySerialImpl::init() {
	pinMode(RX, INPUT_PULLUP);
	pinMode(TX, OUTPUT);

	digitalWrite(TX, HIGH);
}

bool MySerialImpl::available() {
	return digitalRead(RX) == LOW;
}

byte MySerialImpl::parity(byte data) {
	byte bit = 0;
	byte parity = 0;

	for (bit = 0; bit < 7; bit++)
		parity += (data & (1 << bit)) % 2;

	return parity;
}

bool MySerialImpl::check(byte data) {
	return parity(data) == (data & (1 << 7));
}

byte MySerialImpl::read() {
	byte bit = 0;
	byte data = 0;

	digitalWrite(TX, LOW);

	for (bit = 0; bit < 7; bit++) {
		delay(CLOCK_MS);
		digitalWrite(TX, HIGH);
		delay(CLOCK_MS);
		data |= (digitalRead(RX) << bit);
		digitalWrite(TX, LOW);
	}

	return data;
}

void MySerialImpl::write(byte data) {
	byte bit = 0;
	byte low = 0;

	if (parity(data))
		data |= (1 << 7);
	else
		data ^= (1 << 7);

	digitalWrite(TX, LOW);

	while (bit < 8) {
		if (digitalRead(RX) == LOW) {
			low = 1;
		}
		else if (low && digitalRead(RX) == HIGH) {
			digitalWrite(TX, (data & (1 << bit)));
			bit++;

			low = 0;
		}

		delay(CLOCK_MS);
	}
}

MySerialImpl MySerial;
