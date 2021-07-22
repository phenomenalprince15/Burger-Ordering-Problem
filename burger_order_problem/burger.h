#ifndef __BURGER_H
#define __BURGER_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <inttypes.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
/*
#define COOK_COUNT 			3
#define CASHIER_COUNT		2
#define CUSTOMER_COUNT		4
#define RACK_HOLDER_SIZE	4
#define WAITING_TIME		5
*/

int COOK_COUNT; //= 3
int CASHIER_COUNT; //= 2
int CUSTOMER_COUNT;  //= 4
int RACK_HOLDER_SIZE; //= 4
int WAITING_TIME;		//= 5

//bool interrupt = false; /* Flag which will be set to true when the threads should terminate themselves */

/* define struct representing a cashier */
typedef struct {
	uint8_t id;
	sem_t *order;
	sem_t *burger;
} cashier_t;

/* define struct to pass args to the run functions */
typedef struct {
	uint8_t id;
	sem_t *init_done;
} simple_arg_t;

/* Declare function to check current system state */
void assure_state();

/* Define all needed semaphores */
sem_t rack;
sem_t cook;
sem_t cashier;
sem_t cashier_awake;
sem_t customer;
sem_t customer_private_mutex;

/* Define memory space to store a cashier */
cashier_t cashier_exchange;

/* Define the counter for available burgers in the rack */
//uint8_t burger_count = 0;


void *cook_run();
void *cashier_run();
void *customer_run();
void burger_threads();

#endif