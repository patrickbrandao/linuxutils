
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


// comparar semelhanca de bytes entre primeira string
// e segunda string
// a primeira string nao pode ser contrariada dentro da segunda string
// precisa estar contida no inicio da primeira para ter retorno >=
// retorna o numero de bytes iguais nas duas strings
// csense testa com (1) ou sem(0) suporte case-sensitive
// A primeira string e' uma ocorrencia real, enquando que a segunda
// string e' uma palavra apelido
// parametro igno
/*
----------------------------------------------- normal
p real:		addr
apelido:	a
result:		1

p real:		ip
apelido:	ipv
result:		0

p real:		ipv
apelido:	ip
result:		2

p real:		ping
apelido:	pool
result:		1

p real:		p
apelido:	pool
result:		0

p real:		pin
apelido:	pinoia
result:		3
----------------------------------------------- anormal
p real:		addr
apelido:    adr
result:     2


*/
int strmathcount(char *name, char *nick, int csense){
	register int k;
	register int nicklen, namelen;
	int ret = 0;
	// obter tamanhos
	namelen = strlen(name);
	nicklen = strlen(nick);

//	printf("Entrada:\n\tstring1: %s\n\tstring2: %s\n", name, nick);

	// nao e' apelido, tem letras a mais
	if(nicklen>namelen){
		ret = 0;
	}else{
		// comparar bytes
		for(k=0; k < nicklen; k++){
			// comparacao simples case-sensitive
			if(csense && name[k]==nick[k]){ ret++; continue; }
			// comparacao sem case senstivie
			if(tolower(name[k]) == tolower(nick[k])){ ret++; continue; }

			// nao pegou, incompativeis
			// palavras tomaram rumo diferentes, desconsiderar
			ret = 0;
			break;
		}
	}
	//printf(" > Retorno: %d\n", ret);
	return ret;
}


void help_std(){
	printf("\n");
	printf("strlike\n");
	printf("Testa a semelhanca entre duas strings (parametro 1 e parametro 2)\n");
	printf("Primeiro parametro deve ser a palavra completa, segunda parametro o versao reduzida\n");
	printf("Retorna o numero de bytes semelhantes\n");
	printf("\n");
	printf("Autor: Patrick Brandao <patrickbrandao@gmail.com>, todos os direitos reservados\n");
	printf("\n");
	printf("Use: strlike (string1) (string2)\n");
	printf("\n");
	exit(1);	
}


int main(int argc, char **argv){
	int i = 0;
    if(argc!=3) help_std();



    i = strmathcount(argv[1], argv[2], 0);
    printf("%d\n", i);

    return 0;
}


