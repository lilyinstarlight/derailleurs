/*
 * Mackenzie Binns
 *
 * Hall Effect Sensor Module 
 *
 * This code is used for a Latching hall effect sensor to calculate
 * The RPM of a bike being pedaled
 *
 * Version: 1.1
*/

#define NUMMAG 4
#define RPMSIZE 5
#define HALLPIN P1_5

unsigned long oldtime;
double average;

int rps[RPMSIZE];
int rpsidx;
int mag_count;
int hallRead;
int switched;
int target;

void calc_average();
void shift_up();
void shift_down();
void check_cadence();

void setup()
{
    Serial.begin(9600);

    //Pull up to start
    pinMode(HALLPIN,INPUT_PULLUP);
    pinMode(P1_4,OUTPUT);
    digitalWrite(P1_4,HIGH);

    //initialize variables
    switched = 0;
    mag_count = 0;
    oldtime = 0;
    average = 0;
    target = 0;

    for (rpsidx = RPMSIZE - 1; rpsidx >= 0; rpsidx--)
        rps[rpsidx] = 0;
}

void loop()
{
    hallRead = digitalRead(HALLPIN);

    // only reads once a second
    if(millis() - oldtime > 1000)
    {
        calc_average();
        check_cadence();
    }

    // check what state the hall pin is in
    // and calculate revolutions
    if(hallRead == 1 && switched == 0)
    {
        //TODO remove debug statement
        Serial.print("HallPin State: HIGH\n");
        mag_count++;
        switched = 1;
    }else if(hallRead == 0 && switched == 1)
    {
        //TODO remove debug statement
        Serial.print("HallPin State: LOW\n");
        mag_count++;
        switched = 0;
    }
}

void calc_average()
{
    rps[rpsidx] = mag_count;

    // increase the index until you reace 5 then reset to zero
    rpsidx = (rpsidx + 1) % RPMSIZE;

    //calculate the new moving average
    average = 0;
    for (int idx = 0; idx < RPMSIZE; idx++)
        average += rps[idx];
    
    //Calculate RPM from the RPS moving average
    average /= RPMSIZE;
    average *= 60;
    average /= NUMMAG;
    
    //Reset oldtime to calculate the next second
    oldtime = millis();

    //Reset mag_count for the next pass
    mag_count = 0;
     
    //TODO remove debug statement
    Serial.println(average,DEC);
}

void check_cadence()
{
    if((target - average) >= 10)
    {
        shift_down();
    }
    if((average - target) >= 10)
    {
        shift_up();
    }
}

void shift_up()
{
    //TODO send signal to motor board
    Serial.println("SHIFT UP");
}

void shift_down()
{
    //TODO send signal to motor board
    Serial.println("SHIFT DOWN");
}
