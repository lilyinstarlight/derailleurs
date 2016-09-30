#define ONE_BUTTON P1_1
#define ZERO_BUTTON P4_5

const uint16_t code = 0b10100111001;
const byte size = 11;

const unsigned int debounce_time = 40; // ms

byte code_idx = 0;

enum State { LOCKED, PARTIAL, UNLOCKED };

State state = LOCKED;

typedef void (*StateFunc)();

void change(State next);

void locked() {
	digitalWrite(RED_LED, HIGH);
	digitalWrite(GREEN_LED, LOW);

	code_idx = 0;
}

void partial() {
	code_idx++;
}

void unlocked() {
	digitalWrite(RED_LED, LOW);
	digitalWrite(GREEN_LED, HIGH);
}

StateFunc funcs[] = {locked, partial, unlocked};

void change(State next) {
	state = next;

	funcs[next]();
}

unsigned long last_one = 0;
unsigned long last_zero = 0;

void enter(int bit) {
	if (state == UNLOCKED) {
		change(LOCKED);
	}
	else {
		if (bitRead(code, size - 1 - code_idx) != bit) {
			change(LOCKED);
		}
		else {
			change(PARTIAL);
		}

		if (code_idx == size) {
			change(UNLOCKED);
		}
	}
}

void enter_one() {
	unsigned long time = millis();
	if (time - last_one > debounce_time) {
		last_one = time;

		enter(1);
	}
}

void enter_zero() {
	unsigned long time = millis();
	if (time - last_zero > debounce_time) {
		last_one = time;

		enter(0);
	}
}

void setup() {
	pinMode(RED_LED, OUTPUT);
	pinMode(GREEN_LED, OUTPUT);

	pinMode(ONE_BUTTON, INPUT_PULLUP);
	pinMode(ZERO_BUTTON, INPUT_PULLUP);

	attachInterrupt(ONE_BUTTON, enter_one, RISING);
	attachInterrupt(ZERO_BUTTON, enter_zero, RISING);

	change(LOCKED);
}

void loop() {
}
