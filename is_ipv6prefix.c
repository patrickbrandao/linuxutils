#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>


// testar se string esta no formato de rede ipv6
int is_ipv6_prefix(char *str){
	int i, len;
	struct in6_addr ipv6addr;
	char addr[INET6_ADDRSTRLEN + 4];
	char comp[INET6_ADDRSTRLEN + 4];
	int idx1 = 0, idx2 = 0;
	int prefix = 0;
	int barcount = 0;

	// string vazia
	if(!str || !str[0]) return 1;

	// tamanho da string
	len=strlen(str);

	// Formatos validos esperados:
	// ::/0 												4 bytes
	// a
	// 1111:2222:3333:4444:aaaa:bbbb:cccc:dddd/128 			43 bytes
	if(len<4 || len > 43) return 2;

	// limpar a sala
	memset( &(addr), 0, sizeof(addr) );
	memset( &(comp), 0, sizeof(comp) );
	for(i=0; i < len; i++){
		char atch = str[i];

		// encontramos o prefixo
		if(atch == '/'){
			barcount++;
			if(barcount > 1) return 3;
			continue;
		}

		// copiar byte para o endereco ou para
		if(barcount){
			// estamos lendo a prefixo
			comp[idx2++] = atch;
		}else{
			// leitura de ipv6
			addr[idx1++] = atch;
		}
	}
	// printf("ADDR: '%s' COMP: '%s'\n", addr, comp);

	// parte de endereco e prefixo lidos, validar
	// 1 - o ip 
	if(!inet_pton(AF_INET6, addr, &(ipv6addr))) return 5;

	// 2 - prefixo
	if(!barcount || !idx2) return 6;
	prefix = atoi(comp);

	// printf("Prefixo: %d\n", prefix);

	if(prefix < 0 || prefix > 128) return 7;

	return 0;
}


void help_std(){
	printf("\n");
	printf("is_ipv6prefix\n");
	printf("Verifica se os parametros estao no formato de um endereco ipv6 com prefixo ::/prefix, retorno stdno=0 em caso de correto ou >0 se estiver incorreto\n");
	printf("\n");
	printf("Autor: Patrick Brandao <patrickbrandao@gmail.com>, todos os direitos reservados\n");
	printf("\n");
	printf("Use: is_ipv6prefix (ipv6/prefix) [ipv6/prefix] [ipv6/prefix] [...]\n");
	printf("\n");
	exit(1);
}

int main(int argc, char **argv){
	int i = 0;
    if(argc<2) help_std();
	for(i=1;i<argc;i++){
		int r = is_ipv6_prefix(argv[i]);
		if(r > 0) return r;
	}
	return 0;
}



















