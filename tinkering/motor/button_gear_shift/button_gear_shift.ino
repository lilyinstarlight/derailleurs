/* This program will shift up or down one gear using the on-board 
buttons */

// include built in stepper library
#include <Stepper.h>

// define button pins
#define P1 P4_5
#define P2 P1_1

// define enable pins
#define enable_1 18
#define enable_2 19

// specify how many steps the motor has for one revolution
const int stepsPerRevolution = 200;

// initalize the motor
Stepper myStepper(stepsPerRevolution, 12,13,5,9);

// set steps per shift
int shift = 500;

short up;
short down;


void setup() {
  // set the speed at 60 rpm:
  myStepper.setSpeed(60);

  // initialize the serial port:
  Serial.begin(9600);

  // Enable pin pull-up for the buttons
  pinMode(P4_5, INPUT_PULLUP);
  pinMode(P1_1, INPUT_PULLUP);

  // Set modes for enable pins
  pinMode(enable_1, OUTPUT);
  pinMode(enable_2, OUTPUT);

}

void loop() {

  // Read button pins  
  up = digitalRead(P1);
  down = digitalRead(P2);

  // Enable or disable motor
  if (up == 0 || down == 0) {
    digitalWrite(enable_1, HIGH);
    digitalWrite(enable_2, HIGH);
  }
  else {
    digitalWrite(enable_1, LOW);
    digitalWrite(enable_2, LOW);
  }

  // Turn motor if a button is pressed
  if (up == 0) {
    myStepper.step(shift);
  }
  
  if (down == 0) {
    myStepper.step(-shift);
  }
  
}
