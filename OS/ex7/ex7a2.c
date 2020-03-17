/******************************************************************************
 * ex7a2: client program
 * ============================================================================a
 * the client gets operation from user and print the result with server help
 *
 * Written by: Shahar Aviv- 203489307, Tomer Fisher- 205364151
 * LogIn: shaharav, tomerfi
 *
 * Compile: gcc ex7_clnt.c ex7a2.c -o client
 * Run: ./client
 * ============================================================================
 * Input: the right operation from user and the numbers that it needs
 * ============================================================================
 * Output: the result
 * ============================================================================
 * */
//--------------uncludes------------
#include <stdio.h>
#include "/usr/include/rpc/rpc.h"
#include "ex7.h"

//----------defines------------------

#define SIZE 100
//---------variables-----------------
int *result;
double message_num;
char *server;
char message[SIZE];
char word1[SIZE], word2[SIZE];
//---------------------------------------
void error_check(int *result,CLIENT * cl, char *server);
//------------main section-------------
int main(int argc, char** argv)
{
	if (argc != 2)
	{
		fprintf(stderr, "usage: %s <host> <ip address>\n",argv[0]);
		exit(EXIT_FAILURE);
	}

	CLIENT *cl;
	int user_choice=-1;

	/* Save values of command line arguments */
	server = argv[1];

	cl = clnt_create(server, 17, 1, "tcp");
	if (cl == NULL)
	{
		/* Couldn't establish connection with server.
		 * Print error message and die. */
		clnt_pcreateerror (server);
		exit(EXIT_FAILURE);
	}

	while (user_choice != 0)
	{puts("plese choose operations between 1-3 or 0 to finish");
	scanf("%d", &user_choice);
	message[0] = '\0';
	switch (user_choice)
	{
	case 1:
	{
		puts("insert a string plese");
		scanf("%s", message);
		char * tmp = &message[0];
		//Call the remote procedure "printmessage" on the server
		result = small_letters_1(&tmp, cl);
		error_check(result, cl, server);

		if(*result == 0)
			{
			  puts("there is no small letters in string\n");
			}
			puts("there is small letters in string\n");

		break;
	}
	case 2:
	{
		puts("insert a number to sum the digits plese");
		scanf("%lf", &message_num);
		result = number_sum_1(&message_num, cl);
		error_check(result, cl, server);

		printf("sum of digits = %d\n\n", *result) ;
		break;
	}
	case 3:
	{
		puts("insert 2 strings to check\n");
		scanf("%s %s", word1, word2);

		strcat(message, word1);
		strcat(message, " ");
		strcat(message, word2);
		char * temp = &message[0];
		result = same_letters_1(&temp, cl);
		error_check(result, cl, server);

		printf("counter of same letters is = %d\n\n", *result) ;

		break;

	}

	}
	}
	return 0;

}
//--------------error check function-------------------------------
void error_check(int *result,CLIENT * cl, char *server)
{
	if (result == NULL)
	{
		/* An error  occurred while calling the server.
		 * Print error message and die. */
		clnt_perror (cl, server);
		exit(EXIT_FAILURE);
	}
}
//------------------------------------------------------------------
//------------------------------------------------------------------



