
#include <stdlib.h>
#include <stdio.h>

/*
	
	Receber 2 argumentos, o primeiro e' o caracter que deseja contar e o
	segundo argumento e posteriores
	e' o texto/frase

*/

#ifndef NULL
#define NULL 0;
#endif

void help_std(){
	printf("\n");
	printf("countchar\n");
	printf("Conta quantas vezes o byte (primeiro parametro) ocorre nos textos (parametros seguntes)\n");
	printf("\n");
	printf("Autor: Patrick Brandao <patrickbrandao@gmail.com>, todos os direitos reservados\n");
	printf("\n");
	printf("Use: countchar (char) (text) [text] [...]\n");
	printf("\n");
	exit(1);	
}

int main(int argc, char **argv){
	int i;
	int count = 0;
	char byte = 0;
    if(argc<3) help_std();

    // byte
    byte = argv[1][0];

    // analisar 
	for(i=2;i<argc;i++){
		register int j;
		char *arg = argv[i];
		//int len = strlen(arg);
		//if(!len) continue;

		while(*arg != NULL){
			if(*arg==byte) count++;
			arg++;
		}

	}

	printf("%d\n", count);

	if(!count) return 1;
	return 0;
}
