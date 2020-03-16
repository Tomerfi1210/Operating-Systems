/**************************
 * ex6a2: Prime Factor Server
 * ============================================================================
 * This program is Factor server.
 * opens socket reads input values from client then writes answer to client
 * through socket.
 *
 * Written by: Shahar Aviv- 203489307, Tomer Fisher- 205364151
 * LogIn: shaharav, tomerfi
 *
 * Compile: gcc -Wall ex6a2.c -o ex6a2
 * Run: ./ex6a2 <port number>
 * ============================================================================
 * Input: one number to do prime on - from client
 * ============================================================================
 * Output: factorization to the client
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

#define SIZE 100
int main_socket ;

void m_meset(struct addrinfo *con_kind); 
void primeFactors(int n, int *arr) ;
void sig_hand(int s) ;

int main(int argc, char *argv[])
{
	signal(SIGINT, sig_hand) ;

	int rc; // return code
	int serving_socket;
	int fd;
	fd_set rfd;
	fd_set c_rfd;


	struct addrinfo con_kind,
	*addr_info_res ;
	int arr[SIZE] ;
	int num ;

	m_meset(&con_kind) ;

	if ((rc = getaddrinfo(NULL, argv[1], &con_kind, &addr_info_res)) != 0) {
		fprintf(stderr, "(getaddrinfo() failed %s\n", gai_strerror(rc)) ;
		exit(EXIT_FAILURE) ;
	}

	main_socket = socket(addr_info_res -> ai_family, addr_info_res -> ai_socktype, addr_info_res -> ai_protocol);
	if (main_socket < 0) {
		perror("socket: allocation failed");
		exit(EXIT_FAILURE) ;
	}
	rc = bind(main_socket, addr_info_res-> ai_addr, addr_info_res-> ai_addrlen);
	if (rc) {
		perror("bind failed");
		exit(EXIT_FAILURE) ;
	}
	rc = listen(main_socket, 5);
	if (rc) {
		perror("listen failed");
		exit(EXIT_FAILURE) ;
	}

	FD_ZERO(&rfd);
	FD_SET(main_socket, &rfd);



	while (1) { //the main loop
		c_rfd = rfd;
		rc = select(getdtablesize(), &c_rfd, NULL, NULL, NULL);
		if (FD_ISSET(main_socket, &c_rfd)) {
			serving_socket = accept(main_socket,NULL, NULL);

			if (serving_socket >= 0) {
				FD_SET(serving_socket, &rfd);
			}
		}

		for(fd = main_socket + 1 ; fd < getdtablesize() ; fd++)
		{
			if (FD_ISSET(fd, &c_rfd)) {
				rc = read(fd, &num, sizeof(int)) ;
				if (rc == 0) {
					close(fd);
					FD_CLR(fd, &rfd);
				}
				else if(rc > 0)
				{
					primeFactors(num, arr) ;
					write(fd, &arr, sizeof(arr)) ;
				}
				else {
					perror("read() failed") ;
					exit(EXIT_FAILURE) ;
				}
			}
		}
	}

	return EXIT_SUCCESS ;

}
//-------------prime factore func----------------------
void primeFactors(int n, int *arr)
{
	int i, j = 0;

	// Print the number of 2s that divide n
	while (n%2 == 0)
	{
		arr[j] = 2 ;
		j++ ;
		n = n/2;
	}

	// n must be odd at this point.  So we can skip
	// one element (Note i = i +2)
	for (i = 3; i <= n/2; i = i+2)
	{
		// While i divides n, print i and divide n
		while (n%i == 0 )
		{
			arr[j] = i ;
			j++ ;
			n /= i;
		}
	}

	// This condition is to handle the case when n
	// is a prime number greater than 2
	if (n > 2)
	{
		arr[j] = n ;
		j++ ;
	}

	arr[j]=-1;
}
//--------------reset struc func----------------
void m_meset(struct addrinfo *con_kind){

	memset((*&con_kind), 0, sizeof(*con_kind)) ;
	(*con_kind).ai_family = AF_UNSPEC ;
	(*con_kind).ai_socktype = SOCK_STREAM ;
	(*con_kind).ai_flags = AI_PASSIVE ;
}
//----------sig hand func--------------------------

void sig_hand(int s)
{
	close(main_socket) ;
	exit(EXIT_SUCCESS) ;
}

