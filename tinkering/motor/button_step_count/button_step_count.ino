#include <Stepper.h>

#define P1 P4_5
#define P2 P1_1

const int stepsPerRevolution = 200;

Stepper myStepper(stepsPerRevolution, 12,13,5,9);

short forward;
short backward;

int step_count=0;

void setup() {
  // set the speed at 60 rpm:
  myStepper.setSpeed(60);
  
  // initialize the serial port:
  Serial.begin(9600);

  // Enable pin pull-up for the buttons
  pinMode(P4_5, INPUT_PULLUP);
  pinMode(P1_1, INPUT_PULLUP);

  // Set modes for enable pins
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);
}

void loop() {

  // Read button pins  
  forward = digitalRead(P1);
  backward = digitalRead(P2);


  // Enable or disable motor
  if (forward == 0 || backward == 0) {
    digitalWrite(18, HIGH);
    digitalWrite(19, HIGH);
  }
  else {
    digitalWrite(18, LOW);
    digitalWrite(19, LOW);
  }

  // Turn motor if a button is pressed
  if (forward == 0) {
    myStepper.step(1);
    step_count ++;
    Serial.println(step_count);
  }
  
  if (backward == 0) {
    myStepper.step(-1);
    step_count --;
    Serial.println(step_count);
  }

  // Delay before next loop. Determines how fast the motor turns
  delay(4);
  
}
