#include "burger.h"

bool interrupt = false; /* Flag which will be set to true when the threads should terminate themselves */

/* Define the counter for available burgers in the rack */
uint8_t burger_count = 0;

int main ()
{
    	/* Init random number generator */
	srand(time(NULL));

	/* Init all semaphores */
	sem_init(&rack, 0, 1);
	sem_init(&cashier, 0, 1);
	sem_init(&cashier_awake, 0, 0);
	sem_init(&cook, 0, RACK_HOLDER_SIZE);
	sem_init(&customer, 0, 0);
	sem_init(&customer_private_mutex, 0, 1);

	/* Create semaphore to synchronise thread init and args */
	simple_arg_t args;
	sem_t init_done;
	sem_init(&init_done, 0, 0);
	args.init_done = &init_done;

	/* Start all cook threads */
	pthread_t cooks[COOK_COUNT];
	for(uint8_t i=0; i<COOK_COUNT; i++) {
		/* Set id for cook */
		args.id = i;

		/* Start cook thread and pass args, handle possible erros */
		if(pthread_create(cooks+i, NULL, cook_run, (void*) &args)) {
			printf("[MAIN]\t\t ERROR: Unable to create cook thread.\n");
			exit(1);
		}

		/* Wait until the cook is initialised and ready to run */
		sem_wait(&init_done);
	}

	/* Start all cashier threads */
	pthread_t cashiers[CASHIER_COUNT];
	for(uint8_t i=0; i<CASHIER_COUNT; i++) {
		/* Set id for cashier */
		args.id = i;

		/* Start cashier thread and pass args, handle possible erros */
		if(pthread_create(cashiers+i, NULL, cashier_run, (void*) &args)) {
			printf("[MAIN]\t\t ERROR: Unable to create cashier thread.\n");
			exit(2);
		}

		/* Wait until the cashier is initialised and ready to run */
		sem_wait(&init_done);
	}

	/* Start all customer threads */
	pthread_t customers[CUSTOMER_COUNT];
	for(uint8_t i=0; i<CUSTOMER_COUNT; i++) {
		/* Set id for customer */
		args.id = i;

		/* Start customer thread and pass args, handle possible erros */
		if(pthread_create(customers+i, NULL, customer_run, (void*) &args)) {
			printf("[MAIN]\t\t ERROR: Unable to create customer thread.\n");
			exit(3);
		}

		/* Wait until the customer is initialised and ready to run */
		sem_wait(&init_done);
	}

	/* destroy init semaphore */
	sem_destroy(&init_done);

	/* wait for all customer threads to finish */
	for(uint8_t i=0; i<CUSTOMER_COUNT; i++) {
		/* Join customer and handle possible errors */
		if(pthread_join(customers[i], NULL)) {
			printf("[MAIN]\t\t ERROR: Unable to join cutomers[%d]\n", i);
			exit(4);
		}
	}

	/*
	 * CLEANUP
	 */

	printf("[MAIN]\t\t SUCCESS: All customers terminated\n");
	printf("\n----------------\n\n[MAIN]\t\t SUCCESS: Starting Cleanup\n");

	/* Set interrupt flag */
	interrupt = true;

	/* Wake all cooks up, they will see the interrupt flag and will exit */
	for(uint8_t i=0; i<COOK_COUNT; i++) {
		sem_post(&cook);
	}

	/* Wake all cashiers up, they will see the interrupt flag and will exit */
	for(uint8_t i=0; i<CASHIER_COUNT; i++) {
		sem_post(&customer);
	}

	/* All threads were woken up */
	printf("[MAIN]\t\t SUCCESS: Told all threads to terminate themselves\n");

	/* wait for all cook threads to finish */
	for(uint8_t i=0; i<COOK_COUNT; i++) {
		/* Join customer and handle possible errors */
		if(pthread_join(cooks[i], NULL)) {
			printf("[MAIN]\t\t ERROR: Unable to join cooks[%d]\n", i);
			exit(5);
		}
	}

	/* wait for all cashier threads to finish */
	for(uint8_t i=0; i<CASHIER_COUNT; i++) {
		/* Join customer and handle possible errors */
		if(pthread_join(cashiers[i], NULL)) {
			printf("[MAIN]\t\t ERROR: Unable to join cashiers[%d]\n", i);
			exit(6);
		}
	}

	/* Print status and exit */
	assure_state();
	printf("[MAIN]\t\t SUCCESS: All threads terminated, state consistent.\n");
}