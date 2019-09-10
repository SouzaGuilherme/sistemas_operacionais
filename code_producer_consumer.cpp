#include<iostream>
#include<cstdlib>
#include<ctime>
#include<pthread.h>
#include<unistd.h>
#include<list>
#include<iterator>
#include<cstdlib>

#define DIE(...) { \
        fprintf(stderr, __VA_ARGS__); \
        exit(EXIT_FAILURE); \
}

typedef struct parameter{
        int iteration_passed;
        int max_size_list;
}parameter_produce;

using namespace std;

list <int> buffer;
pthread_mutex_t lock;

void *produce(void *v){
        parameter_produce *acess = (parameter_produce*)v;
        int iterations_producer;

        while(iterations_producer < acess->iteration_passed){
          if(buffer.size() == acess->max_size_list)
                  cout << "wait" << endl;
        }
}

void *consumer(void *v){
}

int main(int argc, char *argv[]){
        /* ./producer_consumer <iterations> <number_producer> <number_consumer> <size_max_buffer> */
        system("clear");
        if(argc < 5)
                DIE("** Parameters erro's **\nRun with the parameters:\n\n  ./producer_consumer <iterations> <number_producer> <number_consumer> <size_max_buffer>\n\n");

        /* Receive parameters */
        parameter_produce p_acess;
        p_acess.iteration_passed = atoi(argv[1]);
        int number_producer = atoi(argv[2]);
        int number_consumer = atoi(argv[3]);
        p_acess.max_size_list = atoi(argv[4]);

        /* Get ID of thread main */
        pthread_t main_id = pthread_self();
        cout << main_id << endl;

        pthread_t thread_produce[number_producer];

        /* Created of producers */
        for(int i = 0; i < number_producer; ++i){
                if(pthread_create(&thread_produce[i], NULL, produce, (void*)&p_acess))
                        DIE("Not created threads");
        }


        /* Join threads od producers */
        for(int i = 0; i < number_producer; ++i){
                if(pthread_join(thread_produce[i], NULL))
                        DIE("Not join threads");
        }
}
