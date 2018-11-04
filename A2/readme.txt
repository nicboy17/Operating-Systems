/******************************************************************
 readme -- documentation for A2.
 
 Created: Feb.26 2018
 Author: Nick Major
 Contact: nmajor@mail.uoguelph.ca
 ********************************************************************/

Folder Contents:
1. simcpu.c
2. simcpu.h
3. makefile
4. readme

To run cpu simulator:
1. directory - cd into current directory (project directory)
2. compile - 'make' (make clean to rm compiled files)
3. exec file - './simcpu [-d] [-v] [-r quantum] < input_file'


Assumptions:
- Based on: "threads do not need to wait to do I/O", I did not add this
	to the simulation total time. I just summed it for the IO time for
	each thread.


Questions:
i.  Does your simulator include switch overhead for the first ready state 
to running state transition? Explain.
	No, there is not switch overhead when switching from ready state to
	running state at any time.

ii. Does your simulator include switch overhead if a thread moves from 
ready state to running state and the CPU is idle? Explain.
	No, the CPU is never idle.

iii. Does your simulator include switch overhead if a thread moves from 
running state to blocked state and the ready queue is empty? Explain.
	No, blocked state is never used.

iv. Does your simulation include switch overhead if a thread is interrupted
(due to a time slice) and either the ready queue is empty or the thread has
the highest priority? Explain.
	No, there is no switch overhead when a thread is interupped due to a
	time slice at any time.


Algorithm:
For this assignment, I tried to closely follow the spec provided.

	First, a function checks the arguments passed in and sets the corresponding flags.
Next, the file passed in gets put in a sorted queue by the thread arrival time. For
modularity, the file gets passed into memory by a number of functions that record
various data (CPU time, IO time, etc ..). Next, the corresponding scheduling simulation
is run. Finally, the simulation data is printed and any memory is freed.

First Come First Serve (FCFS)
	Simplest algorithm of the 2. Simply loops through ready Queue and finishs threads
sequentially one after the other until empty. When a thread terminates, threads
are added to the ready queue if their arrival time is less than the total time.

Round Robin (RR)
	Loops through ready queue until queue is empty. When a thread terminates, threads
are added to the ready queue if their arrival time is less than the total time. If 
the total cpu is greater than the time quatum, the total time is increased by the time
quantum, the cpu count is decreased, and the current thread is moved to the tail
of the queue. Last, if the cpu count is less than the time quantum, the thread is
removed from the queue and terminated.

Data Structure:
I created a main data structure to encapsulate the main simulator variables:
- total number of processes
- total number of threads
- thread switch delay
- process switch delay
- total running time
- total cpu sum
- average turnaround time
- event queue head

for the event queue, I created a sorted linked list of threads in order of arrival:
- process id the thread belongs to
- thread id
- arrival time
- number of cpu
- cpu sum
- io sum
- turnaround
- state of the thread
- Next thread

