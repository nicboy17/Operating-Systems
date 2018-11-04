/***************************************************************************
 my_shell.c -- a simple UNIX shell program.
 
 Created: Jan.17 2018
 Author: Nick Major
 Contact: nmajor@mail.uoguelph.ca
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <errno.h>

extern char **getln();
void mainLoop();
void add(char **args);
void arg(char **args);
void command(char **args, int background, int input, char *input_file);

int main(int argc, const char * argv[]) {
    printf("Assignment #1, enter 'exit' to leave the program.\n");
    mainLoop();
    
    return 0;
}

//Case statement ('spagatti code') that gets user input and calls functions
void mainLoop() {
    int i;
    int input;
    int output;
    char *input_file = "";
    char *output_file = "";
    int background = 1;
    char **args;
    
    //infinite loop
    while(1) {
        printf(">> ");
        args = getln();
        for(i = 0; args[i] != NULL; i++) {
            
            //exit
            if(strcmp(args[0], "exit") == 0) {
                printf("bye\n");
                exit(0);
            }

            //check &
            else if(i > 0 && strcmp(args[i], "&") == 0) {
                background = 0;
                free(args[i]);
                args[i] = NULL;
            }
            
            //check input redirection
            else if(strcmp(args[i], "<") == 0) {
                input = 1;
                input_file = args[i+1];
                args[i+1] = NULL;
                args[i] = NULL;
            }
            
            //check output redirection
            else if(strcmp(args[i], ">") == 0) {
                output = 1;
                output_file = args[i+1];
                args[i+1] = NULL;
                args[i] = NULL;
            }
        }
        
        //change stdout to user specified file
        if(output == 1) {
            freopen(output_file, "w", stdout);
        }
        
        //no command
        if(args[0] == NULL);
        
        //adds all numbers in command line
        else if(strcmp(args[0], "add") == 0) {
            add(args);
        }
        
        //counts and lists command line arguments
        else if(strcmp(args[0], "arg") == 0) {
            arg(args);
        }
        
        //pangram
        else if(strcmp(args[0], "pangram") == 0) {
            args[0] = "./pangram";
            command(args, background, input, input_file);
        }
        
        //simple command
        else {
            command(args, background, input, input_file);
        }
        
        //switch back stdout to console
        if(output == 1) {
            freopen("/dev/tty", "a", stdout);
            output = 0;
        }
        
        if(input == 1) {
            input = 0;
        }
        
        if(background == 0) {
            background = 1;
        }
    }
}

//Executes command from user input
void command(char **args, int background, int input, char *input_file) {
    int status;
    pid_t pid = fork();
    
    if (pid == 0) {
        //change stdin to user specified file
        if(input == 1) {
            freopen(input_file, "r", stdin);
        }
        execvp(args[0], args);
    }
    
    //blocks if ampersand is not present
    if(background == 1) {
        waitpid(pid, &status, 0);
    }
}

//Adds up arguments and returns result
void add(char **args) {
    int i;
    char buffer[1024] = "";
    long int total = 0;
    
    for(i = 1; args[i] != NULL; i++) {
        if(args[i+1] == NULL) {
            total = total + strtol(args[i], NULL, 0);
            sprintf(buffer, "%s%s", buffer, args[i]);
            fprintf(stdout, "%s = %ld\n", buffer, total);
        } else {
            sprintf(buffer, "%s%s + ", buffer, args[i]);
            total = total + strtol(args[i], NULL, 0);
        }
    }
}

//Returns number of arguments and the arguments
void arg(char **args) {
    int i;
    char buffer[1024] = "";
    
    for(i = 1; args[i] != NULL; i++) {
        if(args[i+1] == NULL) {
            sprintf(buffer, "%s %s\n", buffer, args[i]);
            fprintf(stdout, "arg = %d, args =%s", i, buffer);
        } else {
            sprintf(buffer, "%s %s, ", buffer, args[i]);
        }
    }
}

