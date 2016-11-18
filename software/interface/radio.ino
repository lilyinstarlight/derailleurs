#include <cc1101.h>

#include <constants.h>

// data transfer values
radio_t radio_data;
interface_t interface_data;
target_t target_data;

int cadence = 0;
int gear = 1;

void setup() {
  // initialize radio
  Radio.Init();
  Radio.SetDataRate(4);
  Radio.SetLogicalChannel(1);
  Radio.RxOn();
}

void loop() {
  // receive potential radio data
  if (Radio.CheckReceiveFlag()) {
    // get general radio data
    Radio.ReceiveData((byte *)&radio_data);

    // ensure this node is the intended recipient
    if (radio_data.header == INTERFACE_HEADER) {
      // decode data into structure
      interface_data = radio_data.interface;

      // update variables according to data
      cadence = interface_data.cadence;
      gear = interface_data.gear;
    }

    // prepare for next radio packet
    Radio.RxOn();
  }
}

void send_up() {
  // prepare structure for target
  target_data.up = 1;
  target_data.down = 0;
  target_data.set = 0;

  // stuff structure in a radio packet
  radio_data.header = INTERFACE_HEADER;
  radio_data.target = target_data;

  // send packet and put radio back in receive mode
  Radio.SendData((byte *)&radio_data, sizeof(radio_data));
  Radio.RxOn();
}

void send_down() {
  // prepare structure for target
  target_data.up = 0;
  target_data.down = 1;
  target_data.set = 0;

  // stuff structure in a radio packet
  radio_data.header = INTERFACE_HEADER;
  radio_data.target = target_data;

  // send packet and put radio back in receive mode
  Radio.SendData((byte *)&radio_data, sizeof(radio_data));
  Radio.RxOn();
}

void send_set() {
  // prepare structure for target
  target_data.up = 0;
  target_data.down = 0;
  target_data.set = 1;

  // stuff structure in a radio packet
  radio_data.header = INTERFACE_HEADER;
  radio_data.target = target_data;

  // send packet and put radio back in receive mode
  Radio.SendData((byte *)&radio_data, sizeof(radio_data));
  Radio.RxOn();
}
