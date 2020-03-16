/******************************************************************************
 * This program receives a series of numbers and returns the max value
 * =============================================================================
 * Written by  	: Tomer Fisher & Shahar Aviv
 * login	: tomerfi / shaharav
 * Year	   		: B
 * =============================================================================
 * Input: series of numbers
 * -----------------------------------------------------------------------------
 * Output: max value
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <memory.h>

//------------prototypes-------------------
bool ifDigit(char arr[]);


//----------main suction---------------------
int main(int argc, char* argv[])
{
    int maxValue=0;

    if (argc < 2) //cheking argv input
    {
        fputs("bad argc", stderr);
        exit(EXIT_FAILURE);
    }
    int i = 1;
    while (argv[i] != NULL)
    {
        if(!ifDigit(argv[i])) //checking the values
        {
            fputs("error", stderr);
            exit(EXIT_FAILURE);
        }

        int value = atoi(argv[i]); //casting to int types

        if (value > maxValue)
        {
            maxValue = value;  //save max value
        }

        i++;
    }

    printf("%d\n", maxValue);
	return 0;
}

//--------checking if value is digit---------------

bool ifDigit(char arr[])
{
    int i;
    char c;
    for (i=0,c=arr[i];c!='\0'; ++i,c=arr[i])
    {
        if (arr[0]=='-')
        {
            continue;
        }

        if(!isdigit(c))
            return false;
    }
    return true;
}
