#include <Stepper.h>

#include <cc1101.h>

#include <constants.h>

#define BUTTON1 P4_5
#define BUTTON2 P1_1

#define ENABLE1 P3_0
#define ENABLE2 P1_2

// specify how many steps the motor has for one revolution
const int stepsPerRevolution = 200;

// initalize the motor
Stepper gearStepper(stepsPerRevolution, 12, 13, 5, 9);

// set steps per shift
int shift = 355;

// data transfer values
radio_t radio_data;
shift_t shift_data;
gear_t gear_data;

// set variables
bool up = false;
bool down = false;

// initalize gear count
int gear = 8;
int gear_number = 8;

// function
void enable();
void disable();
void shift_up();
void shift_down();
void send_gear();


void setup() {
  // set the speed at 60 rpm
  gearStepper.setSpeed(60);

  // initialize the serial port
  Serial.begin(9600);

  // initialize radio
  Radio.Init();
  Radio.SetDataRate(4);
  Radio.SetLogicalChannel(1);
  Radio.RxOn();

  // enable pin pull-up for the buttons
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);

  // set modes for enable pins
  pinMode(ENABLE1, OUTPUT);
  pinMode(ENABLE2, OUTPUT);

}

void loop() {
  // receive potential radio data
  if (Radio.CheckReceiveFlag()) {
    // get general radio data
    Radio.ReceiveData((byte *)&radio_data);

    // ensure this node is the intended recipient
    if (radio_data.header == SWITCHER_HEADER) {
      // decode data into structure
      shift_data = radio_data.shift;

      // shift according to structure
      up = shift_data.up;
      if (up)
        Serial.println("Shift Up");
      down = shift_data.down;
      if (down)
        Serial.println("Shift Down");
    }
    else {
      // no radio data for us
      up = false;
      down = false;
    }

    // wait for next radio packet
    Radio.RxOn();
  }
  else {
    // no radio data for us
    up = false;
    down = false;
  }

  // read button pins
  if (digitalRead(BUTTON1) == LOW)
    up = true;
  if (digitalRead(BUTTON2) == LOW)
    down = true;

  // turn motor if a button is pressed
  if (up) {
    if (gear < gear_number) {
      enable();
      shift_up();
      disable();
      gear++;
    }
    send_gear();
    Serial.print("Gear: ");
    Serial.println(gear);
  }

  if (down) {
    if (gear > 1) {
      enable();
      shift_down();
      disable();
      gear--;
    }
    send_gear();
    Serial.print("Gear: ");
    Serial.println(gear);
  }
}

void enable() {
  Serial.println("Enabled");
  digitalWrite(ENABLE1, HIGH);
  digitalWrite(ENABLE2, HIGH);
}

void disable() {
  Serial.println("Disabled");
  digitalWrite(ENABLE1, LOW);
  digitalWrite(ENABLE2, LOW);
}

void shift_up() {
  gearStepper.step(shift);
  Serial.print("Shifted ");
  Serial.println(shift);
}

void shift_down() {
  gearStepper.step(-shift);
  Serial.print("Shifted ");
  Serial.println(-shift);
}

void send_gear() {
  // prepare a gear structure
  gear_data.gear = gear;

  //stuff structure into a radio packet
  radio_data.header = SWITCHER_HEADER;
  radio_data.gear = gear_data;

  // send data and put radio back in receiving mode
  Radio.SendData((byte *)&radio_data, sizeof(radio_data));
  Radio.RxOn();
}
