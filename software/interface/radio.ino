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
    Radio.ReceiveData((byte *)&radio_data);

    if (radio_data.header == INTERFACE_HEADER) {
      interface_data = radio_data.interface;

      cadence = interface_data.cadence;
      gear = interface_data.gear;
    }

    Radio.RxOn();
  }
}

void send_up() {
  target_data.up = 1;
  target_data.down = 0;
  target_data.set = 0;
  radio_data.header = INTERFACE_HEADER;
  radio_data.target = target_data;

  Radio.SendData((byte *)&radio_data, sizeof(radio_data));
  Radio.RxOn();
}

void send_down() {
  target_data.up = 0;
  target_data.down = 1;
  target_data.set = 0;
  radio_data.header = INTERFACE_HEADER;
  radio_data.target = target_data;

  Radio.SendData((byte *)&radio_data, sizeof(radio_data));
  Radio.RxOn();
}

void send_set() {
  target_data.up = 0;
  target_data.down = 0;
  target_data.set = 1;
  radio_data.header = INTERFACE_HEADER;
  radio_data.target = target_data;

  Radio.SendData((byte *)&radio_data, sizeof(radio_data));
  Radio.RxOn();
}
