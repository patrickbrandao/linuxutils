
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void help_std(){
	printf("\n");
	printf("in_str\n");
	printf("Verifica se o primeiro parametro se encontra dentro do segundo parametro, no-case-sensitive\n");
	printf("\n");
	printf("Autor: Patrick Brandao <patrickbrandao@gmail.com>, todos os direitos reservados\n");
	printf("\n");
	printf("Use: in_str (word) (string)\n");
	printf("\n");
	exit(1);
}

void str_to_lower(char *str){
	int len = strlen(str);
	register int i;
	for(i=0;i<len;i++) str[i] = tolower(str[i]);
}

int main(int argc, char **argv){
	register int i = 0;
	char *word = NULL;
	char *string = NULL;
	char *res;
    if(argc!=3) help_std();

    // palavra
    word = argv[1];

    // frase
    string = argv[2];

    // verificar se esta contido
    str_to_lower(word);
    str_to_lower(string);

    // procurar
    res = strstr(string, word);

    if(res==NULL) return 1;

	// nenhum item encontrado
	return 0;
}



















