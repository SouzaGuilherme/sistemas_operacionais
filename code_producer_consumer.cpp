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

bool number_cousin(int number);

void *produce(void *v){
        parameter_produce *acess = (parameter_produce*)v;
        int iterations_producer = 0;

        srand(time(nullptr));							/* Seed for rand() based in hour */				
        while(iterations_producer < acess->iteration_passed){
                /* Posso usar um enquanto */
                if(buffer.size() == acess->max_size_list)
                        cout << "wait" << endl;

                pthread_mutex_lock(&lock);
                /* Start section critic */
                buffer.push_back(rand());
                /* Finish section critic */
                pthread_mutex_unlock(&lock);

                iterations_producer++;
        }
}

void *consumer(void *v){
        while(true){
                cout << "vazia? " << buffer.empty() << endl;
               /* while(buffer.empty() == true){
                        cout << "preso vazioooooo" << endl;
                        usleep(1); 
                }*/
                if(buffer.empty() == true){
                        while(true){
                                if(buffer.empty() == false){
                                        cout << "saindo do vazio" << endl;
                                        break;
                                }
                        }
                }

                pthread_mutex_lock(&lock);

                cout << "Dentro? " << buffer.front() << endl;
                bool check = false;
                if(buffer.front() == -1){
                        cout << "TIRA: --> " << buffer.front() << endl;
                        break;
                }
                check = number_cousin(buffer.front());
                buffer.pop_front();
                pthread_t id_son = pthread_self();
                cout << "Thread ID: " << id_son << endl;                

                pthread_mutex_unlock(&lock);
                cout << "TO AQUI" << endl;
        }
        cout << "indo embora" << endl;
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
        cout << "main id: " << main_id << endl;

        pthread_t *thread_produce;
        pthread_t *thread_consumer;

        if(((thread_produce = (pthread_t*)malloc(number_producer*sizeof(pthread_t))) == NULL)){
                DIE("Erro'r alloc producers")
        }

        if((thread_consumer = (pthread_t*)malloc(number_consumer*sizeof(pthread_t))) == NULL){
                DIE("Erro'r alloc consumers")
        }

        /* Created of producers */
        for(int i = 0; i < number_producer; ++i){
                if(pthread_create(&thread_produce[i], NULL, produce, (void*)&p_acess))
                        DIE("Not created threads");
        }




        /* ZONA DE TESTE */
/*
        cout << "Finish1" << endl;

        list <int> :: iterator i;
        for(i = buffer.begin(); i != buffer.end(); i++)
                cout << *i << endl;
*/
        /* ------------------------------------- */

        /* Created of consumers */
        for(int i = 0; i < number_consumer; ++i){
                if(pthread_create(&thread_consumer[i], NULL, consumer, (void*)&p_acess))
                        DIE("Not created threads producers");
        }


        /* Join threads of producers */
        for(int i = 0; i < number_producer; ++i){
                if(pthread_join(thread_produce[i], NULL))
                        DIE("Not join threads");
        }
        cout << "Passou producer" << endl;

        for(int j = 0; j < number_consumer; j++)
                buffer.push_back(-1);

        for(auto v : buffer)
                cout << v << endl;
        /* Join threads of consumers */
        for(int r = 0; r < number_consumer; r++){
                cout << "preso join consumer "<< r << endl;
                if(pthread_join(thread_consumer[r], NULL))
                        DIE("Not join threads consumers");
        }

        cout << "Finish join" << endl;
        //free(thread_produce);
        //free(thread_consumer);
        cout << "Finish all" << endl;
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
