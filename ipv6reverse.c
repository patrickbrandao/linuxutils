#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>

#include <string.h>

void help_std(){
	printf("\n");
	printf("ipv6reverse\n");
	printf("Recebe um endereco IPv6 e imprime a notacao para DNS reverso\n");
	printf("\n");
	printf("Autor: Patrick Brandao <patrickbrandao@gmail.com>, todos os direitos reservados\n");
	printf("\n");
	printf("Use: ipv6reverse (ipv6) [numero de nivel]\n");
	printf("\n");
	exit(1);	
}

int main(int argc, char **argv){
    struct sockaddr_in6 sa;
	int i;
	int bytestoread = 32;
    char str[INET6_ADDRSTRLEN];
    char rev[INET6_ADDRSTRLEN];
    if(argc<2 || argc > 3) help_std();

	bzero(str, INET6_ADDRSTRLEN);
	bzero(rev, INET6_ADDRSTRLEN);

	if(argc==3) bytestoread = atoi(argv[2]);
	if(bytestoread < 1 || bytestoread > 32) bytestoread = 32;

	// ler de string para strct 128 bits
    inet_pton(AF_INET6, argv[1], &(sa.sin6_addr));

	// converter struct de 128 bits em notacao hexadecimal completa
	sprintf(str, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
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
	
	// inverter bytes
	for(i=0;i<32;i++) rev[i] = str[32-i-1];

	for(i = 32 - bytestoread; rev[i]; i++) printf("%c.", rev[i]);
	printf("ip6.arpa.\n");
}


