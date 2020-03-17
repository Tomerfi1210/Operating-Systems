/****************************************************************************** * ex7a1: server program * ============================================================================ * The server do several actions. it can check if input string is all * lower case. it can sum the digit of a double. and it can count * how many times letters in first string are in the second one. * * * Written by: Shahar Aviv- 203489307, Tomer Fisher- 205364151 * LogIn: shaharav, tomerfi * * Compile: gcc ex7_svc.c ex7a1.c -lm -o server * Run: ./server * ============================================================================ * Input: arguments from client * ============================================================================ * Output: None * ============================================================================ * */
//--------------uncludes------------
#include <math.h>
#include <stdio.h>
#include "/usr/include/rpc/rpc.h"
#include "ex7.h"

//----------defines------------------
#define SIZE 100

//------------small letters function-----------------
int * small_letters_1_svc(char** msg, struct svc_req *foo)
{
    static int result=0;
	int i;
	result=0;
	int len = strlen(*msg);
	for (i=0; msg[0][i] != '\0'; i++)
	{
		if (msg[0][i] >='a' && msg[0][i]<='z')
		{
			result ++;
		}
	}
	//if all chars are lower case letters
	if(result == len)
		result =  1;
	else
		result = 0; 
	return (&result);
}
//-----------------------------------------------
int * number_sum_1_svc(double *x, struct svc_req *foo)
{
	static int result;
	result=0;
	long temp = (*x) * 100000000L;


		while (temp > 0)
	{
		result += temp % 10;
		temp = temp/10;
	}

	return &result;
}

//----------same letters function--------------------------------
int * same_letters_1_svc(char** msg, struct svc_req *foo)
{

	    static int result;
	    char word1[SIZE], word2[SIZE];
	    int i, j;
	    result = 0;
	    sscanf(*msg, "%s %s", word1, word2);

	    for (i=0; word1[i]!='\0'; i++)
	    {
	    	for(j=0; word2[j]!='\0'; j++)
	    	{
	    		if (word1[i] == word2[j])
	    		{
	    			result++;
				break;
	    		}
	    	}
	    }

	    return (&result);
}
//------------------------------------------------------------------
//------------------------------------------------------------------
