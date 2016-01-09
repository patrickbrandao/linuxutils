#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>

// testar se string esta no formato de rede ipv4
int is_ipv4_prefix(char *str){
	int i, len;
	struct in_addr ipv4addr;
	char addr[INET_ADDRSTRLEN];
	char comp[INET_ADDRSTRLEN];
	int idx1 = 0, idx2 = 0;
	int prefix = 0;
	int barcount = 0;

	// string vazia
	if(!str || !str[0]) return 1;

	// tamanho da string
	len=strlen(str);

	// Formatos validos esperados:
	// 0.0.0.0:1 				9 bytes
	// a
	// 255.255.255.255:65535 	21 bytes
	if(len<9 || len > 21) return 2;

	// limpar a sala
	memset( &(addr), 0, sizeof(addr) );
	memset( &(comp), 0, sizeof(comp) );
	for(i=0; i < len; i++){
		char atch = str[i];

		// nao pode comecar caracter diferente de numero
		if(!i && !isdigit(atch)) return 3;

		// caracter proibidos de acordo com o tipo de requisicao
		if( atch==':' ) return 4;

		// separador de prefixo
		if(atch == '/'){
			barcount++;
			if(barcount > 1) return 6;
			continue;
		}

		// copiar byte para o endereco ou para
		if(barcount){
			// leitura do prefixo
			comp[idx2++] = atch;
		}else{
			// leitura de ip
			addr[idx1++] = atch;
		}
	}

	// parte de endereco e porta lidos, validar
	// 1 - o ip 
	if(!inet_pton(AF_INET, addr, &(ipv4addr))) return 7;

	// 2 - prefixo
	if(!barcount || !idx2) return 8;
	prefix = atoi(comp);
	if(prefix < 0 || prefix > 32) return 9;

	return 0;
}


void help_std(){
	printf("\n");
	printf("is_ipv4prefix\n");
	printf("Verifica se os parametros estao no formato de um endereco ipv4 com prefixo de rede, retorno stdno=0 em caso de correto ou >0 se estiver incorreto\n");
	printf("\n");
	printf("Autor: Patrick Brandao <patrickbrandao@gmail.com>, todos os direitos reservados\n");
	printf("\n");
	printf("Use: is_ipv4prefix (ipv4/prefix) [ipv4/prefix] [ipv4/prefix] [...]\n");
	printf("\n");
	exit(1);
}

int main(int argc, char **argv){
	int i = 0;
    if(argc<2) help_std();
	for(i=1;i<argc;i++){
		int r = is_ipv4_prefix(argv[i]);
		if(r>0) return r;
	}
	return 0;
}



