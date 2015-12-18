
// IPv6
// Gestao de Pools e prefixos IPv6 para ND e DHCP-PD
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdint.h>


// struct para armazenar prefixo
typedef struct ipv6prefix_s {
	struct in6_addr network;	// endereco da rede
	unsigned int plen;			// tamanho do prefixo
	unsigned int slen;			// tamanho das subnets
	int32_t scount;				// numero de subnets
	int32_t sbits;				// numero de bits a variar
} ipv6prefix;

int ipv6p_prefix_read(const char *prefix, ipv6prefix *px);
int ipv6p_read(const char *prefix, const int slen, ipv6prefix *px);
void print_in6_addr(struct in6_addr ipv6ia);
void print_in6_addr_prefix(struct in6_addr ipv6ia);
void print_ipv6_prefix(ipv6prefix *px);


// Ler prefixo: endereco ipv6 e numero de bits
// retorna:
// 0 = ok
// N = onde N e' o numero do erro
//	1 = nao informou o prefixo
//	2 = mascara (tamanho do prefixo) invalida sintaticamente
//	3 = caracter invalido no prefixo
//	4 = nao e' prefixo, muito pequeno
//	5 = nao e' prefixo, sem mascara, precisa ser de /10 a /63
//	6 = mascara violou padrao
//	7 = erro no parse do endereco ipv6
int ipv6p_prefix_read(const char *prefix, ipv6prefix *px){
	int len;			// tamanho da string entrada
	int plen=0;			// tamanho do endereco do prefixo (parte ipv6)
	int psize = 0;		// numero de bits do prefixo
	char ipv6str[INET6_ADDRSTRLEN];
	int i;

	// zerar
	memset(px, 0, sizeof(ipv6prefix));
	memset(&ipv6str, 0, INET6_ADDRSTRLEN);

	if(!prefix || !prefix[0]) return 1;
	len = strlen(prefix);
	if(len < 4) return 4;

	// copiar ipv6 ate a mascara
	for(i=0; i<len; i++){
		if(prefix[i]=='/') break;
		ipv6str[i] = prefix[i];
		plen++;
	}
	if(plen == len) return 5;

	psize = atoi( &(prefix[plen]) + 1);
	if(psize < 10 || psize > 63) return 5;
	printf("Ler [%s] IPv6=[%s] Psize=%d\n", prefix, ipv6str, psize);

	// fazer conversao para 128 bits
	if(!inet_pton(AF_INET6, ipv6str, &(px->network))) return 7;
	px->plen = psize;

	return 0;
}

// Caulo de potencia
// falta

// carregar struct com prefixo e tamanho do prefixo delegado
// retorna: 0=ok, 10=incoerencia no tamanho do sub-prefixo
int ipv6p_read(const char *prefix, const int slen, ipv6prefix *px){
	register int ret = 0, j, bits;
	// converter string para formato ipv6
	ret = ipv6p_prefix_read(prefix, px);
	if(ret) return ret;

	// zerar 64bits eui64
	//for(j=8;j<16;j++) px->network.s6_addr[j] = 0;

	// zerar bits que nao pertencam ao prefixo
	// percorrer a cada 8 bits e comecar
	// a zerar os bits onde atingirmos um restante menor igual a 8
	bits = px->plen;
	j = 0;
	while(bits > 0 || j < 16){
		if(bits >= 8){
			// ignorar esse byte, o prefixo
			// cobre ele
			bits-=8;

		}else if(bits > 0){
			// temos que fazer uma operacao
			// 'and' na casa atual com o numero
			// de bits restantes no prefixo
			int oldbyte = px->network.s6_addr[j];
			int newbyte = oldbyte;

			// calcular numero decimal que representa os bits
			// que faltam para gerar um AND com o byte do IPv6
			int calc = 0;
			int rbits = (8-bits); // numero de bits em zeros
			while(rbits-->0) calc = (calc<<1) + 1; // calcular binario em 1 nos espacos em zero
			calc = 255 - calc; // inverter

			newbyte &= calc;

			//printf("> Operando casa %d, byte=%d (%02x), bits-restantes=%d bits-dec=%d (%02x) resultado-and=%d (%02x)",
			//	j, oldbyte, oldbyte, bits, calc, calc, newbyte, newbyte
			//);

			px->network.s6_addr[j] = newbyte;

			// acabou
			bits = 0;
		}else{
			// casa nao coberta pelo prefixo, zerar
			//printf(" -> zerar casa %d\n", j);
			px->network.s6_addr[j] = 0;
		}
		j++;
	}

	// sub-prefixos
	px->slen = slen;
	if(px->plen > px->slen) return 10;
	px->sbits = px->slen - px->plen;

	// total:
	px->scount = -1;

	// calcular apenas numeros realmente utilizaveis!
	// 4 bilhoes bastam
	if(px->sbits <= 32 && px->sbits > 0){
		int32_t sbits = (px->sbits);
		int32_t total = 1;
		int32_t count;
		//printf("Calcular numero de subnets para %d -> %d\n", sbits, total);
		for(count=0; count < sbits; count++){
		//while(sbits >= 0){
			//printf(" %u -> %u\n", sbits, total);
			total = (total*2);
			//printf("    %u => %u\n", count, total);
		}
		px->scount = (total);
		//printf("Total cauculado: %u\n", px->scount);
	}
	return ret;
}

