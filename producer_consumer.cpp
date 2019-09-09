#include<iostream>
#include<cstdlib>
#include<ctime>
#include<pthread.h>
#include<unistd.h>
#include<list>
#include<iterator>
#define size_max 100

#define DIE(...) { \
        fprintf(stderr, __VA_ARGS__); \
        exit(EXIT_FAILURE); \
}

using namespace std;

int buffer[size_max];

typedef struct parameter{
	int number_passed;
	int position_for_number;
}parameter_produce;


/* 
 * Use in producer
 * srand(time(nullptr));
 * position_in_buffer_recive = rand();
 *
 * print in c++
 * cout << "string" << variable << endl;
 */

bool number_cousin(int number);

void* produce(void *v){
	parameter_produce *acess = (parameter_produce*)v;
	//cout << "AHAM AMIGÂO" << acess->number_passed << endl;
	
	int interation = acess->number_passed;
	
	srand(time(nullptr));							/* Seed for rand() based in hour */				
	while(interation != 0){
		buffer[acess->position_for_number] = rand();
		interation--;
		cout << buffer[acess->position_for_number] << endl;
		usleep(10);
	}
}

int main(int argc, char *argv[]){
	parameter_produce *acess;
	int check_thread;
        /* Receive per parameter */
	acess->number_passed = atoi(argv[1]);
	acess->position_for_number = atoi(argv[2]);
	
	cout << "pass" << endl;
	pthread_t ts_producer, ts_consumer;

	for(int i = 0; i < size_max; ++i)				/* Clean buffer first time */
		buffer[i] = 0;

	cout << "pass" << endl;
	check_thread = pthread_create(&ts_producer, NULL, produce, (void*)acess);
	if(check_thread)
		DIE("Not create threads\n");


	cout << "pass" << endl;
	list <int> l;
	l.push_back(0);

	for(int i = 0; i < 10; i++)
		listing1.push_back(i*2);	

/*	
	list <int> :: iterator it;
	for(it = listing1.begin(); it != listing1.end(); ++it)
		cout << 't' << *it;
*/	
	pthread_join(ts_producer, NULL);
	
	return 0;
}

/* Function verifyc if the number passed is cousin */
bool number_cousin(int number){
	bool is_cousin = true;						/* I assume it is cousin */
	int divider = 2;						/* First Divider */
	
	if(number <= 1)							/* Check if it's positive */
		return false;

	while((is_cousin == true) && (divider <= number/2)){		/* Division secessive */
		int rest = number % divider;
		if(rest == 0)
			is_cousin = false;
		divider++;	
	}

	return is_cousin;
};
