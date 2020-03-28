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

	for(int i=0; i<4; i++)
	{
		link_freelist();
	}

	//initiate the mutex
	//sem_init(&s_mutex, 0, 0);

	//created an array of threads for potential loops
	pthread_t pt[3];
	sem_init(&P_unlink, 0, 1);
  sem_init(&P_threadLock, 0, 1);
  sem_init(&P_link, 0, 1);
	//create all of the threads
	for(int i=0; i<3; i++)
	pthread_create(&pt[i], 0, thread, (void *)(i+1));


	//join all of the threads
	for(int i = 0; i < 3; i++){

		pthread_join(pt[0], NULL);
	}
	sem_destroy(&P_unlink);
	sem_destroy(&P_threadLock);
	sem_destroy(&P_link);
	//destroy the mutex
	//sem_destroy(&s_mutex);

return 0;
}
