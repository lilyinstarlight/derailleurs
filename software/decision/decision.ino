#include <cc1101.h>

#include <constants.h>

#define MAGS 4
#define RPM_AVG 5
#define HALL P1_5
#define QUESTION P1_4

#define SHIFT_OFFSET 10

// global variable declarations
unsigned long oldtime;
double average;
int rps[RPM_AVG];
int rpsidx;
int mag_count;
int hall_read;
int switched;
int target;
int gear;

// radio types
radio_t radio_data;
shift_t shift_data;
gear_t gear_data;
cadence_t cadence_data;

// function prototypes
void calc_average();
void shift_up();
void shift_down();
void send_interface();

void setup() {
  // initialize serial
  Serial.begin(9600);

  // initialize radio
  Radio.Init();
  Radio.SetDataRate(4);
  Radio.SetLogicalChannel(1);
  Radio.RxOn();

  // pull up to start
  pinMode(HALL, INPUT_PULLUP);
  pinMode(QUESTION, OUTPUT);
  digitalWrite(QUESTION, HIGH);

  // initialize variables
  switched = 0;
  mag_count = 0;
  oldtime = 0;
  average = 0;
  target = -1;
  gear = 1;

  for (rpsidx = RPMSIZE - 1; rpsidx >= 0; rpsidx--)
    rps[rpsidx] = 0;
}

void loop() {
  hall_read = digitalRead(HALLPIN);

  // only reads once a second
  if(millis() - oldtime > 1000) {
    // reset oldtime to calculate the next second
    oldtime = millis();

    calc_average();

    int offset = target - average;

    // pedaling is getting hard shift down to make it easier
    if(offset <= SHIFT_OFFSET)
      shift_down();

    // pedaling is getting easy shift up to make it easier
    if(offset >= SHIFT_OFFSET)
      shift_up();

    // send interface data
    send_interface();
  }

  // check what state the hall sensor is in and calculate revolutions
  if(hallRead == 1 && switched == 0) {
    mag_count++;
    switched = 1;
  }
  else if(hallRead == 0 && switched == 1) {
    mag_count++;
    switched = 0;
  }

  // check for waiting radio messages
  if (Radio.CheckReceiveFlag()) {
    Radio.ReceiveData((byte *)&radio_data);

    if (radio_data.header == SWITCHER_HEADER) {
      gear_data = radio_data.gear;
      gear = gear_data.gear;
    }
    else if (radio_data.header == INTERFACE_HEADER) {
      target_data = radio_data.target;

      if (target_data.up)
        shift_up();

      if (target_data.down)
        shift_down();

      if (target_data.set) {
        if (target > 0)
          target = average;
        else
          target = -1;
      }
    }

    Radio.RxOn();
  }
}

void calc_average() {
  rps[rpsidx] = mag_count;

  // increase the index until you reace 5 then reset to zero
  rpsidx = (rpsidx + 1) % RPM_AVG;

  //calculate the new moving average
  average = 0;
  for (int idx = 0; idx < RPM_AVG; idx++)
    average += rps[idx];

  // calculate RPM from the RPS moving average
  average *= 60;
  average /= RPM_AVG;
  average /= MAGS;

  // reset mag_count for the next pass
  mag_count = 0;

  Serial.print("Cadence is: ");
  Serial.println(average, DEC);
}

void shift_up() {
  shift_data.up = true;
  shift_data.down = false;
  radio_data.header = SWITCHER_HEADER;
  radio_data.shift = shift_data;

  Radio.SendData((byte *)&radio_data, sizeof(radio_data));
  Radio.RxOn();

  Serial.println("Shift Up");
}

void shift_down() {
  shift_data.up = false;
  shift_data.down = true;
  radio_data.header = SWITCHER_HEADER;
  radio_data.shift = shift_data;

  Radio.SendData((byte *)&radio_data, sizeof(radio_data));
  Radio.RxOn();

  Serial.println("Shift Down");
}

void send_interface() {
  interface_data.cadence = average;
  interface_data.gear = gear;
  radio_data.header = SWITCHER_HEADER;
  radio_data.interface = interface_data;

  Radio.SendData((byte *)&radio_data, sizeof(radio_data));
  Radio.RxOn();
}
