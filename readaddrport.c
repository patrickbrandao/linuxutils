
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <strings.h>

void help_std(){
	printf("\n");
	printf("readadrport\n");
	printf("Le um endereco (dns, ipv4 ou ipv6) e porta\n");
	printf("\n");
	printf("Autor: Patrick Brandao <patrickbrandao@gmail.com>, todos os direitos reservados\n");
	printf("\n");
	printf("Use: readadrport [fqdn|ipv4|[ipv6addr]][:port]\n");
	printf("\n");
	exit(1);	
}

struct struct_addrport {
	int ai_family;
	struct in_addr ipv4;
	struct in6_addr ipv6;
	int port_number;
};
#define SAPP struct struct_addrport*
#define SAP struct struct_addrport


void addrport_read(SAPP result, char *input){
	int len = 0;
	int idx = 0;
	int i;
	int cc = 0;
	int cd = 0;
	char addr[200];
	char port[6];

	bzero(result, sizeof(result));
	bzero(addr, 200);

	len = strlen(input);
	if(!len || len > 190) return;
	
	printf("INPUT: %s\n", input);

	if(input[idx]=='['){

		// ipv6 com notacao
		result->ai_family = AF_INET6;

		// notacao ipv6
		idx++;
		for(i=idx;i<len;i++){
			char m;
			m=input[i];
			idx=i;
			if(!m) break;
			if(m==']'){
				// avancar para depois da notacao e parar
				idx++;
				break;
			}
			addr[i-1] = m;
		}
	
	}else{
		// ipv6 ou ipv4
		for(i=0;i<len;i++){
			char m;
			m=input[i];
			idx=i;
			if(!m) break;

			// contagem de pontos indica ipv4
			if(m=='.') cd++;

			// contagem de : indica ipv6 ou separacao ipv4:porta
			if(m==':'){
				cc++;
				if(!cd){
					// : sem pontos anteriores, indica familia ipv6
					result->ai_family = AF_INET6;
				}else{
					// pontos encontrados, seguidos de :, familia ipv4
					result->ai_family = AF_INET;
					break;
				}
			}
			addr[i] = m;
		}
	}
	
	// familia padrao: ipv4
	if(!result->ai_family) result->ai_family = AF_INET;

	// ler porta
	if(idx<len){
		int j = 0;
		bzero(port, 6);
		for(i=idx; i<len && j < 5;i++){
			char m;
			m=input[i];
			if(m==':'){
				if(cc) break;
				cc++;
				continue;
			}
			if(m >= 48 && m <= 57) port[j++] = m;
		}	
	}
	
	// conversao string para binario
	result->port_number = atoi(port);
	switch(result->ai_family){
		case AF_INET:
			// converter string addr para ipv4 - 32 bits
			inet_pton(AF_INET, (const char *)addr, &(result->ipv4.s_addr) );
			break;

		case AF_INET6:
			// converter string addr para ipv6 - 128 bits
		    inet_pton(AF_INET6, addr, &(result->ipv6));
			break;
	}

}

int main(int argc, char **argv){
	int i = 0;
    if(argc<2) help_std();
	for(i=1;i<argc;i++){
		SAPP res;
		res = (SAPP)malloc(sizeof(SAPP));
		addrport_read(res, argv[i]);
	}
}


