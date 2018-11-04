/***************************************************************************
 simcpu.c -- CPU simulator for FCFS and RR
 
 Created: Feb.26 2018
 Author: Nick Major
 Contact: nmajor@mail.uoguelph.ca
 ***************************************************************************/

#include "simcpu.h"

int main(int argc, char * argv[]) {
    Simulation *sim = NULL;
    
    //Check for quantum value
    if(checkArguments(argv) != 0) {
        printf("Missing quantun value.\n");
        exit(0);
    }
    
    //Check for input file
    fseek (stdin, 0, SEEK_END);
    if(ftell(stdin) == 0) {
        printf("Please provide a simulation file.\n");
        exit(0);
    }
    rewind(stdin);

    sim = readProcesses(sim);
    
    if(rr_mode) {
        rr_sim(sim);
    } else {
        fcfs_sim(sim);
    }
    
    printResults(sim);
    
    freeSimulation(sim);
    
    return 0;
}

//Check arguments and flag modes
int checkArguments(char **args) {
    int q = 0;
    
    for(int i = 0; args[i] != NULL; i ++) {
        
        if(strcmp(args[i], "-d") == 0) {
            detailed_mode = 1;
        }
        
        if(strcmp(args[i], "-v") == 0) {
            verbose_mode = 1;
        }
        
        if(strcmp(args[i], "-r") == 0) {
            rr_mode = 1;
            
            if(args[i+1] != NULL) {
                quantum = atoi(args[i+1]);
            } else {
                q= 1;
            }
        }
    }
    
    return q;
}

//First come first serve simulation
//Uses temporary files for displaying results after simulation summary
void fcfs_sim(Simulation *sim) {
    FILE *detail = fopen("detail.txt", "w+");
    FILE *verbose = fopen("verbose.txt", "w+");
    Thread *readyQ = sim->head;
    
    int turnaround = 0;
    int old_id = 0;
    
    //Loop through all threads in queue
    for(int i = 0; i < sim->total_threads; i ++) {
        turnaround = 0;
        
        //Verbose Move thread from new to ready
        if(verbose_mode == 1) {
            Thread *current = sim->head;
            
            while(current != NULL) {
                if(current->arrival_time <= sim->total_time && current->state == NEW) {
                    current->state = READY;
                    fprintf(verbose, "\nAt time %d: Thread %d of Process %d moves from new to ready\n", sim->total_time, current->thread_id, current->process_id);
                } else if(current->arrival_time > sim->total_time) {
                    break;
                }
                current = current->next;
            }
        }
        
        //Add overhead for switching to new thread
        sim->total_time += sim->thread_switch;
        
        //Verbose move thread from ready to running
        if(verbose_mode == 1) {
            fprintf(verbose,"\nAt time %d: Thread %d of Process %d moves from ready to running\n", sim->total_time, readyQ->thread_id, readyQ->process_id);
        }
        
        //Add process switch if previous process has different id
        if(old_id != readyQ->process_id) {
            sim->total_time += sim->process_switch;
        }
        old_id = readyQ->process_id;
        
        //Add CPU count to total time
        sim->total_time += readyQ->cpu_count;
        
        //Delete from ready queue
        sim->head = deleteNode(sim->head);
        
        //Verbose move from running to terminated
        if(verbose_mode == 1) {
            fprintf(verbose,"\nAt time %d: Thread %d of Process %d moves from running to terminated\n", sim->total_time, readyQ->thread_id, readyQ->process_id);
        }
        
        //Detailed summary
        if (detailed_mode == 1) {
            turnaround = (sim->total_time - readyQ->arrival_time);
            
            fprintf(detail,"\nThread %d of Process %d:\n", readyQ->thread_id, readyQ->process_id);
            fprintf(detail,"\narrival time: %d", readyQ->arrival_time);
            fprintf(detail,"\nservice time: %d units", readyQ->cpu_count);
            fprintf(detail,"\nI/O time: %d units", readyQ->io_count);
            fprintf(detail,"\nturnaround time: %d units", turnaround);
            fprintf(detail,"\nfinish time: %d units\n", sim->total_time);
        }
        sim->turnaround_time += sim->total_time - readyQ->arrival_time;
        
        readyQ = readyQ->next;
    }
    
    fclose(detail);
    fclose(verbose);
}

