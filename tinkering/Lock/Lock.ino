#define ONE_BUTTON P1_1
#define ZERO_BUTTON P4_5

#define MAX_CODE 6

const bool code[] = {1, 0, 0, 1};

const unsigned int debounce_time = 200; // ms

const byte code_size = sizeof(code);
byte code_idx = 0;

enum State {LOCKED, INCORRECT, PARTIAL, UNLOCKED};

State state = LOCKED;

typedef void (*StateFunc)();

void change(State next);

void locked() {
	digitalWrite(RED_LED, HIGH);
	digitalWrite(GREEN_LED, LOW);

	code_idx = 0;
}

void incorrect() {
	code_idx++;
}

void partial() {
	code_idx++;
}

void unlocked() {
	digitalWrite(RED_LED, LOW);
	digitalWrite(GREEN_LED, HIGH);

	code_idx = code_size;
}

StateFunc funcs[] = {locked, incorrect, partial, unlocked};

void change(State next) {
	funcs[next]();

	state = next;
}

unsigned long last_one = 0;
unsigned long last_zero = 0;

void enter(int bit) {
	if (state == UNLOCKED) {
		change(LOCKED);
	}
	else if (state == INCORRECT) {
		change(INCORRECT);

		if (code_idx == MAX_CODE)
			change(LOCKED);
	}
	else {
		if (code[code_size - 1 - code_idx] == bit) {
			change(PARTIAL);

			if (code_idx == code_size) {
				change(UNLOCKED);
			}
		}
		else {
			change(INCORRECT);
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

bool blink_state = false;
unsigned long blink_time = 0;

State last_state = LOCKED;

void loop() {
	if (last_state != state) {
		if (state == LOCKED) {
			digitalWrite(RED_LED, LOW);
			delay(125);
			digitalWrite(RED_LED, HIGH);
			delay(125);
			digitalWrite(RED_LED, LOW);
			delay(125);
			digitalWrite(RED_LED, HIGH);
			delay(125);
			digitalWrite(RED_LED, LOW);
			delay(250);
			digitalWrite(RED_LED, HIGH);
		}

		last_state = state;
	}

	unsigned long time = millis();

	if ((state == PARTIAL || state == INCORRECT) && time > blink_time) {
		digitalWrite(RED_LED, blink_state);

		blink_state = !blink_state;
		blink_time = time + 1000;
	}

	sleep(80);
}
