/*************************a4.c*******************************************
Student Name: Nick Major                 Student Number:  0879292
Date: 11/22/14                           Course Name: CIS1500
I have exclusive control over this submission via my password.
By including this statement in this header comment, I certify that:
1) I have read and understood the University policy on academic integrity;
2) I have completed the Computing with Integrity Tutorial on Moodle; and
3) I have achieved at least 80% in the Computing with Integrity Self Test.
I assert that this work is my own. I have appropriately acknowledged any and all material
(data, images, ideas or words) that I have used, whether directly quoted or paraphrased.
Furthermore, I certify that this assignment was prepared by me specifically for this course.
****************************************************************************/

int check4pangram(char input[], int size); //Checks the user's sentece to see if it contains all of the letters in the alphabet
int missingLetters(char missing[], int number); // Prints missing letters
int isTautogram(char letters[], int magnitude); // Checks to see if the user's first letter in each word in the sentence is the same

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
# define length 128 //Array length for user input

int main(int argc, const char * argv[])
{
    //Declarations
	char sentence[length] = {'\0'};
	int size;
    int i;
    
    //Initilizations
	size = 0;
    i = 0;

	for(int j = 1; argv[j] != NULL; j++) {
        if(argv[j+1] == NULL) {
            sprintf(sentence, "%s%s", sentence, argv[j] );
        } else {
            sprintf(sentence, "%s%s ", sentence, argv[j] );
        }
    }
    size = strlen(sentence)-1;  //Gets sentence length
    
    if (size == 0)
    {
        printf("No Input detected.\n\n");
        exit(0);
    } else if (size > 127) {
        printf("Input is too big, should be less than 128 characters.\n\n");
        exit(0);
    }

    //If a captial letter is entered, it is changed to lowercase
	for (i = 0; i <= size; i++)
	{
		if (sentence[i] >= 65 && sentence[i] <= 90)
		{
			sentence[i] = sentence[i] + 32;
		}
	}

    //Prints the sentence again
	fprintf(stdout,"The sentence is: %s\n\n", sentence);
    
    //Calls functions
	check4pangram(sentence, size);
	isTautogram(sentence, size);

 return 0;
}

//Checks the user's sentece to see if it contains all of the letters in the alphabet
int check4pangram (char input[], int numLetters)
{
    //Declarations
	int i;
	int j;
	int x;
	char alpha[27] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','\0'};

    //Initializations
	i = 0;
	j = 0;
	x = 0;

	x = strlen(alpha); //gets length of the alphabet

    /*Checks to see how many letters of the alphabet are in the sentence
    and changes them to a dot to remove them from the array */
	for (i = 0; i < numLetters; i ++)
	{
		for (j = 0; j <= 26; j ++)
		{
			if (input[i] == alpha[j])
		    {
                alpha[j] = '.';
				x --; //Subtracts from length of alphabet to show how many letters are missing
			}
        }
	}

    /*If the length of the alphabet is greater than 1, it is not a pangram
     because it doesn't contain all of the letters in the alphabet */
	if (x > 0)
	{
        fprintf(stdout,"This is not a pangram.\n");;
	    missingLetters(alpha, x); //prints the missing letters
    }
    /*All of the letters in the alphabet were entered by the user, so
     it is a pangram*/
	else if (x == 0)
	{
        fprintf(stdout,"This is a pangram.\n");
    }
	printf("\n");
	return 0;
}

// Prints missing letters
int missingLetters (char missing[], int number)
{
	fprintf(stdout,"The following %d letter(s) are missing: %s\n", number, missing);
    
	return 0;
}

// Checks to see if the user's first letter in each word in the sentence is the same
int isTautogram(char letters[], int magnitude)
{
    //Declarations
	int j;
	char t;
	int tautogram;

    //Initializations
	j = 0;
	tautogram = 0;
	t = letters[0];

    //Checks to see that all of the first letters in each of the words are the same
	for (j = 0; j <= magnitude; j ++)
	{
		if (isspace(letters[j]) && letters[j+1] == t)
		{
			tautogram = 1;
		}
		else if (isspace(letters[j]) && letters[j+1] != t)
		{
			tautogram = 0;
			break;
		}
	}

    //Basic Boolean logic for tautogram
	if (tautogram == 1)
	{
		fprintf(stdout,"this is a tautogram.\n");
	}
    else if (tautogram == 0)
	{
		fprintf(stdout,"this is not a tautogram.\n");
	}
    
	return 0;
}
