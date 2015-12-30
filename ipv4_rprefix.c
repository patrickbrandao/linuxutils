#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <time.h>

void help_std(){
    printf("\n");
    printf("ipv4_rprefix\n");
    printf("Gera um prefixo IPv4 aleatorio.\n");
    printf("\n");
    printf("Autor: Patrick Brandao <patrickbrandao@gmail.com>, todos os direitos reservados\n");
    printf("\n");
    printf("Use: ipv4_rprefix [prefix-length]\n");
    printf("\n");
    exit(1);
}

// randomizar byte
unsigned char byterand(int min, int max){
    int dif = max - min;
    unsigned char r;
    r = (rand() % dif) + min;
    return r;
}

int main(int argc, char **argv){
    struct in_addr ipv4;
    char str[INET_ADDRSTRLEN];
    u_int32_t val = 0;
    u_int32_t mask = 0;
    unsigned char a=0,b=0,c=0,d=0;

    // semear
    srand(time(NULL));

    // tamanho do prefixo
    int plen = 0;
    if(argc > 1) plen = atoi(argv[1]);
    if(plen < 1 || plen > 32) plen = byterand(8,24);

    // gerar numero
    a=byterand(0, 223);
    b=byterand(0, 255);
    c=byterand(0, 255);
    d=byterand(0, 255);

    // converter para binario
    val |= (a << 24) | (b << 16) | (c << 8) | d;

    // aplicar prefixo
    mask = (255 << 24) | (255 << 16) | (255 << 8) | 255;
    //mask = mask << (32-plen);

    val &= mask << (32-plen);
    //val & (0 << plen-1);
    

    ipv4.s_addr = htonl(val);

    inet_ntop(AF_INET, &(ipv4), str, INET_ADDRSTRLEN);
    printf("%s/%d\n", str, plen);



    return 0;
}
