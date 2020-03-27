#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <mutex>
#include <unistd.h>

#include <list>
#include <queue>

using namespace std;

sem_t s_mutex;

int process_cnt;

list<int> freelist, list_1, list_2;
int b = 0;
int x = 0;
int y = 0;
int c = 0;

void produce_information_in_block(){

	b = 16;
}

void link(int temp, list<int> L){

	L.push_back(temp);
}

void use_block_x_to_produce_info_in_y(){
	x = b;
	y = x;
}

void consume_information_in_block(){

	c = x;
}

void* thread1(void* arg){

	//wait
	//sem_wait(&s_mutex);
	process_cnt = 1;

	cout << "Entered Thread " << (int)arg << "\n";


	//critical section
	sleep(2);
	//create the while loop here
	while(1){

		b = freelist.front();
		cout << "\nVariable b := unlink(freelist)\n" << endl;
		if(!freelist.empty())
			freelist.pop_front();

		produce_information_in_block();
		cout << "\nVariable b now equals " << b << endl;
		link(b, list_1);
		cout << "\nVariable b is linked to list_1\n" << endl;
		break;
	}

	//signal
	cout << "Exiting Thread " << (int)arg << "\n";
	process_cnt = 2;
	sem_post(&s_mutex);
}

void* thread2(void* arg){

	//wait
	sem_wait(&s_mutex);
	if(process_cnt == 2){

		cout << "Entered Thread " << (int)arg << "\n";

		//critical section
		sleep(2);
		//while loop
		while(1){

			x = list_1.front();
			cout << "\nVariable x := unlink(list_1)\n" << endl;
			if(!list_1.empty())
				list_1.pop_front();

			y = freelist.front();
			cout << "\nVaraible y := unlink(freelist)\n" << endl;
			if(!freelist.empty())
				freelist.pop_front();

			use_block_x_to_produce_info_in_y();

			link(x, freelist);
			cout << "\nVariable x linked to freelist with the value of " << x << "\n" << endl;
			link(y, list_2);
			cout << "\nVariable y linked to list_2 with the value of " << y << "\n" << endl;
			break;
		}

		//signal
		cout << "Exiting Thread " << (int)arg << "\n";

	}
	process_cnt = 3;
	sem_post(&s_mutex);
}

void* thread3(void* arg){

	//wait
	sem_wait(&s_mutex);
	if(process_cnt == 3){
	//m_lock.lock();
	cout << "Entered Thread " << (int)arg << "\n";

	//critical section
	sleep(2);
	//while loop
	while(1){

		c = list_2.front();
		cout << "\nVariable c := unlink(list_2)\n" << endl;
		if(!list_2.empty())
			list_2.pop_front();

		consume_information_in_block();

		link(c, freelist);
		cout << "\nVariable c is linked to freelist with the value of " << c << "\n" << endl;
		break;
	}
	//signal
	cout << "Exiting Thread " << (int)arg << "\n";
	process_cnt = 1;
	//m_lock.unlock();
	}
	sem_post(&s_mutex);
}
