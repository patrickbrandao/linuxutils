
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>

void help_std(){
	printf("\n");
	printf("ipv6read\n");
	printf("Recebe um endereco IPv6 e imprime endereco no formato completo, aceita varios enderecos de entrada\n");
	printf("\n");
	printf("Autor: Patrick Brandao <patrickbrandao@gmail.com>, todos os direitos reservados\n");
	printf("\n");
	printf("Use: ipv6read (ipv6) [ipv6] [ipv6] [...]\n");
	printf("\n");
	exit(1);	
}

void ipv6read(char *ipv6str){
    struct sockaddr_in6 sa;
    char str[INET6_ADDRSTRLEN];

	// ler de string para strct 128 bits
    inet_pton(AF_INET6, ipv6str, &(sa.sin6_addr));

	// converter struct de 128 bits em notacao string
    inet_ntop(AF_INET6, &(sa.sin6_addr), str, INET6_ADDRSTRLEN);
    printf("%s\n", str);

}

int main(int argc, char **argv){
	int i = 0;
    if(argc<2) help_std();
	for(i=1;i<argc;i++) ipv6read(argv[i]);
}


