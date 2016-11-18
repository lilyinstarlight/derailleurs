#include <cc1101.h>

#include <constants.h>

#define MAGS 4
#define RPM_AVG 5
#define HALL P1_5

#define SHIFT_OFFSET 10

#define GEAR_COUNT 8

#define MIN_SHIFT 20

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
interface_t interface_data;
target_t target_data;

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

  // initialize variables
  switched = 0;
  mag_count = 0;
  oldtime = 0;
  average = 0;
  target = 35;
  gear = 1;

  for (rpsidx = RPM_AVG - 1; rpsidx >= 0; rpsidx--)
    rps[rpsidx] = 0;
}

void loop() {
  hall_read = digitalRead(HALL);

  // only reads once a second
  if(millis() - oldtime > 1000) {
    // reset oldtime to calculate the next second
    oldtime = millis();

    calc_average();

    int offset;
    if (average >= MIN_SHIFT)
      offset = target - average;
    else
      offset = 0;

    // pedaling is getting easy shift up to make it harder
    if(offset <= -SHIFT_OFFSET)
      shift_up();

    // pedaling is getting hard shift down to make it easier
    if(offset >= SHIFT_OFFSET)
      shift_down();

    // send interface data
    send_interface();
  }

  // check what state the hall sensor is in and calculate revolutions
  if(hall_read == 1 && switched == 0) {
    mag_count++;
    switched = 1;
  }
  else if(hall_read == 0 && switched == 1) {
    mag_count++;
    switched = 0;
  }

  // check for waiting radio messages
  if (Radio.CheckReceiveFlag()) {
    // receive general radio data
    Radio.ReceiveData((byte *)&radio_data);

    // check sender of radio data
    if (radio_data.header == SWITCHER_HEADER) {
      // decode into gear data and account for inconsistency in system gear numbering
      gear_data = radio_data.gear;
      gear = GEAR_COUNT - gear_data.gear;
    }
    else if (radio_data.header == INTERFACE_HEADER) {
      // decode into target data
      target_data = radio_data.target;

      // send necessary data down to switcher
      if (target_data.up)
        shift_up();

      if (target_data.down)
        shift_down();

      // set or clear target cadence
      if (target_data.set) {
        if (target > 0)
          target = average;
        else
          target = -1;
      }
    }

    // prepare for next radio packet
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
  average /= RPM_AVG;
  average *= 60;
  average /= MAGS;

  // reset mag_count for the next pass
  mag_count = 0;

  Serial.print("Cadence is: ");
  Serial.println(average, DEC);
}

void shift_up() {
  // prepare shift structure
  shift_data.up = false;
  shift_data.down = true;

  // put it into a radio packet
  radio_data.header = SWITCHER_HEADER;
  radio_data.shift = shift_data;

  // send packet and put radio back in receive mode
  Radio.SendData((byte *)&radio_data, sizeof(radio_data));
  Radio.RxOn();

  Serial.println("Shift Up");
}

void shift_down() {
  // prepare shift structure
  shift_data.up = true;
  shift_data.down = false;

  // put it into a radio packet
  radio_data.header = SWITCHER_HEADER;
  radio_data.shift = shift_data;

  // send packet and put radio back in receive mode
  Radio.SendData((byte *)&radio_data, sizeof(radio_data));
  Radio.RxOn();

  Serial.println("Shift Down");
}

void send_interface() {
  // prepare interface structure
  interface_data.cadence = average;
  interface_data.gear = gear;

  // put it into a radio packet
  radio_data.header = INTERFACE_HEADER;
  radio_data.interface = interface_data;

  // send packet and put radio back in receive mode
  Radio.SendData((byte *)&radio_data, sizeof(radio_data));
  Radio.RxOn();
}
