
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

struct hostent *he;
struct in_addr a;

int main (int argc, char **argv){
	if (argc != 2){
		fprintf(stderr, "usage: %s hostname\n", argv[0]);
		return 1;
	}
	he = gethostbyname (argv[1]);
	if (he){
		while (*he->h_addr_list){
			bcopy(*he->h_addr_list++, (char *) &a, sizeof(a));
			printf("%s\n", inet_ntoa(a));
			return 0;
		}
	}else return 1;
	return 2;
}
