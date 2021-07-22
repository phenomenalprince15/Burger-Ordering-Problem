#include "burger.h"

extern uint8_t burger_count;

extern bool interrupt; // = false; /* Flag which will be set to true when the threads should terminate themselves */
void *customer_run(void *args) {
	/* Get args from void pointer */
	simple_arg_t *args_ptr = (simple_arg_t*) args;

	/* Get id */
	uint8_t customer_id = args_ptr->id;

	/* Print status and signal the init_done semaphore */
	printf("[CUSTOMER %d]\t CREATED.\n", customer_id);
	sem_post(args_ptr->init_done);

	/* Wait random time to mix customers (wait atleast 1s to assure that all
	   customers are already created) */
	sleep(rand() % WAITING_TIME + 1);

	/* Synchronize all customers to get a cashier. This will queue up the 
	   customers and guarantees every customer to get a cashier without race
	   conditions */
	sem_wait(&customer_private_mutex);

	/* Signal cashier that a customer is in the room and wait for a cashier to
	   wake up */
	sem_post(&customer);
	sem_wait(&cashier_awake);

	/* The cashier has placed his information in the cashier_exchange variable.
	   -> Get it! */
	sem_t *order = cashier_exchange.order;
	sem_t *burger = cashier_exchange.burger;
	uint8_t cashier_id = cashier_exchange.id;

	/* Leave syncronized area, now the next customer may aquire a cashier */
	sem_post(&customer_private_mutex);

	/* Print a status about the approached cashier */
	printf("[CUSTOMER %d]\t Approached cashier no. %d.\n", 
		customer_id, cashier_id);

	/* Print status that now the order will be placed */
	printf("[CUSTOMER %d]\t Placing order to cashier no. %d.\n", 
		customer_id, cashier_id);

	/* Place order */
	sleep(rand() % WAITING_TIME);

	/* Tell cashier about the order */
	sem_post(order);

	/* Wait for cashier to hand over the burger */
	sem_wait(burger);

	/* Process done, burger received. Print status and exit */
	printf("[CUSTOMER %d]\t Got burger from cashier no. %d. Thank you!\n", 
		customer_id, cashier_id);
	printf("[CUSTOMER %d]\t DONE.\n", customer_id);

	return NULL;

}