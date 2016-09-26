#include "MySerial.h"

unsigned long errors = 0;

MySerial mySerial(P1_4, P1_5);

void setup() {
	mySerial.begin();
	Serial.begin(9600);
}

void loop() {
	if (mySerial.available()) {
		byte data = mySerial.read();
		if (!mySerial.check(data))
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
				mySerial.write(str[ch]);
			mySerial.write('\n');
		}
	}
}
