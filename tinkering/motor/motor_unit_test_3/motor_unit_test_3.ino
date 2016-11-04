/*
This is an integrated test to automate the shifting. It is similar
to unit test 2, but instead of being activated by manually pushing
a button, it receives signals to shift from the sensor module. 

The button inputs are replaced by two spare input pins that connect
to the other microcontroller running hall_decision_test.

 */

// include built in stepper library
#include <Stepper.h>

// define signal pins
#define P1 P1_3
#define P2 P1_4

// define enable pins
#define enable_1 18
#define enable_2 19

// specify how many steps the motor has for one revolution
const int stepsPerRevolution = 200;

// initalize the motor
Stepper myStepper(stepsPerRevolution, 12,13,5,9);

// set steps per shift
int shift = 500;

// set variables
short up;
short down;

// initalize count
int gear = 1;

// function prototypes
void enable();
void disable();
void shift_up();
void shift_down();


void setup() {
  // set the speed at 60 rpm:
  myStepper.setSpeed(60);

  // initialize the serial port:
  Serial.begin(9600);
  
  // Enable pin pull-up for the buttons
  pinMode(P4_5, INPUT_PULLUP);
  pinMode(P1_1, INPUT_PULLUP);

  // set modes for enable pins
  pinMode(enable_1, OUTPUT);
  pinMode(enable_2, OUTPUT);
  
}

void loop() {

  // Read button pins  
  up = digitalRead(P1);
  down = digitalRead(P2);

//  // Enable or disable motor
//  if (up == 0 || down == 0) {
//    enable();
//  }
//  else {
//    disable();
//  }

  // Turn motor if a button is pressed
  if (up == 0) {
    if (gear < 7) {
      enable();
      shift_up();
      disable();
      gear ++;
    }
    Serial.print("Gear: ");
    Serial.println(gear);
  }
  
  if (down == 0) {
    if (gear > 1) {
      enable();
      shift_down();
      disable();
      gear --;
    }
    Serial.print("Gear: ");
    Serial.println(gear);
  }
  
}


// supporting functions

void enable() {
  Serial.println("Enabled");
  digitalWrite(enable_1, HIGH);
  digitalWrite(enable_2, HIGH);
}

void disable() {
  Serial.println("Disabled");
  digitalWrite(enable_1, LOW);
  digitalWrite(enable_2, LOW);
}

void shift_up() {
  myStepper.step(shift);
  Serial.print("Shifted ");
  Serial.println(shift);
}

void shift_down() {
  myStepper.step(-shift);
  Serial.print("Shifted ");
  Serial.println(-shift);
}
