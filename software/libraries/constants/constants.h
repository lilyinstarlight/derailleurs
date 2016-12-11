#define SWITCHER_HEADER 0b11011001
#define INTERFACE_HEADER 0b10011101

typedef struct {
	uint8_t up;
	uint8_t down;
} shift_t;

typedef struct {
	uint8_t gear;
} gear_t;

typedef struct {
	uint8_t cadence;
	uint8_t gear;
} interface_t;

typedef struct {
	uint8_t up;
	uint8_t down;
	uint8_t set;
} target_t;

typedef struct {
	uint8_t header;

	union {
		shift_t shift;
		gear_t gear;
		interface_t interface;
		target_t target;
	};
} radio_t;
