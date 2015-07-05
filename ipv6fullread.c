#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>

void help_std(){
	printf("\n");
	printf("ipv6fullread\n");
	printf("Recebe um endereco IPv6 e imprime endereco no formato completo, aceita varios enderecos de entrada\n");
	printf("\n");
	printf("Autor: Patrick Brandao <patrickbrandao@gmail.com>, todos os direitos reservados\n");
	printf("\n");
	printf("Use: ipv6fullread (ipv6) [ipv6] [ipv6] [...]\n");
	printf("\n");
	exit(1);	
}


void ipv6fullread(char *ipv6str){
    struct sockaddr_in6 sa;

	// ler de string para strct 128 bits
    inet_pton(AF_INET6, ipv6str, &(sa.sin6_addr));

	// converter struct de 128 bits em notacao hexadecimal completa
	printf("%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x\n",
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

int main(int argc, char **argv){
	int i = 0;
    if(argc<2) help_std();
	for(i=1;i<argc;i++) ipv6fullread(argv[i]);
}


