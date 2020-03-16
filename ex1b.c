/******************************************************************************
 * This program recieves operation from user- plus,minus,sum,max.
 * the program creates child process and operate the specific operation
 * =============================================================================
 * Written by  	: Tomer Fisher & Shahar Aviv
 * login	: tomerfi / shaharav
 * Year	   		: B
 * =============================================================================
 * Input: operation from user
 * -----------------------------------------------------------------------------
 * Output: outcome of the specific operation
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>
#include <wait.h>

//-----------define consts----------------
#define MAX_STR_LEN 1000

//--------prototypes--------------------
bool validChoice(const char* userChoice);
void operateChoice(const char* userChoice);
void operatePluse(void);
void operateMinus(void);
void operateSum(void);
void operateMax(void);

//-----------main suction---------------------
int main()
{
    
    puts("Hi you! choose some operation and start to play!");
    
    char userChoice[MAX_STR_LEN];
    scanf("%s", userChoice);
    
    while (strcmp(userChoice, "exit") != 0)
    {
        
        
        //----------------creation process---------------------------------
        pid_t process = fork(); //gave birth
        
        if(process < 0)
        {
            perror("cannot fork");
            exit(EXIT_FAILURE);
        }
        
        //----------------------process for child---------------------------
        if(process == 0)
        {
            operateChoice(userChoice);
            exit(EXIT_SUCCESS);
        }
        
        wait(NULL);
        
        scanf("%s", userChoice);
    }
    
    return 0;
}


//-----------operate action by user choice--------------------------
void operateChoice(const char* userChoice)
{
    
    if (strcmp(userChoice, "plus") == 0)
    {
        operatePluse();
    }
    
    else if (strcmp(userChoice, "minus") == 0)
    {
        operateMinus();
    }
    
    else if (strcmp(userChoice, "sum") == 0)
    {
        operateSum();
    }
    
    else if (strcmp(userChoice, "max") == 0)
    {
        operateMax();
    }
    else
        fputs("invalid command\n", stderr); //error if invalid operation from user
    
}
//-------------pluse operation-----------------------
void operatePluse()
{
    puts("give me 2 numbers please");
    char num1[MAX_STR_LEN], num2[MAX_STR_LEN];
    
    scanf("%s %s", num1, num2);
    char *argv[] = {"plus", num1, num2, NULL};
    if (execvp("./plus", argv) != 0) //starting new program 
    {
        perror("execvp() failed");
        exit(EXIT_FAILURE);
    }
}
//-------------minus operation-----------------------
void operateMinus()
{
    puts("give me 2 numbers please");
    char num1[MAX_STR_LEN], num2[MAX_STR_LEN];
    
    scanf("%s %s", num1, num2);
    if (execlp("./minus", "minus", num1, num2, NULL) != 0) //starting new program 
    {
        perror("execvp() failed");
        exit(EXIT_FAILURE);
    }
}
//-------------sum operation-----------------------
void operateSum()
{
    puts("give some numbers please with space between them");
    char numbers[MAX_STR_LEN];
    getchar();
    scanf("%[^\n]", numbers);
    if (execlp("./sum", "sum", numbers, NULL) != 0) //starting new program 
    {
        perror("execvp() failed");
        exit(EXIT_FAILURE);
    }
}
//-------------max operation-----------------------
void operateMax()
{
    puts("give some numbers please with space between them");
    char numbers[MAX_STR_LEN];
    getchar();     //getting the line

    scanf("%[^\n]", numbers);
    char* newArr[MAX_STR_LEN];
    
    const char d[2] = " ";
    char* token;
    int i = 1;
    
    token = strtok(numbers, d); // using of strtok call
    
    newArr[0] = "max";
    while (token != NULL)
    {
        newArr[i] = token;
        token = strtok(NULL, d);
        i++;
    }
    newArr[i] = NULL;
    
    if (execvp("./max", newArr) != 0) //starting new program 
    {
        perror("execvp() failed");
        exit(EXIT_FAILURE);
    }

}
