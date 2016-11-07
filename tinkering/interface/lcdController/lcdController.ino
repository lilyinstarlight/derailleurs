/*
Philips PCD8544 Controller and interface code.

Basis of library pulled from: http://playground.arduino.cc/Code/PCD8544
Mostly unmodified except for pin mappings in first commit, modified since

Currently displays cadence data to lcd screen.
Pin mapping follows as defined below, with vcc and gnd going directly between the
dev board and the screen, and a 330 ohm resistor between the backlight pin(P1_2), and the screen.
Note that the pin labled SCL* on the dev board is used instead of SCLK for the screen SCLK input.

Run tests by uncommenting the define TEST and calling the runTests() function
Ryan Becwar
*/

#include <SPI.h>
#include <stdlib.h>
#include <libraries/LCD_Functions.h>

//Uncomment if you want to run in test mode
//#define TEST

//Cadence will never go above 200, so 8 bit ints work well
uint8_t targetCadence = 0;
uint8_t currentCadence = 0;
uint8_t gearPos = 0;

volatile int flag = HIGH;

//Writes out current cadence information by overwriting every pixel in the display.
void writeCadence(void) {

  String targetString = String("Target:  ");
  if(targetCadence < 100)
    targetString += " ";
  if(targetCadence < 10)
    targetString += " ";
  targetString += targetCadence;
  char tarBuf[targetString.length() + 1];
  targetString.toCharArray(tarBuf, targetString.length() + 1);

  String currentString = String("Current: ");
  if(currentCadence < 100)
    currentString += " ";
  if(currentCadence < 10)
    currentString += " ";
  currentString += currentCadence;
  char curBuf[currentString.length() + 1];
  currentString.toCharArray(curBuf, currentString.length() + 1);
  
  LcdString(tarBuf);
  LcdString("____________");
  LcdString("            ");
  LcdString(curBuf);
  LcdString("            ");
  LcdString("Gear:      5");

  #ifdef TEST
    Serial.println(tarBuf);
    Serial.println("____________");
    Serial.println("            ");
    Serial.println(curBuf);
    Serial.println("            ");
    Serial.println("Gear:      5");
   #endif
}

/*Functions to handle buttons to increase or decrease target cadence.*/
void bUp(){
  wakeup();
  flag = HIGH;
  targetCadence++;
}

void bDown(){
  wakeup();
  flag = HIGH;
  targetCadence--;
}

uint8_t setCurrent(uint8_t cadence){
  currentCadence = cadence;
  return currentCadence;
}

uint8_t setGear(uint8_t gear){
  if(gear < 1)
    gear = 0;
  if(gear > 8)
    gear = 8;
   gearPos = gear;
    return 0;
}

//Writes output to Serial, always returns true
int testWrite(void){
  writeCadence();
  return 0;
}

//Tests the setCurrent function
int testCurrentAdjust(){
  setCurrent(124);
  if(currentCadence == 124)
    return 0;
  else
    return 1;
}

int testSetGear(){
  setGear(5);
  if(gearPos == 5)
    return 0;
  else
    return 1;
}

int testSetGearTooHigh(){
  setGear(200);
  if(gearPos == 8)
    return 0;
  else
    return 1;
}

//Tests return 0 if pass, 1 if fail
//This should definitley be restructured to use function pointers and iteration, but is used now in interest of time.
void runTests(void){
  Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
  Serial.println("TEST: testWrite");
  Serial.println("OUTPUT:");
  if(!testWrite()){
    Serial.println("PASSED");
  } else {
    Serial.println("FAILED");
  }
  Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
  Serial.println();
  
  Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
  Serial.println("TEST: testCurrentAdjust");
  Serial.println("OUTPUT:");
  if(!testCurrentAdjust()){
    Serial.println("PASSED");
  } else {
    Serial.println("FAILED");
  }
  Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
  Serial.println();
  
  Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
  Serial.println("TEST: testSetGear");
  Serial.println("OUTPUT:");
  if(!testSetGear()){
    Serial.println("PASSED");
  } else {
    Serial.println("FAILED");
  }
  Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
  Serial.println();
  
  Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
  Serial.println("TEST: testSetGearTooHigh");
  Serial.println("OUTPUT:");
  if(!testSetGearTooHigh()){
    Serial.println("PASSED");
  } else {
    Serial.println("FAILED");
  }
  Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
  Serial.println();
}

void setup(void)
{
  Serial.begin(9600);
  //Setup Buttons
  //Enable internal pullup. 
  //These will be changed to the button pins when connected to the board, curretnly represents redboard buttons
  pinMode(PUSH1, INPUT_PULLUP);
  pinMode(PUSH2, INPUT_PULLUP);
  
  attachInterrupt(PUSH1, bUp, FALLING); // Interrupt is fired whenever button is pressed
  attachInterrupt(PUSH2, bDown, FALLING); // Interrupt is fired whenever button is pressed
  
  LcdInitialise();
  LcdClear();
  targetCadence = 75;
  currentCadence = 25;
  writeCadence();
  
  runTests();
}

void loop(void)
{
  suspend();
  if(flag) {
    writeCadence();
    flag = LOW;
  }  
}
