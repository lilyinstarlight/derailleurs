#include <SPI.h>
#include <ADXL362.h>

#define THRESHOLD 1500

#define STEP_TIME 500

#define LOW_FILTER 0.2

#define SAMPLES 20
#define SAMPLE_TIME 10

ADXL362 xl;

bool red;

int lx, ly, lz, lt;
int fx, fy, fz, ft;
int x, y, z, t;

int iter;

long sx, sy, sz, st;

int vec;

void step() {
  if (red) {
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);

    red = false;
  }
  else {
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);

    red = true;
  }
}

void setup() {
  // prepare LEDs
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, LOW);

  red = true;

  // prepare accelerometer
  xl.begin(P3_0);
  xl.beginMeasure();
}

void loop() {
  sx = 0;
  sy = 0;
  sz = 0;
  st = 0;

  for (iter = 0; iter < SAMPLES; iter++) {
    // read accelerometer
    xl.readXYZTData(x, y, z, t);

    // filter data
    fx = (1.0 - LOW_FILTER)*x + LOW_FILTER*lx;
    fy = (1.0 - LOW_FILTER)*y + LOW_FILTER*ly;
    fz = (1.0 - LOW_FILTER)*z + LOW_FILTER*lz;
    ft = (1.0 - LOW_FILTER)*t + LOW_FILTER*lt;

    lx = fx;
    ly = fy;
    lz = fz;
    lt = ft;

    sx += fx;
    sy += fy;
    sz += fz;
    st += ft;

    delay(SAMPLE_TIME);
  }

  sx /= SAMPLES;
  sy /= SAMPLES;
  sz /= SAMPLES;
  st /= SAMPLES;

  vec = sqrt(sx*sx + sy*sy + sz*sz);

  if (vec > THRESHOLD) {
    // record two steps since accelerometer is only on one leg
    step();
    delay(STEP_TIME);
    step();
  }
}
