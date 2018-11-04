/***************************************************************************
 holes.c -- Memory Management Simulator for Assignment 3
 
 Created: March.26 2018
 Author: Nick Major
 Contact: nmajor@mail.uoguelph.ca
 ***************************************************************************/

#include "holes.h"

char *checkArguments(int argc, char **args) {
    if(strcmp(args[0], "./holes") == 0) {
        if(argc < 2) {
            printf("Missing File\n");
            return NULL;
        } else if(argc > 2) {
            printf("Number of arguments is greater than 2\n");
            return NULL;
        }
    } else {
        printf("'%s' is an unkown command\n", args[0]);
        return NULL;
    }
    
    return args[1];
}

int checkParameters(char *file) {
    FILE *readFile = fopen(file, "r");
    
    if(readFile != NULL) {
        fseek (readFile, 0, SEEK_END);
        if(ftell(stdin) == 0) {
            return 1;
        } else {
            rewind(readFile);
            return 0;
        }
    }
    
    return 1;
}

int main(int argc, char * argv[]) {
    Simulation *simulation= (Simulation*)malloc(sizeof(Simulation));
    char *file = argv[1];
    char algorithms[4] = {'f', 'b', 'w', 'n'};
    
    file = checkArguments(argc, argv);
    if(file == NULL) {
        exit(0);
    }
    
    if(checkParameters(file) == 1) {
        printf("Please provide a valid filename.\n");
        exit(0);
    }
    
    //Run simulation for each algorithm
    for(int i = 0; i < 4; i ++) {
        simulation = initSimulation(simulation);
        simulation = readProcesses(simulation, file);
        simulation = memorySimulation(simulation, algorithms[i]);
    }
    
    freeSimulation(simulation);
    
    return 0;
}

//Main loop for memory simulation for fit algorithms
Simulation *memorySimulation(Simulation *simulation, char fit) {
    int i = 0;
    int index = 0;
    int oldest = 1;
    int processes = 0;
    
    if(fit == 'f') {
        printf("---First Fit Results---\n");
    } else if(fit == 'b') {
        printf("---Best Fit Results---\n");
    } else if(fit == 'w') {
        printf("---Worst Fit Results---\n");
    } else {
        printf("---Next Fit Results---\n");
    }
    
    while(i < simulation->queueSize) {
        if(fit == 'f') {
           index = firstPosition(simulation->memory, simulation->memorySize, &simulation->queue[i]);
        } else if(fit == 'b') {
            index = bestPosition(simulation->memory, simulation->memorySize, &simulation->queue[i]);
        } else if(fit == 'w') {
            index = worstPosition(simulation->memory, simulation->memorySize, &simulation->queue[i]);
        } else {
            index = nextPosition(simulation->memory, simulation->memorySize, &simulation->queue[i], index);
        }
        
        //Remove Oldest Process from memory, if cant find a position for process
        if(index == -1) {
            simulation = removeoldestProcess(simulation);
            i = -1;
        }
        //Add process to memory
        else if(simulation->space >= 0) {
            //If process needs to fit in between processes in memory, create hole
            if(simulation->memory[index].size > simulation->queue[i].size) {
                if(simulation->memory[index+1].pid != '\0') {
                    simulation->memory = createHole(simulation->memory, simulation->memorySize, index);
                }
                simulation->memory[index+1].size = simulation->memory[index].size - simulation->queue[i].size;
            }
            
            //Only increment memory if the process and hole arent the same size
            if(simulation->memory[index].size != simulation->queue[i].size) {
                simulation->memorySize ++;
            }
            
            simulation->memory[index] = simulation->queue[i];
            simulation->holes = getHoles(simulation->memory, simulation->memorySize);
            simulation->avg_holes = simulation->avg_holes + simulation->holes;
            processes = getProcesses(simulation->memory, simulation->memorySize);
            simulation->avg_processes = simulation->avg_processes + processes;
            simulation->total_loads ++;
            simulation->space = simulation->space - simulation->memory[index].size;
            simulation->cum_memory = simulation->cum_memory + (float)(128-simulation->space)/128*100;
            
            printf("pid:%c loaded, #processes:%d, #holes:%d, %memusage:%.2f%%, cumulative %%mem:%.2f%%\n", simulation->memory[index].pid, processes, simulation->holes, (float)(128-simulation->space)/128*100, simulation->cum_memory/simulation->total_loads);
            
            simulation->memory[index].oid = oldest;
            oldest ++;
            
            simulation->queue = removeProcess(&simulation->queue[i], simulation->queueSize);
            simulation->queueSize --;
            
            i = -1;
        }
        i ++;
        //printMemory(simulation->memory, simulation->memorySize);
        //printQueue(simulation->queue, simulation->queueSize);
    }
    
    printf("\ntotal loads:%d, avg #processes:%.2f, avg #holes:%.2f, cumulative %%mem:%.3f%%\n\n", simulation->total_loads, (float)simulation->avg_processes/simulation->total_loads, (float)simulation->avg_holes/simulation->total_loads, simulation->cum_memory/simulation->total_loads);
    
    return simulation;
}

