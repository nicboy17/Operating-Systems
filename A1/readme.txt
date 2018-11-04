/***************************************************
readme -- documentation for A1.
 
 Created: Jan.17 2018
 Author: Nick Major
 Contact: nmajor@mail.uoguelph.ca
 ****************************************************/

Assumptions
- Tested on macOS and Linux
- 

To run shell:
1. 'make'
2. './msh'

Algorithm:
For this assignment, I tried to closely follow the spec provided. I used the given code found on the course website as a starting point.

For the shell, I created an infinte loop that will exit when the user types in exit. A command with and without arguments is executed by forking a child process and waiting for it to complete. If a command includes an ampersand at the end, the process is not blocked and returns immediately to the console. The ampersand is deleted and wait is not executed. File output and input redirection was implemented using freopen as stdout and stdin are redirected. The greater than or less than sign is deleted with the file input/output and filename is saved.

The new commands were implemented by iterating through the command arguments and providing the desired output. For the custom function, I decided to reuse an old CIS1500 assignment. The command in 'pangram', simply enter in a sentence (< 128 char) and it will check if it is a pangram or tautogram. It is implemented by passing it to my execution function, with blocking.

Ex.
>> pangram abcdefghijklmnopqrstuvwxyz
output:
The sentence is: abcdefghijklmnopqrstuvwxyz 

This is a pangram.

this is not a tautogram.

Note: to compile a4.c
'gcc a4.c -o pangram'

void execR(char ** args)
{
	int status,sig;
	pid_t process;
	int i;
	sig =0;
	/*Check for andpercent&*/
	for (i = 1; args[i]!=NULL;i++)
	{
		if (strcmp(args[i],"&")==0)
		{
			sig = 1;/*Set the sig to 1*/
			args[i] = NULL;/*Remove the andpercent*/
		}

	}
	
	if (sig == 1)
	{
		sigset(SIGCHLD,ignore);/*Sigset to run in the background*/
	}
	
	pid_t ret = fork();
	if (ret == 0)
	{
		process = execvp(args[0],args);/*execvp to execute the shell command*/
	}
	else
		process = wait(&status);/*look wait first*/
}
/*Ignore subfunction*/
void ignore (int sig)
{
	int status;
  	int result = wait(&status);
	printf("Process is done");
}