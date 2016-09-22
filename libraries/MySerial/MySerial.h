#ifndef MYSERIAL_H
#define MYSERIAL_H
#include <Arduino.h>

class MySerial {
	private:
		int rx, tx;

		unsigned long clock_ms;

	public:
		MySerial(int rx_pin=7, int tx_pin=8);

		void begin(unsigned long speed=10);
		bool available();
		byte parity(byte data);
		bool check(byte data);
		byte read();
		void write(byte data);
};
#endif
