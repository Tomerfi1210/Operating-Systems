/**************************
 * ex6a1: GCD server
 * ============================================================================
 * This program is GCD server.
 * opens socket reads input values from client then writes answer to client
 * through socket.
 *
 * Written by: Shahar Aviv- 203489307, Tomer Fisher- 205364151
 * LogIn: shaharav, tomerfi
 *
 * Compile: gcc -Wall ex6a1.c -o ex6a1
 * Run: ./ex6a1
 * ============================================================================
 * Input: 2 numbers <port number>
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

#define SIZE 3
int main_socket;

void m_meset(struct addrinfo *con_kind) ;
int gcd(int a, int b) ;
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


	while (1) {
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
				rc = read(fd, &arr, sizeof(arr)) ;
				if (rc == 0) {
					close(fd);
					FD_CLR(fd, &rfd);
				}
				else if(rc > 0)
				{
					arr[0] = gcd(arr[1], arr[2]) ;
					int gcd = arr[0] ;
					write(fd, &gcd, sizeof(int)) ;
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
//----------------------------------------------
int gcd(int a, int b)
{
	int temp;
	while (b != 0)
	{
		temp = a % b;

		a = b;
		b = temp;
	}
	return a;
}
//------------------------------------------------
void m_meset(struct addrinfo *con_kind){

	memset((*&con_kind), 0, sizeof(*con_kind)) ;
	(*con_kind).ai_family = AF_UNSPEC ;
	(*con_kind).ai_socktype = SOCK_STREAM ;
	(*con_kind).ai_flags = AI_PASSIVE ;
}
//---------------------------------------------------
void sig_hand(int s)
{
	close(main_socket) ;
	exit(EXIT_SUCCESS) ;
}

