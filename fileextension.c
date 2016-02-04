/*

	obter extencao do arquivo

*/


#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){

	// posicao do ultimo ponto
	char *lastdot = NULL;
	char *str = NULL;

	// tamanho da string de entrada
	int slen = 0;
	register int i = 0, m = 0;

	// nao informou o nome do arquivo
	if(argc != 2) return 1;

	// obter posicao do ultimo ponto
	str = argv[1];
	slen = strlen(str);
	if(slen < 3) return 0;

	for(i=0; i < slen; i++)
		if(str[i]=='.'){
			lastdot = &str[i];
			m = i;
		}
	//-
	
	// uma extencao foi encontrada apos ultimo ponto
	if(m < slen -1) printf("%s\n", ++lastdot);
	return 0;
}
