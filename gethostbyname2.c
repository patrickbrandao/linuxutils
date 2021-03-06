
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
	char buf[INET6_ADDRSTRLEN];
	bzero(buf, INET6_ADDRSTRLEN);

	he = gethostbyname2(argv[1], AF_INET6);
	if (he){
		printf("FQDN=%s\n", he->h_name);
		while (*he->h_aliases) printf("CNAME=%s\n", *he->h_aliases++);
		while (*he->h_addr_list){
			//bcopy(*he->h_addr_list++, (char *) &a, sizeof(a));
			inet_ntop(AF_INET6, *he->h_addr_list++, buf, sizeof(buf));
			printf("IPV6=%s\n", buf);
		}
	}else herror(argv[0]);
	return 0;
}
