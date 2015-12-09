#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <stdint.h>

#define IPV6GROUPS 8

// ipv6 em 8 grupos decimais
int ipv6dec[8];

// struct ipv6 oficial
struct in6_addr ipv6addr;

// representacao string
char ipv6str[INET6_ADDRSTRLEN];

void help_std(){
	printf("\n");
	printf("ipv6_padd\n");
	printf("Recebe um prefixo IPv6 e o numero que devera ser adicionado ao prefixo, retorna prefixo resultante,\n");
	printf("\tapenas os primeiros 64 bits sao relevantes.\n");
	printf("\n");
	printf("Autor: Patrick Brandao <patrickbrandao@gmail.com>, todos os direitos reservados\n");
	printf("\n");
	printf("Use: ipv6_padd (ipv6) (num)\n");
	printf("\n");
	exit(1);
}

/*
	GGGG:CCCC:ISPX:PREF:none:none:none:none

	8 grupos que vao de 0 a 65535 em hexadecimal

void read_prefix(char *prefix){
	int i, j, k, m, len;

	// buffer de leitura do grupo
	char buf[5];

	// limpar
    for(i=0;i<8;i++)ipv6dec[i]=0;
    len = strlen(prefix);

}

ipv6addr.s6_addr[0],
ipv6addr.s6_addr[1],
ipv6addr.s6_addr[2],
ipv6addr.s6_addr[3],
ipv6addr.s6_addr[4],
ipv6addr.s6_addr[5],
ipv6addr.s6_addr[6],
ipv6addr.s6_addr[7],
ipv6addr.s6_addr[8],
ipv6addr.s6_addr[9],
ipv6addr.s6_addr[10],
ipv6addr.s6_addr[11],
ipv6addr.s6_addr[12],
ipv6addr.s6_addr[13],
ipv6addr.s6_addr[14],
ipv6addr.s6_addr[15]
*/
/*
// construir numero de 64 bits com o prefixo ipv6
int64_t prefix64 = 0;
// construir o salto em 64 bits
int64_t inc = (int64_t) number;
prefix64 &= ipv6addr.s6_addr[0] << 64;
prefix64 &= ipv6addr.s6_addr[1] << 56;
prefix64 &= ipv6addr.s6_addr[2] << 48;
prefix64 &= ipv6addr.s6_addr[3] << 40;
prefix64 &= ipv6addr.s6_addr[4] << 32;
prefix64 &= ipv6addr.s6_addr[5] << 24;
prefix64 &= ipv6addr.s6_addr[6] << 16;
prefix64 &= ipv6addr.s6_addr[7] << 8;

prefix64 &= ipv6addr.s6_addr[0] << 56;
prefix64 &= ipv6addr.s6_addr[1] << 48;
prefix64 &= ipv6addr.s6_addr[2] << 40;
prefix64 &= ipv6addr.s6_addr[3] << 32;
prefix64 &= ipv6addr.s6_addr[4] << 24;
prefix64 &= ipv6addr.s6_addr[5] << 16;
prefix64 &= ipv6addr.s6_addr[7];
printf("blabla %lu\n", prefix64);
*/


void ipv6read_prefix(char *ipv6str_input){
    int i;

	// ler de string para strct 128 bits
    inet_pton(AF_INET6, ipv6str_input, &(ipv6addr));

    // zerar todas as casas apos primeiros 64 bits
    for(i=8; i < 16; i++) ipv6addr.s6_addr[i] = 0;

}

void print_ipv6full(){
	printf("%02x%02x:%02x%02x:%02x%02x:%02x%02x\n",
		ipv6addr.s6_addr[0],
		ipv6addr.s6_addr[1], //-
		ipv6addr.s6_addr[2],
		ipv6addr.s6_addr[3], //-
		ipv6addr.s6_addr[4],
		ipv6addr.s6_addr[5], //-
		ipv6addr.s6_addr[6],
		ipv6addr.s6_addr[7]  //-
/*
		ipv6addr.s6_addr[8],
		ipv6addr.s6_addr[9],
		ipv6addr.s6_addr[10],
		ipv6addr.s6_addr[11],
		ipv6addr.s6_addr[12],
		ipv6addr.s6_addr[13],
		ipv6addr.s6_addr[14],
		ipv6addr.s6_addr[15]
*/
	);	
}


int main(int argc, char **argv){
	char *ipv6 = NULL;
	long int number = 0;
	int tmp;
	int i = 0;

	// parametros incorretos
    if(argc!=3) help_std();


    // ler ipv6
   	ipv6read_prefix(argv[1]);

   	// numero
   	number = atoi(argv[2]);

   	//printf("PRE OP\n");
    inet_ntop(AF_INET6, &(ipv6addr), ipv6str, INET6_ADDRSTRLEN);
    //printf("%s\n", ipv6str);
	//print_ipv6full();

   	// efetuar operacao
	if(number > 0){
		tmp = number;
		int idx, dif;
		while(tmp > 0){
			idx = 7;

			// quantos numeros podemos incrementar num salto
			dif = tmp - ipv6addr.s6_addr[idx];
			if(dif > 0 && dif < 255){
				ipv6addr.s6_addr[idx] += dif;
				tmp -= dif;
				continue;
			}

			//printf(" A ");print_ipv6full();
			//printf("   %d > idx [%d] = [%d][%d]-[%d]\n", tmp, idx, ipv6addr.s6_addr[idx-2], ipv6addr.s6_addr[idx-1], ipv6addr.s6_addr[idx]);

			// adicionar mais 1
			if(ipv6addr.s6_addr[idx] >= 255){

				while(idx >= 0 && ipv6addr.s6_addr[idx] >= 255){
					// nao da pra aumentar, zerar e aumentar a proxima casa
					ipv6addr.s6_addr[idx] = 0;
					idx--;
					ipv6addr.s6_addr[idx]++;					
				}

			}else{
				ipv6addr.s6_addr[idx]++;
			}

			//printf(" B ");print_ipv6full();
			//printf("   %d > idx [%d] = [%d][%d]-[%d]\n", tmp, idx, ipv6addr.s6_addr[idx-2], ipv6addr.s6_addr[idx-1], ipv6addr.s6_addr[idx]);
			//printf("\n");

			tmp--;
		}

	}


   	//printf("POS OP\n");
    inet_ntop(AF_INET6, &(ipv6addr), ipv6str, INET6_ADDRSTRLEN);
    printf("%s\n", ipv6str);
	//print_ipv6full();	

	return 0;

}

















