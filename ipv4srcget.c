#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#include <sys/types.h>
#include <sys/param.h>
#include <arpa/inet.h>
#include <ctype.h>

/*
	
	Recebe um ipv4 e retorna qual o ip local usado como ip de origem para enviar pacotes

*/

#define IPV4SRCGET_PORT 53


#define IPv4_PARTS 4
#define IPv4_MAXLEN 15

#define ZERO4(a, b)	{ a[0]=0; a[1]=0; a[2]=0; a[3]=0; b=0; }
//#define ZERO2NUM(n, a) { n = ; ZERO4(a); }

#define COLET(_readed, _read, _idx) if(_readed && _idx < 4){ int total; total = atoi(_read); nums[idx] = total; ZERO4(_read, _readed); }

void help_std(){
	printf("\n");
	printf("ipv4srcget\n");
	printf("Recebe um endereco IPv4, interpreta zeros a esquerda e omissoes e imprime na saida\n");
	printf("\n");
	printf("Autor: Patrick Brandao <patrickbrandao@gmail.com>, todos os direitos reservados\n");
	printf("\n");
	printf("Use: ipv4srcget [ipv4] [ipv4] [ipv4][...]\n");
	printf("\n");
	exit(1);	
}

// ler string e interpretar ipv4 a partir dela
static int inet_pton_ng(char *src){
	int nums[IPv4_PARTS];
	register int i, r, idx, j, readed;
	register char c;
	char *mystr;
	char read[4];

	for(i=0;i<IPv4_PARTS;i++) nums[i] = 0;
	
	c = *src;
	j = IPv4_MAXLEN;
	idx = 0;
	ZERO4(read, readed);

	while(j-- && idx < IPv4_PARTS){
		if(c=='.'){
			// se ha coleca de numeros, contabilizar no slot de idx
			COLET(readed, read, idx);
			c = *++src;
			idx++;
			continue;
		}
		if(isdigit(c)){
			if(readed<3) read[readed++] = c;
			c = *++src;
			continue;
		}
		// caracter desconhecido
		break;
	}
	
	// ultima sequencia
	COLET(readed, read, idx);

	// converter para decimal
	nums[1] = nums[1] << 8;
	nums[2] = nums[2] << 16;
	nums[3] = nums[3] << 24;

	r = nums[0] + nums[1] + nums[2] + nums[3];
	return r;
}

void ipv4srcget(unsigned char * find_ipaddr){
	struct sockaddr_in srv_sock;
	int netsocket = socket ( AF_INET, SOCK_DGRAM, 0);
	unsigned char buffer[100];
	unsigned char * ret = NULL;

	// erro ao criar socket para rede
	if(netsocket < 0){
		printf("\n");
		return;
	}

	// zerar socket e buffer
	memset( &srv_sock, 0, sizeof(srv_sock) );
	memset( buffer, 0, sizeof(buffer) );

	// configurar
	srv_sock.sin_family = AF_INET;
	srv_sock.sin_port = htons(IPV4SRCGET_PORT);

	// ler string e codificar em decimal 32 bits
	//- srv_sock.sin_addr.s_addr = inet_addr( find_ipaddr );
	srv_sock.sin_addr.s_addr = inet_pton_ng( find_ipaddr );

	// criar tomada de conexao/datagrama
	int err = connect(netsocket, (const struct sockaddr*) &srv_sock, sizeof(srv_sock) );

	struct sockaddr_in name;
	socklen_t namelen = sizeof(name);
	err = getsockname(netsocket, (struct sockaddr*) &name, &namelen);

	const char* p = inet_ntop(AF_INET, &name.sin_addr, buffer, 100);

	// fechar socket, buffer ja foi preenchido!
	close(netsocket);

	// verificar se foi possivel obter o tip de 'name'
	if(p != NULL){
		// obtido com sucesso!
		printf("%s\n", buffer);

	}else{
		// deu erro... falta de gateway ou problemas de rede nao configurada
		printf("\n");
	}
}


int main(int argc, char **argv){
	int i;
    if(argc!=2) help_std();
	ipv4srcget((unsigned char*) argv[1]);
}
