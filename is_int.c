#include <stdio.h>
#include <string.h>
#include <stdlib.h>


// testar se string esta no formato numerico inteiro
int is_integer(char *str){
	char atch;
	char lastch;
	char nextch;
	register int i, j;

	int len;

	// string vazia
	if(!str || !str[0]) return 1;

	// tamanho da string
	len=strlen(str);
	if(!len) return 2;


	// Formatos validos esperados:
	// 1
	// 123456
	// 193839
	// 00001222
	// 12.3
	// 1239.04

	// nao valido:
	// 123.
	// .192

	// Loop byte a byte

	// loop
	for(i=0;i<len;i++){
		atch = str[i];
		lastch = i ? str[i-1] : 0;

		if(isdigit(atch)) continue;

		// caracter invalido
		return 4;

	} // for i

	// tudo correto
	return 0;
}


void help_std(){
	printf("\n");
	printf("is_integer\n");
	printf("Verifica se os parametros estao no formato numerico inteiro,\n\tretorno stdno=0 em caso de correto ou >0 se estiver incorreto\n");
	printf("\n");
	printf("Autor: Patrick Brandao <patrickbrandao@gmail.com>, todos os direitos reservados\n");
	printf("\n");
	printf("Use: is_integer (data) [data] [data] [...]\n");
	printf("\n");
	exit(1);
}

int main(int argc, char **argv){
	int i = 0;
    if(argc<2) help_std();
	for(i=1;i<argc;i++){
		int r = is_integer(argv[i]);
		if(r>0) return r;
	}
	return 0;
}



















