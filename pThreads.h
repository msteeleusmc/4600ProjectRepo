#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <mutex>
#include <unistd.h>

#include <list>
#include <queue>

using namespace std;

int p_mutex=1, ptid;
static list<int> freelist, list_1, list_2;
int b,x,y,c;

void P_unlink(int &p_mutex, list<int> &lst){
	while(lst.size()<1);
	while(p_mutex==0);
	p_mutex =0;
}

void P_link(int &p_mutex){

	while(p_mutex==0);
	p_mutex=0;
}

void V(int &p_mutex){

	p_mutex=1;
}

void link_list(int temp, list<int> &lst){

		lst.push_back(temp);
	}

void link_freelist()
{
	for(int i=0; i<4; i++)
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
		ptid = (int)arg;
		//create the while loop here
		if(ptid==1){
		while(1){
			P_unlink(p_mutex, freelist);
					b = freelist.front();
					freelist.pop_front();
			V(p_mutex);
					cout<<"Removed "<<b<<" from freelist\n";
			P_link(p_mutex);
					link_list(b, list_1);
					cout<<"Added "<<b<<" to list 1\n";
			V(p_mutex);
			}
		}
			//while loop
		if(ptid==2){
		while(1){
				P_link(p_mutex);
						x = list_1.front();
						list_1.pop_front();
						cout<<"Removed "<<x<<" from list 1\n";
				V(p_mutex);
				P_link(p_mutex);
						y = freelist.front();
						freelist.pop_front();
						cout<<"Removed "<<y<<" from freelist\n";
				V(p_mutex);

				use_block_x_to_produce_info_in_y(x,y);

				P_link(p_mutex);
						link_list(x, freelist);
						cout<<"Added "<<x<<" to feeelist\n";
				V(p_mutex);

				P_link(p_mutex);
						link_list(y, list_2);
						cout<<"Added "<<y<<" to list 2\n";
				V(p_mutex);
				}
			}

		if(ptid==2){
		while(1){
			P_link(p_mutex);
					c = list_2.front();
					list_2.pop_front();
					cout<<"Removed "<<c<<" from list 2\n"<<flush;
			V(p_mutex);

			consume_information_in_block(c);

			P_link(p_mutex);
					link_list(c, freelist);
					cout<<"Added "<<c<<" to freelist\n";
			V(p_mutex);
			}
		}
	}