//Print simulation results
//Uses temporary files for displaying results after simulation summary
void printResults(Simulation *sim) {
    float avg_turnaround_time = (float)sim->turnaround_time/sim->total_threads;
    float cpu_utilization = (float)sim->total_cpu/sim->total_time * 100;
    
    if(rr_mode == 1) {
        printf("Round Robin (quantun = %d time units):\n", quantum);
    } else {
        printf("FCFS:\n");
    }
    
    printf("\nTotal Time required is %d time units\n", sim->total_time);
    printf("Average Turnaround Time is %0.1f time units\n", avg_turnaround_time);
    printf("CPU Utilization is %0.1f%%\n\n", cpu_utilization);
    
    system("cat detail.txt");
    system("cat verbose.txt");
    system("rm detail.txt");
    system("rm verbose.txt");
}

//Add threads in order to ready queue
Thread *addToQueue(Thread *head, Thread *node) {
    int i = 0;
    Thread *temp = head;
    
    while(temp != NULL) {

            if(i == 0 && node->arrival_time < temp->arrival_time) {
                node->next = temp;
                head = node;
                return head;
            } else if(temp->next == NULL) {
                temp->next = node;
                return head;
            } else if(node->arrival_time < temp->next->arrival_time) {
                node->next = temp->next;
                temp->next = node;
                return head;
            }
        
        temp = temp->next;
        i ++;
    }
    
    return head;
}

//Pop head from queue
Thread *deleteNode(Thread *node) {
    Thread *pop = node->next;
    
    return pop;
}

//Move head to tail
Thread *nodeToTail(Thread *head) {
    Thread *next = head->next;
    Thread *current = head;
    
    while(current->next != NULL) {
        current = current->next;
    }
    
    current->next = head;
    head->next = NULL;
    
    return next;
}

//Round robin simulation
//Uses temporary files for displaying results after simulation summary
void rr_sim(Simulation *sim) {
    FILE *detail = fopen("detail.txt", "w+");
    FILE *verbose = fopen("verbose.txt", "w+");
    Thread *readyQ = sim->head;
    
    int turnaround = 0;
    int old_id = 0;
    
    //Loops through queue
    while(readyQ != NULL) {
        turnaround = 0;
        
        //Verbose move thread from new to ready
        if(verbose_mode == 1) {
            Thread *current = sim->head;
            
            while(current != NULL) {
                if(current->arrival_time <= sim->total_time && current->state == NEW) {
                    current->state = READY;
                    fprintf(verbose, "\nAt time %d: Thread %d of Process %d moves from new to ready\n", sim->total_time, current->thread_id, current->process_id);
                } else if(current->arrival_time > sim->total_time) {
                    break;
                }
                current = current->next;
            }
        }
        
        //Overhead thread switch
        sim->total_time += sim->thread_switch;
        
        //Verbose move thread from ready to running
        if(verbose_mode == 1) {
            readyQ->state = RUNNING;
            
            fprintf(verbose,"\nAt time %d: Thread %d of Process %d moves from ready to running\n", sim->total_time, readyQ->thread_id, readyQ->process_id);
        }
        
        //Overhead process switch
        if(old_id != readyQ->process_id) {
            sim->total_time += sim->process_switch;
        }
        old_id = readyQ->process_id;
        
        //Time Slice or Remove from queue
        if(readyQ->cpu_count > quantum) {
            sim->total_time += quantum;
            readyQ->cpu_count -= quantum;
            
            readyQ->state = READY;
            
            //Move thread to back of the ready queue
            if(verbose_mode == 1) {
                fprintf(verbose,"\nAt time %d: Thread %d of Process %d moves from running to ready\n", sim->total_time, readyQ->thread_id, readyQ->process_id);
            }
            
            sim->head = nodeToTail(sim->head);
            readyQ = sim->head;
            
        } else {
            sim->total_time += readyQ->cpu_count;
            
            readyQ->state = TERMINATED;
            
            //Verbose terminate thread
            if(verbose_mode == 1) {
                fprintf(verbose,"\nAt time %d: Thread %d of Process %d moves from running to terminated\n", sim->total_time, readyQ->thread_id, readyQ->process_id);
            }
            
            //Detailed mode summary
            if (detailed_mode == 1) {
                turnaround = (sim->total_time - readyQ->arrival_time);
                
                fprintf(detail,"\nThread %d of Process %d:\n", readyQ->thread_id, readyQ->process_id);
                fprintf(detail,"\narrival time: %d", readyQ->arrival_time);
                fprintf(detail,"\nservice time: %d units", readyQ->cpu_count);
                fprintf(detail,"\nI/O time: %d units", readyQ->io_count);
                fprintf(detail,"\nturnaround time: %d units", turnaround);
                fprintf(detail,"\nfinish time: %d units\n", sim->total_time);
            }
            sim->turnaround_time += sim->total_time - readyQ->arrival_time;
            
            sim->head = deleteNode(sim->head);
            readyQ = sim->head;
        }
    }
    
    fclose(detail);
    fclose(verbose);
}

