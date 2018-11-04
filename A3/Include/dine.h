/***************************************************************************
 dine.h -- Philosphers Dining problem for Assignment 3
 
 Created: March.26 2018
 Author: Nick Major
 Contact: nmajor@mail.uoguelph.ca
 ***************************************************************************/

#ifndef dine_h
#define dine_h

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define max_philosopher 100

int dine = 0;
int num_phil = 0;
int num_eat = 0;
int eaten[max_philosopher] = {0};
pthread_mutex_t forks [max_philosopher];

void *philospher(void * param);
void take_fork(int id);
void put_fork(int id);
void test(int id);

#endif /* dine_h */
