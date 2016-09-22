#ifndef MYSERIAL_H
#define MYSERIAL_H
#include <Arduino.h>

#ifndef RX
#define RX P1_5
#endif
#ifndef TX
#define TX P1_4
#endif

#ifndef CLOCK_MS
#define CLOCK_MS 10
#endif

class MySerialImpl {
	public:
		void init();
		bool available();
		byte parity(byte data);
		bool check(byte data);
		byte read();
		void write(byte data);
};

extern MySerialImpl MySerial;
#endif
