/******************************************************************
 readme -- documentation for A3.
 
 Created: March.26 2018
 Author: Nick Major
 Contact: nmajor@mail.uoguelph.ca
 ********************************************************************/

Folder Contents:
1. src folder - src files
2. include folder - header files
3. bin folder - executables
4. Test files - test files
5. makefile
6. readme

To run programs:
1. directory - cd into current directory (project directory)
2. compile - make (compiles everything), make dine, make holes, and make clean
3. run - Follows spec
	- ./dine #ofphil #ofTimesEat
	- ./holes testfile 


Assumptions:
- Runs on Linux (Tested on OSX)
- The act of removing processes from memory creates 'holes'


Algorithm:
For this assignment, I tried to closely follow the spec provided.
The first task was relatively straight forward as there are many
examples of psedocode. However, the second assignment was not
as straight forward, but fun nonetheless.

Dining Philospers Problem
   The dining philosphers problem was implemented as explained in
lab, using the pseudocode below. The pthread library was used as
sugested; threads are created, executed, then destroyed when they
have eaten the max amount. As shown below, only one philosopher
can eat at a time.

while(1) {
   sleep(random())  //Thinking
   lock(left_fork)  //locking mutexes aka
   lock(right_fork) //picking up forks
   sleep(random()) //Eating
   unlock(left_fork)
   unlock(right_fork)
}
	

Memory Management Simulation
   Initially, I treated the 128MB as 1 big empty hole and the process
of removing processes will create holes. Each algorithm will fill the
the memory the same way for the first cycle. The following iterations
will fill the memory as per each algorithm. If the memory array is
empty and there is no space for the current processes, the memory
will be reset to its initial 128MB hole. The memory and queue was
simulated with dynamic arrays of structs (see below).

   I tried to keep the program as modular and compact as possible.
So, I created a main simulator function that will call each
respective algorithm and return the position the process should
be inserted at. This worked well as the results from the program
matched the example output posted on the course website.

Data Structures:
For the Queue and Memory, arrays of structs were used:
- pid: process id (character)
- iod: oldest id (number)
- size: megabytes
- swapped: # of times swapped between arrays

For the simulation, a data structure was used to keep the code
clean and consice. Also, to reduce global variables.
- num_processes: total number of processes
- total_loads: total number of loads
- avg_processes: average number of processes
- avg_holes: average number of holes
- cum_memory: cummulative memory usage
- space: running space in memory
- holes: total number of holes in memory
- queueSize: number of elements in queue
- memorySize: number of elements in memory


