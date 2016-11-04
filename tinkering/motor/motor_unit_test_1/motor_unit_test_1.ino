/* this unit test ensures that the motor module properly recives and
processes a signal. The signal is simulated by the buttons on the 
microcontroller. The response should be the motor turning the proper
amount. 

the serial monitor indicates when a button has been pressed and how
much the motor moves in response. There will be a short delay that
indicates the signal to move is being sent to the motor.

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

  // Enable or disable motor
  if (up == 0 || down == 0) {
    enable();
  }
  else {
    disable();
  }

  // Turn motor if a button is pressed
  if (up == 0) {
    Serial.println("Button pressed - activate shift up");
    shift_up();
  }
  
  if (down == 0) {
    Serial.println("Button pressed - activate shift down");
    shift_down();
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
  Serial.print("Shifted ");
  Serial.println(shift);
}

void shift_down() {
  myStepper.step(-shift);
  Serial.print("Shifted ");
  Serial.println(-shift);
}

