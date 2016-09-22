#include "MySerial.h"

unsigned long errors = 0;

void setup() {
	MySerial.init();
	Serial.begin(9600);
}

void loop() {
	if (MySerial.available()) {
		byte data = MySerial.read();
		if (!MySerial.check(data))
			errors++;
		Serial.print((char)data);
	}

	if (Serial.available()) {
		String str = Serial.readString();

		if (str == "stat") {
			Serial.println();
			Serial.print("Errors: ");
			Serial.println(errors);
			Serial.println();
		}
		else {
			int ch;
			for (ch = 0; str[ch] != '\0'; ch++)
				MySerial.write(str[ch]);
			MySerial.write('\n');
		}
	}
}
