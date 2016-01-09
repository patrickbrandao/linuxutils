#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>


// testar se string esta no formato ipv4
// se is_prefix for 1, sera verificado se a mascara foi informada em numero de bits
// se have_port for 1, sera verificado se a porta foi informada, mas nao e' possivel ter o prefixo e a
// porta na mesma declaracao
int is_ipv4_port(char *str){
	int i, len;
	struct in_addr ipv4addr;
	char addr[INET_ADDRSTRLEN];
	char comp[INET_ADDRSTRLEN];
	int idx1 = 0, idx2 = 0;
	int port = 0;
	int dotcount = 0;
	int comacount = 0;

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
		if( atch=='/' ) return 4;

		// ponto
		if(atch == '.'){
			dotcount++;
			if(dotcount > 3) return 5;
		}
		// separador de porta
		if(atch == ':'){
			comacount++;
			if(comacount > 1) return 6;
			continue;
		}

		// copiar byte para o endereco ou para
		if(comacount){
			// leitura do ip
			comp[idx2++] = atch;
		}else{
			// leitura de porta
			addr[idx1++] = atch;
		}
	}

	// parte de endereco e porta lidos, validar
	// 1 - o ip 
	if(!inet_pton(AF_INET, addr, &(ipv4addr))) return 7;

	// 2 - porta
	port = atoi(comp);
	if(port < 1 || port > 65535) return 8;

	return 0;
}

void help_std(){
	printf("\n");
	printf("is_ipv4port\n");
	printf("Verifica se os parametros estao no formato de um endereco ipv4 com porta, retorno stdno=0 em caso de correto ou >0 se estiver incorreto\n");
	printf("\n");
	printf("Autor: Patrick Brandao <patrickbrandao@gmail.com>, todos os direitos reservados\n");
	printf("\n");
	printf("Use: is_ipv4port (ipv4:port) [ipv4:port] [ipv4:port] [...]\n");
	printf("\n");
	exit(1);
}

int main(int argc, char **argv){
	int i = 0;
    if(argc<2) help_std();
	for(i=1;i<argc;i++){
		int r = is_ipv4_port(argv[i]);
		if(r>0) return r;
	}
	return 0;
}



















