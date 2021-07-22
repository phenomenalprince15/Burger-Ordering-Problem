#include "burger.h"

extern uint8_t burger_count;

extern bool interrupt; // = false; /* Flag which will be set to true when the threads should terminate themselves */
void *cook_run(void *args) {
	/* Get args from void pointer */
	simple_arg_t *args_ptr = (simple_arg_t*) args;

	/* Get id */
	uint8_t cook_id = args_ptr->id;
	/* Print status and signal the init_done semaphore */
	printf("[COOK %d]\t CREATED.\n", cook_id);
	sem_post(args_ptr->init_done);

	/* Infinite loop */
	while(1) {
		/* Aquire cook semaphore */
		sem_wait(&cook);

		/* Check if we should terminate */
		if(interrupt) {
			break;
		}

		/* Cook */
		sleep(rand() % WAITING_TIME);

		/* Lock rack and produce burger */
		sem_wait(&rack);
		assure_state();
		burger_count++;
		assure_state();
		sem_post(&rack);

		printf("[COOK %d]\t Placed new burger in rack.\n", cook_id);

		/* Signal a waiting cashier */
		sem_post(&cashier);
	}

	printf("[COOK %d]\t DONE.\n", cook_id);
	return NULL;
}