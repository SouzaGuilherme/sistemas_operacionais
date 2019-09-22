#include<iostream>
#include<cstdlib>
#include<ctime>
#include<pthread.h>
#include<unistd.h>
#include<list>
#include<iterator>
#include<cstdlib>

using namespace std;

#define DIE(...) { \
        fprintf(stderr, __VA_ARGS__); \
        exit(EXIT_FAILURE); \
}

/* Parameters for send function produce/consumer */
typedef struct parameter{
        int iteration_passed;
        int max_size_list;
}parameter_produce;

/* Declared Buffer, using list of C++ */
list <int> buffer;

/* Declared variable mutex */
pthread_mutex_t lock;

/* Declared variables of condition */
pthread_cond_t more;
pthread_cond_t notice;

/* Function verify if is cousing */
bool number_cousin(int number);

/* Signiture of consumer */
void *consumer(void *v);

/* Signiture of produce */
void *produce(void *v);

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

        pthread_t id_father = pthread_self();
        cout << id_father << endl;
        pthread_t *thread_produce;
        pthread_t *thread_consumer;

        /* Start vaviables of conditions */
        if(pthread_cond_init(&more, NULL))
                DIE("Erro'r in init variable of condition");
        if(pthread_cond_init(&notice, NULL))
                DIE("Erro'r in init variable of condition");

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

        /* Fill Buffer with -1 for finalize the consumers */
        for(int j = 0; j < number_consumer; j++){
                /* Section critic */
                pthread_mutex_lock(&lock);
                if(buffer.size() == p_acess.max_size_list)
                        pthread_cond_wait(&notice, &lock);

                buffer.push_back(-1);
                /* Finish section critic */
                pthread_mutex_unlock(&lock);
                /* Signal of finish */
                pthread_cond_signal(&more);
        }

        /* Join threads of consumers */
        for(int r = 0; r < number_consumer; r++){
                if(pthread_join(thread_consumer[r], NULL))
                        DIE("Not join threads consumers");
        }

        if(pthread_cond_destroy(&more))
                DIE("Erro'r not destroy variable of codition");
        if(pthread_cond_destroy(&notice))
                DIE("Erro'r not destroy variable of codition");

        free(thread_produce);
        free(thread_consumer);
        cout << "Finish all" << endl;
}

/* Function produce */
void *produce(void *v){
        parameter_produce *acess = (parameter_produce*)v;
        int iterations_producer = 0;

        while(iterations_producer < acess->iteration_passed){

                pthread_mutex_lock(&lock);
                if(buffer.size() == acess->max_size_list)
                        pthread_cond_wait(&notice, &lock);

                /* Start section critic */
                buffer.push_back(rand());
                /* Finish section critic */
                
                pthread_mutex_unlock(&lock);

                /* Send signal of finish */
                pthread_cond_signal(&more);

                iterations_producer++;
        }
}

/* Function consumer */
void *consumer(void *v){
        while(true){
                pthread_mutex_lock(&lock);

                while(buffer.empty() == true)
                        pthread_cond_wait(&more, &lock);

                bool check = false;
                int number_tmp = buffer.front();
                buffer.pop_front();

                pthread_t id_son = pthread_self();
                if(number_tmp == -1){
                        pthread_mutex_unlock(&lock);
                        break;
                }

                pthread_mutex_unlock(&lock);

                /* Signal your end */
                pthread_cond_signal(&notice);

                /* Verify if is cousin in parallel */
                check = number_cousin(number_tmp);

                /* Verify your answer, if yes, print, else continue */
                if(check == true)
                        cout << "[" << id_son << " : " << number_tmp << "]" << endl;                
        }
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
