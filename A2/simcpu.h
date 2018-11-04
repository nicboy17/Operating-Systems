/***************************************************************************
 simcpu.h -- Public interface for 

 Created: Feb.26 2018
 Author: Nick Major
 Contact: nmajor@mail.uoguelph.ca
 ***************************************************************************/

#ifndef SIMCPU_H
#define SIMCPU_H A2

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Simulation CPU settings */
int detailed_mode = 0;
int verbose_mode = 0;
int rr_mode = 0;
int quantum = 0;

/* Data structures */
typedef enum { NEW=0,
    READY,     // more text found after end of calendar
    RUNNING,   // version missing or wrong
    BLOCKED,   // BEGIN...END not found as expected
    TERMINATED,// I/O error
} State;

typedef struct Thread Thread;
typedef struct Thread {
    int process_id;
    int thread_id;
    int arrival_time;
    int cpu;
    int cpu_count;
    int io_count;
    int turnaround;
    State state;
    
    Thread *next;
}Thread;

typedef struct Simulation Simulation;
typedef struct Simulation {
    int number_of_processes;
    int thread_switch;
    int process_switch;
    int total_time;
    int total_threads;
    int total_cpu;
    int turnaround_time;
    
    Thread *head;
}Simulation;

/* Functions */
int checkArguments(char **args);
void printResults(Simulation *sim);
void fcfs_sim(Simulation *sim);
void rr_sim(Simulation *sim);

Thread *deleteNode(Thread *head);
Thread *nodeToTail(Thread *head);
Thread *addToQueue(Thread *head, Thread *node);

Simulation *readThreads(Simulation *sim, int process_id, int number_of_threads);
Thread *readBursts(Simulation *sim, Thread *thread);
Simulation *readProcesses(Simulation *sim);

Simulation *initSimulation(int number_of_processes, int thread_switch, int process_switch);

void freeSimulation(Simulation *sim);
void freeThread(Thread *thread);

#endif
