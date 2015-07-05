#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>

void help_std(){
	printf("\n");
	printf("ipv6bytesread\n");
	printf("Recebe um endereco IPv6 e imprime os bytes em decimal\n");
	printf("\n");
	printf("Autor: Patrick Brandao <patrickbrandao@gmail.com>, todos os direitos reservados\n");
	printf("\n");
	printf("Use: ipv6bytesread (ipv6)\n");
	printf("\n");
	exit(1);	
}

int main(int argc, char **argv){
    struct sockaddr_in6 sa;
    char str[INET6_ADDRSTRLEN];
    if(argc<2 || argc > 2) help_std();

	// ler de string para strct 128 bits
    inet_pton(AF_INET6, argv[1], &(sa.sin6_addr));

	// converter struct de 128 bits em notacao hexadecimal completa
	printf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
			sa.sin6_addr.s6_addr[0],
			sa.sin6_addr.s6_addr[1],
			sa.sin6_addr.s6_addr[2],
			sa.sin6_addr.s6_addr[3],
			sa.sin6_addr.s6_addr[4],
			sa.sin6_addr.s6_addr[5],
			sa.sin6_addr.s6_addr[6],
			sa.sin6_addr.s6_addr[7],
			sa.sin6_addr.s6_addr[8],
			sa.sin6_addr.s6_addr[9],
			sa.sin6_addr.s6_addr[10],
			sa.sin6_addr.s6_addr[11],
			sa.sin6_addr.s6_addr[12],
			sa.sin6_addr.s6_addr[13],
			sa.sin6_addr.s6_addr[14],
			sa.sin6_addr.s6_addr[15]
		);		

}


