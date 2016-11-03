#define RPMSIZE 5
#define HALLPIN P1_5

int revs;
unsigned long oldtime;
unsigned int average;
int rpm[RPMSIZE];
int rpmidx;
int hallRead;
int switched;

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

void loop()
{
    hallRead = digitalRead(HALLPIN);

    if(millis() - oldtime > 1000)
    {
        rpm[rpmidx] = revs;

        rpmidx = (rpmidx + 1) % RPMSIZE;

        average = 0;
        for (int idx = 0; idx < RPMSIZE; idx++)
            average += rpm[idx];
        average /= RPMSIZE;

        oldtime = millis();
        revs = 0;
        Serial.println(average,DEC);
    }

    if(hallRead == 1 && switched == 0)
    {
        Serial.print("HallPin State: HIGH\n");
       revs++;
       switched = 1;
    }else if(hallRead == 0 && switched == 1)
    {
       Serial.print("HallPin State: LOW\n");
       revs++;
       switched = 0;
    }
}

