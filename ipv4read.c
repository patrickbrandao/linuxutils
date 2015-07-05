#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#include <sys/types.h>
#include <sys/param.h>
#include <arpa/inet.h>
#include <ctype.h>

/*
	
	Receber ip no primeiro parametro e retornar notacao decimal perfeita, principalmente
	evitando interpretacao errada do zero a esquerda
	
	valores do usuario:
	
	192.168..1
	192.168.01.1
	192.168.001.01
	200...1
	
	500.1.2.3

*/
#define IPv4_PARTS 4
#define IPv4_MAXLEN 15

#define ZERO4(a, b)	{ a[0]=0; a[1]=0; a[2]=0; a[3]=0; b=0; }
//#define ZERO2NUM(n, a) { n = ; ZERO4(a); }

#define COLET(_readed, _read, _idx) if(_readed && _idx < 4){ int total; total = atoi(_read); nums[idx] = total; ZERO4(_read, _readed); }

void help_std(){
	printf("\n");
	printf("ipv4read\n");
	printf("Recebe um endereco IPv4, interpreta zeros a esquerda e omissoes e imprime na saida\n");
	printf("\n");
	printf("Autor: Patrick Brandao <patrickbrandao@gmail.com>, todos os direitos reservados\n");
	printf("\n");
	printf("Use: ipv4read [ipv4] [ipv4] [ipv4][...]\n");
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

void ipv4read(char* ipv4str){
    struct sockaddr_in sa;
    char str[INET_ADDRSTRLEN];

	// ler ip em argv[1]
	sa.sin_addr.s_addr = inet_pton_ng(ipv4str);
	inet_ntop(AF_INET, &(sa.sin_addr), str, INET_ADDRSTRLEN);
	printf("%s\n", str);

}

int main(int argc, char **argv){
	int i;
    if(argc<2) help_std();
	for(i=1;i<argc;i++) ipv4read(argv[i]);
}