//Finds posiiton based on First Fit Algorithm
int firstPosition(Process *memory, int size, Process *node) {
    int index = -1;
    
    for(int i = 0; i < size; i ++) {
        if(memory[i].pid == '\0' && memory[i].size >= node->size) {
            index = i;
            break;
        }
    }
    
    return index;
}

//Finds posiiton based on Best Fit Algorithm
int bestPosition(Process *memory, int size, Process *node) {
    int index = -1;
    int smallestSpace = 128;
    
    for(int i = 0; i < size; i ++) {
        if(memory[i].pid == '\0' && memory[i].size >= node->size && memory[i].size <= smallestSpace) {
            smallestSpace = memory[i].size;
            index = i;
        }
    }
    
    return index;
}

//Finds posiiton based on Worst Fit Algorithm
int worstPosition(Process *memory, int size, Process *node) {
    int index = -1;
    int largestSpace = 0;
    
    for(int i = 0; i < size; i ++) {
        if(memory[i].pid == '\0' && memory[i].size >= node->size && memory[i].size > largestSpace) {
            largestSpace = memory[i].size;
            index = i;
        }
    }
    
    return index;
}

//Finds posiiton based on Next Fit Algorithm
int nextPosition(Process *memory, int size, Process *node, int lastIndex) {
    int index = -1;
    
    for(int i = lastIndex; i < size; i ++) {
        if(memory[i].pid == '\0' && memory[i].size >= node->size) {
            index = i;
            break;
        }
        if(i >= size) {
            i = -1;
            break;
        }
    }
    
    return index;
}


//Cycles through Processes and add to queue
Simulation *readProcesses(Simulation *simulation, char *file) {
    FILE *readFile = fopen(file, "r");
    char buffer[20];
    char a;
    int x = 0, i = 0;
    
    while(fgets (buffer, 20, readFile) != NULL) {
        sscanf(buffer, "%c %d", &a, &x);
        if(a != ' ') {
            simulation->queueSize ++;
            simulation->queue = realloc(simulation->queue, sizeof(Process)*(simulation->queueSize+2));
            simulation->memory = realloc(simulation->memory, sizeof(Process)*(simulation->queueSize+2));
            simulation->queue[i] = *initProcess(&simulation->queue[i], a, x);
            simulation->num_processes ++;
            i ++;
        }
    }
    
    fclose(readFile);
    
    return simulation;
}

//Shift Memory Right and create hole
Process *createHole(Process *memory, int size, int index) {
    for(int i = size; i > index; i --) {
        memory[i] = memory[i-1];
    }
    memory[index+1].pid = '\0';
    
    return memory;
}

//Get Number of holes in memory
int getHoles(Process *memory, int size) {
    int holes = 0;
    
    for(int i = 0; i < size; i ++) {
        if(memory[i].pid == '\0') {
            holes ++;
        }
    }
    
    return holes;
}

