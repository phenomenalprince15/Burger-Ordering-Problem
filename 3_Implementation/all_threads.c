#include "burger.h"


void burger_threads()
{

    /*Init random number generator */
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
}
