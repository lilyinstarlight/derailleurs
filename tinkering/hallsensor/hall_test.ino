#define HALLPIN P1_5

int revs;
int count; 
unsigned long oldtime;
unsigned long average;
int rpm[5];
int hallRead;
int switched;
void magnet_detect();

void setup()
{
    Serial.begin(9600);
    //Pull down to start
    pinMode(HALLPIN,INPUT_PULLDOWN);

    //initialize variables
    switched = 0;
    revs = 0;
    oldtime = 0;
    average = 0;
    count = 0;
}

void loop()
{
    hallRead = digitalRead(HALLPIN);

    //Using moving average to calculate RPM, I don't think it is totally correct
    if(millis() - oldtime > 1000)
    {
        average -= average / 5;
        average += (revs*30) / 5;

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

