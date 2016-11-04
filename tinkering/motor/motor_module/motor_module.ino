/*
Current motor module code. Operated by button instead of radio 
signal. 
*/


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

  // Turn motor if a button is pressed
  if (up == 0) {
    if (gear < 7) {
      enable();
      shift_up();
      disable();
      gear ++;
    }
  }
  
  if (down == 0) {
    if (gear > 1) {
      enable();
      shift_down();
      disable();
      gear --;
    }
  }
  
}


// supporting functions

void enable() {
  digitalWrite(enable_1, HIGH);
  digitalWrite(enable_2, HIGH);
}

void disable() {
  digitalWrite(enable_1, LOW);
  digitalWrite(enable_2, LOW);
}

void shift_up() {
  myStepper.step(shift);
}

void shift_down() {
  myStepper.step(-shift);
}
