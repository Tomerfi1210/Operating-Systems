/******************************************************************************
 * This program receives 2 numbers and returns the difference 
 * =============================================================================
 * Written by  	: Tomer Fisher & Shahar Aviv
 * login	: tomerfi / shaharav
 * Year	   		: B
 * =============================================================================
 * Input: 2 numbers
 * -----------------------------------------------------------------------------
 * Output: difference between numbers
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
//------------prototypes-------------------
bool ifDigit(char arr[]);


//----------main suction---------------------
int main(int argc, char* argv[])
{
    if (argc !=3) //cheking argv input
    {
        fputs("bad argc", stderr);
        exit(EXIT_FAILURE);
    }

    int i;
    for (i=1; i<argc; i++) 
    {
        if (!ifDigit(argv[i])) //checking the values
        {
            puts("error");
            exit (EXIT_FAILURE);
        }

    }

    int num1 = atoi(argv[1]); //casting to int types
    int num2 = atoi(argv[2]);

    int difference = num1-num2;

    printf("%d\n", difference);

    return 0;
}

//--------checking if value is digit---------------

bool ifDigit(char arr[])
{
    int i;
    char c;
    for (i=0,c=arr[i];c!='\0'; ++i,c=arr[i])
    {
        if(!isdigit(c))
            return false;
    }
    return true;
}
