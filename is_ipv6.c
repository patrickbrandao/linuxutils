#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>

// testar se string esta no formato ipv6
int is_ipv6(char *str){
	struct in6_addr ipv6addr;
	if(!inet_pton(AF_INET6, str, &(ipv6addr))) return 1;
	return 0;
}

void help_std(){
	printf("\n");
	printf("is_ipv6\n");
	printf("Verifica se os parametros estao no formato de um endereco ipv6, retorno stdno=0 em caso de correto ou >0 se estiver incorreto\n");
	printf("\n");
	printf("Autor: Patrick Brandao <patrickbrandao@gmail.com>, todos os direitos reservados\n");
	printf("\n");
	printf("Use: is_ipv6 (ipv6) [ipv6] [ipv6] [...]\n");
	printf("\n");
	exit(1);
}

int main(int argc, char **argv){
	int i = 0;
    if(argc<2) help_std();
	for(i=1;i<argc;i++){
		int r = is_ipv6(argv[i]);
		if(r>0) return r;
	}
	return 0;
}



















