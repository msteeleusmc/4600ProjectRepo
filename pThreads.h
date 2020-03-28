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
--	This is the header file to project 01 "main.cpp".
--	
--	It provides the funtions for the threads and lists as required
--	by the assignment pdf.
*/

#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <mutex>
#include <unistd.h>
#include <sys/resource.h>

#include <list>
#include <queue>

using namespace std;

/*Create semaphores for unlinking and linking the threads to lists*/
sem_t P_unlink;
sem_t P_threadLock;
sem_t P_link;

/*Created mutex's ptid for process id and iterations*/
int p_mutex=1, p_mutex2=1, ptid, iter=0;

/*Global lists used thoughout all of the functions*/
list<int> freelist, list_1, list_2;

/*Integers used in all of the funcitons to control what is passed to all of the lists*/
int b,x,y,c;

/*Bool function for checking resources in the list passed to function.*/
bool resourceCheck(list<int> &lst) {

	if(lst.size()>0)
		return true;	//If list size greater than 0, return true
	else
		return false;	//If list size == 0, return false
}

/*Function used as a list template for any list passed by reference*/
void link_list(int temp, list<int> &lst) {

		//variable is pushed into the list
		lst.push_back(temp);
}

/*Function used to create N blocks that are all empty in freelist*/
void link_freelist() {

	freelist.push_back(0);
}

/*Thread 2 calls this function to do the following*/
void use_block_x_to_produce_info_in_y(int &x, int &y) {

	y = x;	//y = x where x was assigned value from list-1
	x = 0;	//x now equals zero
}

/*Called by thread 3 to consume information in variable c*/
void consume_information_in_block(int &c) {

	c = 0;	//c now equals zero
}


void* thread(void* arg) {

	//critical section
	int ptid = (int)arg;

	//create the while loop here
	if(ptid == 1){

		while(1){

			//Lock the P_threadLock semaphore
			sem_wait(&P_threadLock);

			cout<< "Thread " << ptid << " Holds the lock\n";

			if(resourceCheck(freelist)==false  || (freelist.size()==1 && resourceCheck(list_2)==false)) {

				//Unlock the P_threadLock semaphore if above conditions are met
				sem_post(&P_threadLock);

        			cout<<"Thread "<<ptid<<" Released the lock\n";
				continue;  //Continue process
			}

			/***********************************************/
			//Lock semaphore P_unlink
			sem_wait(&P_unlink);

			b = freelist.back();			//Variable b now equals back element of 'freelist'
			freelist.pop_back();			//'freelist' deletes back element

			cout << "Thread One Removed " << b << " from freelist\n";

			//Unlock semaphore P_unlink
			sem_post(&P_unlink);
			/**********************************************/

			/**********************************************/
			//Lock semaphore P_link
			sem_wait(&P_link);

			b = ++iter;				//iterator incremented
			link_list(b, list_1);			//Function called to push var b into list-1

			cout<<"Thread One Added "<<b<<" to list 1\n";

			//Unlock semaphore P_link
			sem_post(&P_link);
			/**********************************************/

			/**********************************************/
			//Lock semaphore P_threadLock
			sem_post(&P_threadLock);

    			cout << "Thread " << ptid << " Released the lock\n";
	
		}//End of while-loop

	}//End of "if(ptid == 1)"

	/*******************************************************************/
	if(ptid == 2){

		while(1) {

			//Lock semaphore P_threadLock
			sem_wait(&P_threadLock);

			cout << "Thread " << ptid << " Holds the lock\n";

			if(resourceCheck(list_1)==false || resourceCheck(freelist)==false) {

				//Unlock semaphore P_threadLock if above conditions are met
				sem_post(&P_threadLock);

			        cout << "Thread " << ptid << " Released the lock\n";
				continue;  //Continue the process
			}

			/**********************************************/
			//Lock semaphore P_unlink
			sem_wait(&P_unlink);

			x = list_1.back();		//Variable x now equals back element of 'list-1'
			list_1.pop_back();		//list-1 deletes the back element 

			cout << "Thread Two Removed " << x << " from list 1\n";

			//Unlock semaphore P_unlink
			sem_post(&P_unlink);
			/**********************************************/

			/**********************************************/
			//Lock semaphore P_unlink
			sem_wait(&P_unlink);

			y = freelist.front();		//Variable y now equals front element of 'freelist'
			freelist.pop_front();		//freelist deletes the front element

			cout << "Thread Two Removed " << y << " from freelist\n";

			use_block_x_to_produce_info_in_y(x,y);	//Function used to produce info of var. x into var.  y

			//Unlock semaphore P_unlink
			sem_post(&P_unlink);
			/**********************************************/

			/**********************************************/
			//Lock semaphore P_link
			sem_wait(&P_link);

			link_list(x, freelist);		//Function pushes x variable into 'freelist'

			cout << "Thread Two Added " << x << " to freelist\n";

			//Unlock semaphore P_link
			sem_post(&P_link);
			/**********************************************/

			/**********************************************/
			//Lock semaphore P_link
			sem_wait(&P_link);

			link_list(y, list_2);		//Function pushes y variable into 'list-2'

			cout << "Thread Two Added " << y << " to list 2\n";

			//Unlock semaphore P_link
			sem_post(&P_link);
			/**********************************************/

			/**********************************************/
			//Unlock semaphore P_threadLock
			sem_post(&P_threadLock);

        		cout << "Thread " << ptid << " Released the lock\n";

		}//End of while-loop

	}//End if(ptid == 2)

	/*******************************************************************/
	if(ptid==3){

		while(1){

			/**********************************************/
			//Lock P_threadLock
			sem_wait(&P_threadLock);

			cout << "Thread " << ptid << " Holds the lock\n";

			if(resourceCheck(list_2)==false) {

				//Unlock semaphore P_threadLock if above conditions are met
				sem_post(&P_threadLock);

              			cout << "Thread " << ptid << " Released the lock\n";
				continue;  //Continue process
			}

			/**********************************************/
			//Lock semaphore P_unlink
			sem_wait(&P_unlink);

			c = list_2.back();		//Variable c now equals back element of 'list-2'
			list_2.pop_back();		//list-2 back element is removed

			cout << "Thread Three Removed " << c << " from list 2\n" << flush;

			consume_information_in_block(c);	//Funciton consumes the information from vriable c

			//Unlock semaphore P_unlink
			sem_post(&P_unlink);
			/**********************************************/

			/**********************************************/
			//Lock semaphore P_link
			sem_wait(&P_link);

			link_list(c, freelist);		//Function to push var. c into 'freelist'

			cout << "Thread Three Added " << c << " to freelist\n";

			//Unlock semaphore P_link
			sem_post(&P_link);
			/**********************************************/

			/**********************************************/
			//Unlock semaphore P_threadLock
			sem_post(&P_threadLock);

          		cout << "Thread " << ptid << " Released the lock\n";

		}//End of while-loop

	}//End of if(ptid == 3_

}
