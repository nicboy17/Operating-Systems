/***************************************************************************
 dine.c -- Assignment 3
 
 Created: March.26 2018
 Author: Nick Major
 Contact: nmajor@mail.uoguelph.ca
 ***************************************************************************/

#include "dine.h"

int checkArguments(int argc, char **args) {
    if(strcmp(args[0], "./dine") == 0) {
        if(argc == 3) {
            num_phil = atoi(args[1]);
            num_eat = atoi(args[2]);
        } else {
            printf("Number of arguments is greater than 3\n");
            return 0;
        }
    } else {
        printf("'%s' is an unkown command\n", args[0]);
        return 0;
    }
    
    return 1;
}

int checkParameters() {
    if(num_phil < 2 && num_phil > 100) {
        printf("The number of philosophers must be larger than 2 and less than 100\n");
        return 0;
    } else if(num_eat < 1 || num_eat > 1000) {
        printf("The number of times each philosophers must be greater than 1 and less than 1000\n");
        return 0;
    }
    
    return 1;
}

int main(int argc, char * argv[]) {
    if(checkArguments(argc, argv) == 0) {
        exit(0);
    }
    
    if(checkParameters() == 0) {
        exit(0);
    }
    
    pthread_t thread[max_philosopher];
    int thread_id[max_philosopher];
    
    for (int i = 0; i < num_phil; i++) {
        thread_id[i] = i;
        pthread_create(&(thread[i]), NULL, &philospher, &thread_id[i]);
        pthread_mutex_init(&forks[i], NULL);
    }
    
    //Join the threads
    for (int i = 0; i < num_phil; i++) {
        pthread_join(thread[i], NULL);
    }
    
    //Kill the threads
    for (int i = 0; i < num_phil; i++) {
        pthread_mutex_destroy(&forks[i]);
    }
    
    return 0;
}

void *philospher(void * param) {
    int right = *((int*) param);
    int left = right+1;
    
    if(left == -1) {
        left = num_phil-1;
    }
    
    while (eaten[left] <= num_eat) {
        printf("Philosopher %d Thinking\n", right+1);
        pthread_mutex_lock(&forks[right]);
        pthread_mutex_lock(&forks[left]);
        
        sleep(1);
        //Unlock 1 and 2
        if(eaten[left])
        printf("Philosopher %d Eating\n", right+1);
        eaten[left]++;
        pthread_mutex_unlock(&forks[right]);
        pthread_mutex_unlock(&forks[left]);
    }
    
    pthread_exit(NULL);
}

