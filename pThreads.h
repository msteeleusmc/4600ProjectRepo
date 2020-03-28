#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <mutex>
#include <unistd.h>
#include <sys/resource.h>

#include <list>
#include <queue>

using namespace std;

sem_t P_unlink;
sem_t P_threadLock;
sem_t P_link;


int p_mutex=1, p_mutex2=1, ptid, iter=0;
  list<int> freelist, list_1, list_2;
	int b,x,y,c;



	bool resourceCheck(list<int> &lst)
	{
		if(lst.size()>0)
			return true;
			else
			return false;
	}

/*void P_unlink(int &p_mutex, list<int> &lst, int id){
	cout<<id<<" in - ";
while(1){
	while(p_mutex==0);
	p_mutex =0;
	if(resourceCheck(lst)==false)
	{
		p_mutex=1;
		continue;
	}
	else
	break;
	}
	cout<<id<<"out\n";
}
void P_threadLock(int &p_mutex2)
{
	while(p_mutex2==0);
	p_mutex2=0;
}

void V_threadLock(int &p_mutex2)
{
	p_mutex2=1;
}

void P_link(int &p_mutex){

	while(p_mutex==0);

	p_mutex=0;
	//cout<<"Made it past the mutex in P_link and set Mutex to 0\n";
}

void V(int &p_mutex){

	p_mutex=1;
	return;
}
*/
void link_list(int temp, list<int> &lst){

		lst.push_back(temp);
	}

void link_freelist()
{
		freelist.push_back(0);
}

void use_block_x_to_produce_info_in_y(int &x, int &y){
		y=x;
		x=0;
	}

void consume_information_in_block(int &c){

		c = 0;
	}


	void* thread(void* arg){
		//critical section
		int ptid = (int)arg;


		//create the while loop here
		if(ptid==1){
		while(1){

			sem_wait(&P_threadLock);
			//cout<<"Thread "<<ptid<<" Holds the lock\n";
			if(resourceCheck(freelist)==false  || (freelist.size()==1 && resourceCheck(list_2)==false))
			{
				sem_post(&P_threadLock);
        //cout<<"Thread "<<ptid<<" Released the lock\n";
				continue;
			}


			sem_wait(&P_unlink);
					b = freelist.back();
					freelist.pop_back();
					cout<<"Thread One Removed "<<b<<" from freelist\n";
			sem_post(&P_unlink);

			sem_wait(&P_link);
					b=++iter;
					link_list(b, list_1);
				cout<<"Thread One Added "<<b<<" to list 1\n";
			sem_post(&P_link);
			sem_post(&P_threadLock);
    //  cout<<"Thread "<<ptid<<" Released the lock\n";
			 //break;
			}
		}
			//while loop
		if(ptid==2){
		while(1){
			sem_wait(&P_threadLock);
			//cout<<"Thread "<<ptid<<" Holds the lock\n";
			if(resourceCheck(list_1)==false || resourceCheck(freelist)==false)
			{
				sem_post(&P_threadLock);
        //cout<<"Thread "<<ptid<<" Released the lock\n";
				continue;
			}
				sem_wait(&P_unlink);
						x = list_1.back();
						list_1.pop_back();
						cout<<"Thread Two Removed "<<x<<" from list 1\n";

				sem_post(&P_unlink);

				sem_wait(&P_unlink);
						y = freelist.front();
						freelist.pop_front();
						cout<<"Thread Two Removed "<<y<<" from freelist\n";
						use_block_x_to_produce_info_in_y(x,y);
				sem_post(&P_unlink);



				sem_wait(&P_link);
						link_list(x, freelist);
						cout<<"Thread Two Added "<<x<<" to freelist\n";
				sem_post(&P_link);

				sem_wait(&P_link);
						link_list(y, list_2);
						cout<<"Thread Two Added "<<y<<" to list 2\n";
				sem_post(&P_link);
				sem_post(&P_threadLock);
        //cout<<"Thread "<<ptid<<" Released the lock\n";

				}
			}


					if(ptid==3){
					while(1){
						sem_wait(&P_threadLock);
						//cout<<"Thread "<<ptid<<" Holds the lock\n";
						if(resourceCheck(list_2)==false)
						{
							sem_post(&P_threadLock);
              //cout<<"Thread "<<ptid<<" Released the lock\n";
							continue;
						}
						sem_wait(&P_unlink);
								c = list_2.back();
								list_2.pop_back();
								cout<<"Thread Three Removed "<<c<<" from list 2\n"<<flush;
								consume_information_in_block(c);
						sem_post(&P_unlink);
						sem_wait(&P_link);
								link_list(c, freelist);
								cout<<"Thread Three Added "<<c<<" to freelist\n";
						sem_post(&P_link);
						sem_post(&P_threadLock);
          //  cout<<"Thread "<<ptid<<" Released the lock\n";
						}
					}
	}
