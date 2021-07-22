#include "burger.h"

extern bool interrupt; 
extern uint8_t burger_count;

void *cashier_run(void *args) {
	/* Get args from void pointer */
	simple_arg_t *args_ptr = (simple_arg_t*) args;

	/* Get id */
	uint8_t cashier_id = args_ptr->id;

	/* Create private order and burger semaphore */
	sem_t order;
	sem_t burger;
	sem_init(&order, 0, 0);
	sem_init(&burger, 0, 0);

	/* Print status and signal the init_done semaphore */
	printf("[CASHIER %d]\t CREATED.\n", cashier_id);
	sem_post(args_ptr->init_done);

	/* Infinite loop */
	while(1) {
		 /* Wait for customer */
		sem_wait(&customer);

		/* Check if we should terminate */
		if(interrupt) {
			break;
		}

		/* print status */
		printf("[CASHIER %d]\t Serving customer.\n", cashier_id);

		/* Save my id to the exchange */
		cashier_exchange.order = &order;
		cashier_exchange.burger = &burger;
		cashier_exchange.id = cashier_id;

		/* Tell customer that I am awake and information is placed */
		sem_post(&cashier_awake);

		/* Wait for the order and print sttaus */
		sem_wait(&order);
		printf("[CASHIER %d]\t Got order.\n", cashier_id);

		/* Print sttaus that now the burger will be get from the rack */
		printf("[CASHIER %d]\t Going to rack to get burger...\n", cashier_id);

		/* Go to rack */
		sleep(rand() % WAITING_TIME);

		/* Aquire cashier semaphore */
		sem_wait(&cashier);

		/* Lock rack and get burger */
		sem_wait(&rack);
		assure_state();
		burger_count--;
		assure_state();
		sem_post(&rack);

		/* Signal a waiting cook a new burger can be produced */
		sem_post(&cook);

		/* Got successfull a burger. Print status */
		printf("[CASHIER %d]\t Got burger from rack, going back\n", cashier_id);

		/* Go back to customer */
		sleep(rand() % WAITING_TIME);

		/* Give burger to customer and print status */
		sem_post(&burger);
		printf("[CASHIER %d]\t Gave burger to customer.\n", cashier_id);
	}

	/* free semaphores and print message */
	sem_destroy(&order);
	sem_destroy(&burger);
	printf("[CASHIER %d]\t DONE.\n", cashier_id);

	return NULL;
}