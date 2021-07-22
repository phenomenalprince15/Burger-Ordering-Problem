#include "burger.h"

extern uint8_t burger_count;

void assure_state() {
	/* Assure that more than zero burgers are available */
	if(burger_count < 0) {
		printf("[ASSURE_STATE]\t ERROR: Negative burger count!\n");
		exit(40);
	}

	/* Assure that not more burgers than rack spaces are available */
	if(burger_count > RACK_HOLDER_SIZE) {
		printf("[ASSURE_STATE]\t ERROR: Rack overfull!\n");
		exit(41);
	}

	printf("[ASSURE_STATE]\t State consistent.\n");
}