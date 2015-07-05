#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>

int main(int argc, char *argv[]){
	struct addrinfo hint, *res = NULL;
	int ret;
	
	memset(&hint, '\0', sizeof hint);
	
	hint.ai_family = PF_UNSPEC;
	hint.ai_flags = AI_NUMERICHOST;
	
	ret = getaddrinfo(argv[1], NULL, &hint, &res);
	if (ret) {
		printf("unk\n");
	}else{
		if(res->ai_family == AF_INET) {
			printf("4\n");
		} else if (res->ai_family == AF_INET6) {
			printf("6\n");
		} else {
			printf("unk\n");
		}
		freeaddrinfo(res);
	}
	return 0;
}
