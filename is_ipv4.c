#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>

// testar se string esta no formato ipv4
// porta na mesma declaracao
int is_ipv4(char *str){
	struct in_addr ipv4addr;
	if(!inet_pton(AF_INET, str, &(ipv4addr))) return 1;
	return 0;
}

void help_std(){
	printf("\n");
	printf("is_ipv4\n");
	printf("Verifica se os parametros estao no formato de um endereco ipv4, retorno stdno=0 em caso de correto ou >0 se estiver incorreto\n");
	printf("\n");
	printf("Autor: Patrick Brandao <patrickbrandao@gmail.com>, todos os direitos reservados\n");
	printf("\n");
	printf("Use: is_ipv4 (ipv4) [ipv4] [ipv4] [...]\n");
	printf("\n");
	exit(1);
}

int main(int argc, char **argv){
	int i = 0;
    if(argc<2) help_std();
	for(i=1;i<argc;i++){
		int r = is_ipv4(argv[i]);
		if(r>0) return r;
	}
	return 0;
}



