//--------------------------------------------- generico:

// imprimir struct de ipv6 completo
void print_in6_addr(struct in6_addr ipv6ia){
	printf("%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x\n",
		ipv6ia.s6_addr[0], ipv6ia.s6_addr[1], ipv6ia.s6_addr[2], ipv6ia.s6_addr[3],
		ipv6ia.s6_addr[4], ipv6ia.s6_addr[5], ipv6ia.s6_addr[6], ipv6ia.s6_addr[7],
		ipv6ia.s6_addr[8], ipv6ia.s6_addr[9], ipv6ia.s6_addr[10], ipv6ia.s6_addr[11],
		ipv6ia.s6_addr[12], ipv6ia.s6_addr[13], ipv6ia.s6_addr[14], ipv6ia.s6_addr[15]
	);
}
// imprimir struct de ipv6 apenas PREFIXO 64 bits
void print_in6_addr_prefix(struct in6_addr ipv6ia){
	printf("%02x%02x:%02x%02x:%02x%02x:%02x%02x\n",
		ipv6ia.s6_addr[0], ipv6ia.s6_addr[1], ipv6ia.s6_addr[2], ipv6ia.s6_addr[3],
		ipv6ia.s6_addr[4], ipv6ia.s6_addr[5], ipv6ia.s6_addr[6], ipv6ia.s6_addr[7]
	);
}

// imprimir struct ipv6prefix
void print_ipv6_prefix(ipv6prefix *px){
	printf("IPv6 Prefix\n");
	printf("\tCompleto.........: "); print_in6_addr(px->network);
	printf("\tPrefixo..........: ");

	printf("%02x%02x:%02x%02x:%02x%02x:%02x%02x::/%d\n",
		px->network.s6_addr[0], px->network.s6_addr[1], px->network.s6_addr[2], px->network.s6_addr[3],
		px->network.s6_addr[4], px->network.s6_addr[5], px->network.s6_addr[6], px->network.s6_addr[7],
		px->plen
	);

	printf("\tBits prefixo.....: %d\n", px->plen);
	printf("\tBits sub-prefix..: %d\n", px->slen);
	printf("\tBits sub-nets....: %d\n", px->sbits);
	printf("\tTotal sub-nets...: %u\n", px->scount);

}


void help_std(){
	printf("\n");
	printf("ipv6pinfo\n");
	printf("Recebe um prefixo IPv6 e a quantidade de sub-redes desejadas (prefixos menores)\n");
	printf("\te exibe informacoes do resultado.\n");
	printf("\n");
	printf("Autor: Patrick Brandao <patrickbrandao@gmail.com>, todos os direitos reservados\n");
	printf("\n");
	printf("Use: ipv6pinfo (ipv6-prefix) (subnets)\n");
	printf("\n");
	exit(1);
}

int main(int argc, char **argv){

	ipv6prefix six;
	int slen=0;
	int ret = 0;

	if(argc!=3) help_std();

	// tamanho do sub-prefixo
	slen = atoi(argv[2]);

	// ler prefixo
	ret = ipv6p_read(argv[1], slen, &six);
	if(ret) return ret;

    print_ipv6_prefix(&six);

	return ret;

}
