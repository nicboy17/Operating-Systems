/***************************************************************************
 holes.h -- Memory Management Simulator for Assignment 3
 
 Created: March.26 2018
 Author: Nick Major
 Contact: nmajor@mail.uoguelph.ca
 ***************************************************************************/

#ifndef holes_h
#define holes_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define memory_size 128

typedef struct Process Process;
typedef struct Process {
    char pid;
    int size;
    int swapped;
    int oid;
} Process;

typedef struct Simulation Simulation;
typedef struct Simulation {
    int num_processes;
    int total_loads;
    int avg_processes;
    int avg_holes;
    float cum_memory;
    int space;
    int holes;
    int queueSize;
    int memorySize;
    
    Process *queue;
    Process *memory;
} Simulation;


Simulation *memorySimulation(Simulation *simulation, char fit);
int firstPosition(Process *memory, int size, Process *node);
int bestPosition(Process *memory, int size, Process *node);
int worstPosition(Process *memory, int size, Process *node);
int nextPosition(Process *memory, int size, Process *node, int lastIndex);

Simulation *removeoldestProcess(Simulation *simulation);
Simulation *combineHoles(Simulation *simulation);
Process *removeProcess(Process * memory, int size);
Process *insertProcess(Process *memory, Process *node);
Process *createHole(Process *memory, int size, int index);
int getHoles(Process *memory, int size);
int getProcesses(Process * queue, int size);
float getMemoryUsage(Process *memory, int size);

Simulation *readProcesses(Simulation *simulation, char *file);
Simulation *initSimulation(Simulation *simulation);
Process *initProcess(Process *process, char id, int size);
void freeSimulation(Simulation *simulation);

void printMemory(Process *memory, int size);
void printQueue(Process *memory, int size) ;

#endif /* holes_h */

