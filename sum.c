/******************************************************************************
 * This program receives a series of numbers and returns the sum
 * =============================================================================
 * Written by  	: Tomer Fisher & Shahar Aviv
 * login	: tomerfi / shaharav
 * Year	   		: B
 * =============================================================================
 * Input: series of numbers
 * -----------------------------------------------------------------------------
 * Output: sum of numbers
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
    int sum =0 ;

    if (argc < 2) //cheking argv input
    {
        fputs("bad argc", stderr);
        exit(EXIT_FAILURE);
    }

    const char d[2] = " ";
    char* token;

    token = strtok(argv[1], d);

    while (token != NULL)
    {
        if(!ifDigit(token)) //checking the values
        {
            puts("error blas");
            exit (EXIT_FAILURE);
        }
        sum+= atoi(token); //casting to int types and sum all
        token = strtok(NULL, d);
    }
    printf("%d\n",sum);

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
