/*
 * Mackenzie Binns
 *
 * Hall Effect Sensor Module testing framework
 *
 * This code is used for a Latching hall effect sensor to calculate
 * The RPM of a bike being pedaled and to test the correctness of 
 * output
 *
 * Version: 1.1
*/

#define RPMSIZE 5
#define HALLPIN P1_5

void setup()
{
    Serial.begin(9600);

    //Pull up to start
    pinMode(HALLPIN,INPUT_PULLUP);
    pinMode(P1_4,OUTPUT);
    digitalWrite(P1_4,HIGH);

    //initialize variables
    switched = 0;
    revs = 0;
    oldtime = 0;
    average = 0;

    for (rpmidx = RPMSIZE - 1; rpmidx >= 0; rpmidx--)
        rpm[rpmidx] = 0;
}
