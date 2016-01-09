#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>

// testar se string esta no formato de rede ipv6 com porta
int is_ipv6_port(char *str){
	int i, len;
	struct in6_addr ipv6addr;
	char addr[INET6_ADDRSTRLEN + 9];
	char comp[INET6_ADDRSTRLEN + 9];
	int idx1 = 0, idx2 = 0;
	int port = 0;
	int commacount = 0;

	// controle de "[ ipv6 ]:xx" em relacao ao '[' e ']'
	int _status = 0; // 0=nao aberto, 1=aberto, 2=fechado

	// string vazia
	if(!str || !str[0]) return 1;

	// tamanho da string
	len=strlen(str);

	// Formatos validos esperados:
	// [::]:0 												6 bytes
	// a
	// [1111:2222:3333:4444:aaaa:bbbb:cccc:dddd]:65535 		47 bytes
	if(len<6 || len > 47) return 2;

	// limpar a sala
	memset( &(addr), 0, sizeof(addr) );
	memset( &(comp), 0, sizeof(comp) );
	for(i=0; i < len; i++){
		char atch = str[i];

		// printf("ch=%c status=%d\n", atch, _status);

		// abrindo
		if(atch == '['){
			if(_status!=0) return 3;
			_status = 1; // abrindo
			continue;
		}
		// fechando
		if(atch == ']'){
			if(_status!=1) return 4;
			_status = 2; // fechando
			continue;
		}

		// encontramos a porta
		if(atch == ':'){
			if(_status==2){
				commacount++;
				if(commacount > 1) return 5;
				// nao coletar esse byte
				continue;
			}
		}

		// copiar byte para o endereco ou para
		if(commacount){
			// estamos lendo a porta
			comp[idx2++] = atch;
		}else{
			// leitura de ipv6
			// problema: nao estamos dentro do '[' .. ']'
			if(_status!=1) return 7;
			addr[idx1++] = atch;
		}
	}
	// printf("ADDR: '%s' COMP: '%s'\n", addr, comp);

	// parte de endereco e porta lidos, validar
	// 1 - o ip 
	if(!inet_pton(AF_INET6, addr, &(ipv6addr))) return 8;

	// 2 - porta
	if(!commacount || !idx2) return 9;
	port = atoi(comp);
	if(port < 1 || port > 65535) return 10;

	return 0;
}

void help_std(){
	printf("\n");
	printf("is_ipv6port\n");
	printf("Verifica se os parametros estao no formato de um endereco ipv6 com porta [::]:port, retorno stdno=0 em caso de correto ou >0 se estiver incorreto\n");
	printf("\n");
	printf("Autor: Patrick Brandao <patrickbrandao@gmail.com>, todos os direitos reservados\n");
	printf("\n");
	printf("Use: is_ipv6port ([ipv6]:port) [[ipv6]:port] [[ipv6]:port] [...]\n");
	printf("\n");
	exit(1);
}

int main(int argc, char **argv){
	int i = 0;
    if(argc<2) help_std();
	for(i=1;i<argc;i++){
		int r = is_ipv6_port(argv[i]);
		if(r>0) return r;
	}
	return 0;
}



















