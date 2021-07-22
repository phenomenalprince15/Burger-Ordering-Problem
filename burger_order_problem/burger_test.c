#include "burger.h"

bool interrupt = false; /* Flag which will be set to true when the threads should terminate themselves */

/* Define the counter for available burgers in the rack */
uint8_t burger_count = 0;

int main ()
{
	int num_of_tests;
	printf("Number of test cases :: ");
	scanf("%d",&num_of_tests);
	while (num_of_tests--)
	{
		printf("Enter cook count :: ");
		scanf("%d",&COOK_COUNT);
		printf("Enter cashier count :: ");
		scanf("%d",&CASHIER_COUNT);
		printf("Enter customer count :: ");
		scanf("%d",&CUSTOMER_COUNT);
		printf("Enter rack holder size :: ");
		scanf("%d",&RACK_HOLDER_SIZE);
		printf("Enter waiting time :: ");
		scanf("%d",&WAITING_TIME);
	
		burger_threads();
	}
}