//Cycles through Processes and add processor id to respective threads
Simulation *readProcesses(Simulation *sim) {
    int number_of_processes, thread_switch, process_switch;
    int process_id, number_of_threads;
    
    fscanf(stdin, "%d %d %d", &number_of_processes, &thread_switch, &process_switch);
    sim = initSimulation(number_of_processes, thread_switch, process_switch);
    
    for(int i = 0; i < sim->number_of_processes; i ++) {
        fscanf(stdin, "%d %d", &process_id, &number_of_threads);
        
        sim = readThreads(sim, process_id, number_of_threads);
    }

    return sim;
}

//Create queue of threads from file
Simulation *readThreads(Simulation *sim, int process_id, int number_of_threads) {
    int x, y, z;
    Thread *node;
    
    for(int i = 0; i < number_of_threads; i ++) {
        fscanf(stdin, "%d %d %d", &x, &y, &z);
        node = (Thread*)malloc(sizeof(Thread));
        node->thread_id = x;
        node->arrival_time = y;
        node->cpu = z;
        node->cpu_count = 0;
        node->io_count = 0;
        node->process_id = process_id;
        node->turnaround = 0;
        node->state = NEW;
        node->next = NULL;
        node = readBursts(sim, node);
        sim->total_threads ++;

        if(sim->head == NULL) {
            sim->head = node;
        } else {
            sim->head = addToQueue(sim->head, node);
        }
    }
    
    return sim;
}

//Sums CPU and IO Bursts and adds them to respective thread
Thread *readBursts(Simulation *sim, Thread *thread) {
    int x, y, z = 0;
    
    for(int i = 0; i < thread->cpu; i ++) {
        if(i == thread->cpu-1) {
            fscanf(stdin, "%d %d", &x, &y);
            z = 0;
        } else {
            fscanf(stdin, "%d %d %d", &x, &y, &z);
        }
        
        thread->cpu_count += y;
        thread->io_count += z;
        sim->total_cpu += y;
    }
    
    return thread;
}

//Initialize Simulation structure
Simulation *initSimulation(int number_of_processes, int thread_switch, int process_switch) {
    Simulation *sim;
    sim = (Simulation*)malloc(sizeof(Simulation));
    sim->number_of_processes = number_of_processes;
    sim->process_switch = process_switch;
    sim->thread_switch = thread_switch;
    sim->total_time = 0;
    sim->total_threads = 0;
    sim->total_cpu = 0;
    sim->turnaround_time = 0;
    sim->head = NULL;
    
    return sim;
}

//Free Simulation
void freeSimulation(Simulation *sim) {
    free(sim->head);
    sim->head = NULL;
    
    free(sim);
    sim = NULL;
}

//Free Thread
void freeThread(Thread *thread) {
    thread->thread_id = 0;
    thread->cpu = 0;
    thread->arrival_time = 0;
    thread->cpu_count = 0;
    thread->io_count = 0;
    
    free(thread);
    thread = NULL;
}
