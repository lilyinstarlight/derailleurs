int flip = 1;

void setup()
{
    Serial.begin(9600);
    pinMode(P1_4, OUTPUT);
    pinMode(P1_3, OUTPUT);
    digitalWrite(P1_4, HIGH);
    digitalWrite(P1_3, LOW);
}

void loop()
{
    if(flip)
    {
        digitalWrite(P1_4, HIGH);
        digitalWrite(P1_3, LOW);
        flip = 0;
    }else
    {
        digitalWrite(P1_3, HIGH);
        digitalWrite(P1_4, LOW);
        flip = 1;
    }
    delay(250);
}
