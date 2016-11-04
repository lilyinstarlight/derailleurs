/*
 * Mackenzie Binns
 *
 * Hall Effect Sensor Module testing framework
 *
 * This code is used for a Latching hall effect sensor to calculate
 * The RPM of a bike being pedaled and to test the correctness of 
 * output through random number generation and a verification function
 *
 * Version: 1.1
*/

#define NUMMAG 4
#define RPMSIZE 5
#define HALLPIN P1_5

unsigned long oldtime;
double average;

int rpm[RPMSIZE];
int rpmidx;
unsigned int revs;
int hallRead;
int switched;

void calc_average();
int speed_test(int magnets);
bool check_average();

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

    //Seed the test for different numbers
    srand(millis());

    for (rpmidx = RPMSIZE - 1; rpmidx >= 0; rpmidx--)
        rpm[rpmidx] = 0;
}

void loop()
{
    hallRead = digitalRead(HALLPIN);

    // only reads once a second
    if(millis() - oldtime > 1000)
    {
        speed_test((rand() % 10)+1);
        calc_average();

        if(check_average())
        {
            Serial.println("Average is correct!");
        }else
        {
            Serial.println("Average is incorrect!");
        }
    }
}

void calc_average()
{
    rpm[rpmidx] = revs;

    //Increase the index until you reace 5 then reset to zero
    rpmidx = (rpmidx + 1) % RPMSIZE;

    //Calculate the new moving average
    average = 0;
    for (int idx = 0; idx < RPMSIZE; idx++)
    {
        average += rpm[idx];
    }
    
    average /= RPMSIZE;
    
    //Sanity check for averages
    Serial.print("Average is: ");
    Serial.println(average);
    
    //Calculate RPM not RPS
    average *= 60;
    average /= NUMMAG;

    oldtime = millis();
    revs = 0;
     
    //TODO remove debug statement
    Serial.println(average,DEC);
}

//Set random number of magnets being detected per-second
int speed_test(int magnets)
{
    revs = abs(magnets);
}

bool check_average()
{
    double verify = 0; 
    for(int i = 0; i < RPMSIZE; i++)
    {
        verify += rpm[i];       
    }

    verify /=RPMSIZE;
    verify *= 60;
    verify /=NUMMAG;

    if(verify == average)
    {
        return true; 
    }else
    {
        Serial.print("Verification is: ");
        Serial.println(verify);
        return false; 
    }
}
