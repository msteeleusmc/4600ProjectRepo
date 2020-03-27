#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

#include "pThreads.h"

using namespace std;

//sem_t mutex;

int main() {
	int one = 1, two = 2, three = 3;

	//initiate the mutex
	sem_init(&s_mutex, 0, 0);

	//created an array of threads for potential loops
	pthread_t pt[3];

	//create all of the threads
	pthread_create(&pt[0], 0, thread1, (void *)one);
	pthread_create(&pt[1], 0, thread2, (void *)two);
	pthread_create(&pt[2], 0, thread3, (void *)three);

	//join all of the threads
	for(int i = 0; i < 3; i++){

		pthread_join(pt[i], NULL);
	}

	//destroy the mutex
	sem_destroy(&s_mutex);

return 0;
}
