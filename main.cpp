#include <iostream>
#include <pthread.h>
#include <semaphore.h>
/*
=====================================================================
--	Class: 	"CSCE 4600"	Project 01
--
--	Group:	"Group_Random"
--
--	Members:
--		1. Ayman Alqaq		ata0099@unt.edu
--		2. Blake Cox		bac0323@unt.edu
--		3. Michael Steele	mls0586@unt.edu
=====================================================================
-- 
--	This is the "main.cpp" program file for project 01.
--	
*/
#include <pthread.h>
#include <unistd.h>

#include "pThreads.h"

using namespace std;

int main() {

	int one = 1, two = 2, three = 3;
	int N;
	
	cout << "\nEnter the number of N blocks for freelist ";
	cin >> N;

	//For-loop to link_free_list funciton which creates N number of empty blocks
	for(int i = 0; i < N; i++) {

		link_freelist();
	}

	//created an array of threads for potential loops
	pthread_t pt[3];

	//Initialize semaphore P_unlink
	sem_init(&P_unlink, 0, 1);

	//Initialize semaphore P_threadLock
	sem_init(&P_threadLock, 0, 1);

	//Initialize semaphore P_link
	sem_init(&P_link, 0, 1);

	//create all of the threads
	for(int i=0; i<3; i++) {
	
		pthread_create(&pt[i], 0, thread, (void *)(i+1));
	}

	//join all of the threads
	for(int i = 0; i < 3; i++){

		pthread_join(pt[0], NULL);
	}

	//Destroy semaphore P_unlink
	sem_destroy(&P_unlink);

	//Destroy semaphore P_threadLock
	sem_destroy(&P_threadLock);

	//Destroy semaphore P_link
	sem_destroy(&P_link);

return 0;
}
