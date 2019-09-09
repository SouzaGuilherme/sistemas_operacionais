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
	int iteration_passed;
	int position_for_number;                                                /* Acho que nem vais er usado */
        int max_size_list;
}parameter_produce;

list <int> listing;

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
        int number_producer, number_consumer;
        /* Receive per parameter */
	acess->iteration_passed = atoi(argv[1]);
	//acess->position_for_number = atoi(argv[2]);                         /* Acho que sera desnecessario */
	number_producer = atoi(argv[2]);
	number_consumer = atoi(argv[3]);
        acess->max_size_list = atoi(argv[4]);

	cout << "pass" << endl;
	pthread_t ts_producer[number_producer], ts_consumer[number_consumer];

	for(int i = 0; i < size_max; ++i)				/* Clean buffer first time */
		buffer[i] = 0;

	cout << "pass" << endl;
        for(int i = 0; i < number_producer; ++i){
  	  check_thread = pthread_create(&ts_producer, NULL, produce, (void*)acess);
	  if(check_thread)
	    DIE("Not create threads\n");
        }

	pthread_join(ts_producer, NULL);
       
        cout << "aham" << endl; 
        cout << listing.max_size() << endl;        
        cout << listing.empty() << endl; 
        listing.push_back(10);
        cout << listing.empty() << endl; 
        cout << listing.back() << endl;
        list <int> :: iterator i = listing.end();
        cout << i << endl;
        cout << listing.size() << endl;
        if(listing.max_size() == 10)
                cout << "aham" << endl;
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
