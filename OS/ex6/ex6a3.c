/**************************
 * ex6a3: Client
 * ============================================================================
 * the program is a client and 2 servers. 
 * every server has its own socket.
 * the client sends the input to some server for getting the spacific result 
 *
 * Written by: Shahar Aviv- 203489307, Tomer Fisher- 205364151
 * LogIn: shaharav, tomerfi
 *
 * Compile: gcc -Wall ex6a3.c -o ex6a3
 * Run: ./ex6a3 <ip GCD> <port GCD> <ip factor> <port factor>
 * ============================================================================
 * Input: the client get values from args
 * ============================================================================
 * Output: print the spacific result from the spacific server
 * ============================================================================
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>        // for memset
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>


#define SIZE1 3
#define SIZE2 100


void check_socket(const int) ;
void m_meset(struct addrinfo *con_kind) ;

int main(int argc, char *argv[])
{
	int rc, my_socket, choice, i ;
	int gcd_arr[SIZE1] , prime[SIZE2] ;
	struct addrinfo con_kind, *addr_info_res ;


	while(1)
	{
		do{
			puts("Please enter your choice 0 to GCD or 1 to primeFactor") ;
			scanf("%d", &choice) ;
		}while(choice != 0 && choice != 1 && choice != 2) ;

		m_meset(&con_kind) ;

		switch(choice)
		{
		case 0: // for GCD

			if ((rc = getaddrinfo(argv[1], argv[2], &con_kind, &addr_info_res) != 0)) {
				fprintf(stderr, "(getaddrinfo() failed %s\n", gai_strerror(rc)) ;
				exit(EXIT_FAILURE) ;
			}

			my_socket = socket(addr_info_res->ai_family, // PF_INET
					addr_info_res->ai_socktype, // SOCK_STREAM
					addr_info_res->ai_protocol);

			check_socket(my_socket) ;

			if((rc = connect(my_socket, addr_info_res->ai_addr, addr_info_res->ai_addrlen)) > 0){
				perror("cannon connect") ;
				exit(EXIT_FAILURE) ;
			}

			puts("Please enter 2 numbers") ;
			scanf("%d %d", &gcd_arr[1], &gcd_arr[2]) ;
			write(my_socket, &gcd_arr, sizeof(gcd_arr)) ;

			rc = read(my_socket, &gcd_arr[0], sizeof(int)) ;
			printf("result = %d\n", gcd_arr[0]);

			break ;

		case 1: // for prime factors

			if ((rc = getaddrinfo(argv[3], argv[4], &con_kind, &addr_info_res) != 0)) {
				fprintf(stderr, "(getaddrinfo() failed %s\n", gai_strerror(rc)) ;
				exit(EXIT_FAILURE) ;
			}

			my_socket = socket(addr_info_res->ai_family, // PF_INET
					addr_info_res->ai_socktype, // SOCK_STREAM
					addr_info_res->ai_protocol);

			check_socket(my_socket) ;

			if((rc = connect(my_socket, addr_info_res->ai_addr, addr_info_res->ai_addrlen)) > 0){
				perror("cannon connect") ;
				exit(EXIT_FAILURE) ;
			}
			puts("Please enter 1 number") ;
			int num ;
			scanf("%d", &num) ;
			write(my_socket, &num, sizeof(int)) ;

			rc = read(my_socket, prime, sizeof(prime)) ;
			for(i = 0 ; prime[i] != -1 ; i++)
				printf("result = %d\n", prime[i]) ;

			break ;

		case 2:

			close(my_socket);
			freeaddrinfo(addr_info_res) ;
			exit(EXIT_SUCCESS);
		}

		close(my_socket);
	}

	freeaddrinfo(addr_info_res) ;

	return EXIT_SUCCESS ;
}
//----------check socket func-----------------
void check_socket(const int my_socket){
	if (my_socket < 0) {
		perror("socket: allocation failed");
		exit(EXIT_FAILURE) ;
	}
}
//---------reset the struct func-------------------
void m_meset(struct addrinfo *con_kind){

	memset((*&con_kind), 0, sizeof(*con_kind)) ;
	(*con_kind).ai_family = AF_UNSPEC ;
	(*con_kind).ai_socktype = SOCK_STREAM ;
	(*con_kind).ai_flags = AI_PASSIVE ;
}
//------------------------------------------------------