//Get Number of processes in memory
int getProcesses(Process * memory, int size) {
    int processes = 0;
    
    for(int i = 0; i < size; i ++) {
        if(memory[i].pid != '\0') {
            processes ++;
        }
    }
    
    return processes;
}

//Get memory usage
float getMemoryUsage(Process *memory, int size) {
    float usage = 0.0;
    
    for(int i = 0; i < size; i ++) {
        if(memory[i].pid != '\0') {
            usage = usage + memory[i].size;
        }
    }
    
    return usage/128*100;
}

//Combines holes beside eachother into one
Simulation *combineHoles(Simulation *simulation) {
    for(int i = 0; i < simulation->memorySize-1; i ++) {
        if(simulation->memory[i].pid == '\0' && simulation->memory[i+1].pid == '\0') {
            simulation->memory[i].size = simulation->memory[i].size + simulation->memory[i+1].size;
            for(int j = i+1; j < simulation->memorySize-1; j ++) {
                simulation->memory[j] = simulation->memory[j+1];
            }
            simulation->memorySize --;
            simulation->holes --;
            i --;
        }
    }
    
    return simulation;
}

//Removes the oldest process from memory and adds it to
//the back of the processes queue
Simulation *removeoldestProcess(Simulation *simulation) {
    int oldest = 1000;
    
    for(int i = 0; i < simulation->memorySize; i ++) {
        if(simulation->memory[i].pid != '\0' && simulation->memory[i].oid <= oldest) {
            oldest = simulation->memory[i].oid;
        }
    }
    
    for(int i = 0; i < simulation->memorySize; i ++) {
        if(simulation->memory[i].oid == oldest && simulation->memory[i].pid != '\0') {
            if(simulation->memory[i].swapped < 2) {
                simulation->memory[i].swapped ++;
                simulation->memory[i].oid = 0;
                simulation->queue[simulation->queueSize] = simulation->memory[i];
                simulation->queueSize ++;
            }
            
            simulation->memory[i].pid = '\0';
            simulation->memory[i].oid = 0;
            simulation->space = simulation->space + simulation->memory[i].size;
            simulation->holes ++;
            break;
        }
    }
    
    simulation = combineHoles(simulation);
    
    return simulation;
}

//Remove process from head of Queue and shift
//everything back
Process *removeProcess(Process *memory, int size) {
    for (int i = 0; i < size-1; i ++) {
        memory[i] = memory[i+1];
    }
    
    return memory;
}

//Initialize Simulation
Simulation *initSimulation(Simulation *simulation) {
    simulation->num_processes = 0;
    simulation->total_loads = 0;
    simulation->avg_processes = 0;
    simulation->avg_holes = 0;
    simulation->cum_memory = 0;
    simulation->space = 128;
    simulation->holes = 0;
    simulation->queueSize = 0;
    simulation->memorySize = 1;
    
    simulation->queue = (Process*)malloc(sizeof(Process));
    simulation->memory = (Process*)malloc(sizeof(Process));
    simulation->memory[0] = *initProcess(&simulation->memory[0], '\0', 128);
    
    return simulation;
}

//Initialize Process
Process *initProcess(Process *process, char id, int size) {
    process = malloc(sizeof(Process));
    process->pid = id;
    process->size = size;
    process->swapped = 0;
    process->oid = 0;
    
    return process;
}

//Free Simulation
void freeSimulation(Simulation *simulation) {
    free(simulation->memory);
    free(simulation->queue);
    free(simulation);
}

//Print functions for testing
void printMemory(Process *memory, int size) {
    printf("----Mem----\n");
    for(int i = 0; i < size; i ++) {
        printf("%c %d\n", memory[i].pid, memory[i].size);
    }
    printf("\n");
}

void printQueue(Process *memory, int size) {
    printf("----Queue----\n");
    for(int i = 0; i < size; i ++) {
        printf("%c %d %d\n", memory[i].pid, memory[i].size, memory[i].swapped);
    }
    printf("\n");
}
