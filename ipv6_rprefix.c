#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <time.h>
#include <unistd.h>

void help_std(){
    printf("\n");
    printf("ipv6_rprefix\n");
    printf("Gera um prefixo IPv6 aleatorio.\n");
    printf("\n");
    printf("Autor: Patrick Brandao <patrickbrandao@gmail.com>, todos os direitos reservados\n");
    printf("\n");
    printf("Use: ipv6_rprefix [prefix-length]\n");
    printf("\n");
    exit(1);
}

// randomizar byte
unsigned char byterand(int min, int max){
    int dif = max - min;
    unsigned char r;
    int n;

    // semear
    srand((unsigned int) time(0) + getpid());
    n= (int)(rand());
    r = (n % dif) + min;

    // security!
    if(r>max)r=max;

    return r;
}

void ipv6_mask(struct in6_addr *v6, int prefix_len){
    int j = 0;
    int bits = prefix_len;
    if(!bits) return;
    // Aplicar mascara do prefixo no ipv6
    while(bits > 0 || j < 8){
	if(bits >= 8){
	    // byte dentro do prefixo
	    bits -= 8;
	}else if(bits > 0){
	    // temos que fazer uma operacao
	    // 'and' na casa atual com o numero
	    // de bits restantes no prefixo
	    int oldbyte = v6->s6_addr[j];
	    int newbyte = oldbyte;
	    // calcular numero decimal que representa os bits
	    // que faltam para gerar um AND com o byte do IPv6
	    int calc = 0;
	    int rbits = (8-bits); // numero de bits em zeros
	    while(rbits-->0) calc = (calc<<1) + 1; // calcular binario em 1 nos espacos em zero
	    calc = 255 - calc; // inverter
	    newbyte &= calc;
	    v6->s6_addr[j] = newbyte;
	    // acabou
	    bits = 0;
	}else{
	    // casa nao coberta pelo prefixo, zerar
	    v6->s6_addr[j] = 0;
	}
	j++;
    }
}

int main(int argc, char **argv){
    struct in6_addr ipv6;
    char str[INET6_ADDRSTRLEN];
    int plen = 0;
    int k, bits;

    // zerar memoria de trabalho
    memset(&ipv6, 0, sizeof(struct in6_addr));
    memset(&str, 0, INET6_ADDRSTRLEN);

    // tamanho do prefixo
    if(argc > 1) plen = atoi(argv[1]);
    if(plen < 12 || plen > 48) plen = byterand(32,48);

    // Gerar os bytes de prefixo aleatorios
    bits = plen;
    for(k=1;k<8;k++) ipv6.s6_addr[k] = byterand(0,255);
    
    // Definir como prefixo global
    ipv6.s6_addr[0] = byterand(32,63);

    // zerar bits fora do prefixo
    ipv6_mask(&ipv6, plen);

    // converter struct de 128 bits em notacao string
    inet_ntop(AF_INET6, &(ipv6), str, INET6_ADDRSTRLEN);

    printf("%s/%d\n", str, plen);
    return 0;

}

