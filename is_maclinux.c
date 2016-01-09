#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// testar se string esta no formato ipv4
// porta na mesma declaracao
int is_maclinux(char *str){
	int i, len;
	const char *maskstr = "xx:xx:xx:xx:xx:xx";
	const int masklen = 17;

	// string vazia
	if(!str || !str[0]) return 1;

	// tamanho
	len = strlen(str);
	if(len != masklen) return 2;

	// analisar string na mascara
	for(i=0; i < masklen; i++){
		char atch = str[i];
		char mkch = maskstr[i];
		if(mkch==':' && atch==':') continue;
		if(mkch=='x' && isxdigit(atch)) continue;
		return 3;
	}

	// tudo certo!
	return 0;
}


void help_std(){
	printf("\n");
	printf("is_maclinux\n");
	printf("Verifica se os parametros estao no formato de um endereco MAC do linux\n");
	printf("Formato aceito: XX:XX:XX:XX:XX:XX\n");
	printf("\n");
	printf("Autor: Patrick Brandao <patrickbrandao@gmail.com>, todos os direitos reservados\n");
	printf("\n");
	printf("Use: is_maclinux (mac-address) [mac-address] [mac-address] [...]\n");
	printf("\n");
	exit(1);
}

int main(int argc, char **argv){
	int i = 0;
    if(argc<2) help_std();
	for(i=1;i<argc;i++){
		int r = is_maclinux(argv[i]);
		if(r>0) return r;
	}
	return 0;
}


