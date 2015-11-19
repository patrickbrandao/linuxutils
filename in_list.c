
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void help_std(){
	printf("\n");
	printf("in_list\n");
	printf("Verifica se o primeiro parametro se encontra na lista (demais parametros), no-case-sensitive\n");
	printf("\n");
	printf("Autor: Patrick Brandao <patrickbrandao@gmail.com>, todos os direitos reservados\n");
	printf("\n");
	printf("Use: in_list (word) [item1] [item2] [...]\n");
	printf("\n");
	exit(1);
}

void str_to_lower(char *str){
	int len = strlen(str);
	register int i;
	for(i=0;i<len;i++) str[i] = tolower(str[i]);
}

// comprar strings sem case-sensitive
int string_comp_nocase(char *word, char *strtotest){
	int ret = 0;

	char *_word = NULL;
	char *_strtotest = NULL;

	// duplicar
	_word = strdup(word);
	if(!_word) return 0;

	_strtotest = strdup(strtotest);
	if(!_strtotest) return 0;

	// converter para minusculas
	str_to_lower(_word);
	str_to_lower(_strtotest);

	// printf("  Compare '%s' com '%s'\n", _word, _strtotest);

	// transformar as duas em minusculas
	if(strcmp(_word, _strtotest)==0) ret = 1;

	// cleanup
	free(_word);
	free(_strtotest);

	return ret;
}

int main(int argc, char **argv){
	register int i = 0;
	char *word = NULL;
    if(argc<2) help_std();

    // passou a palavra sem argumentos, sem lista
    if(argc==2) return 1;

    // 3 ou mais, primeiro apos o comando elemento e' a palavra a ser procurada
    word = argv[1];

    //printf("INPUT: '%s', argc=%d\n", word, argc);
	for(i=2; i<argc; i++){
		int r = string_comp_nocase(word, argv[i]);
		// printf("Test '%s' com '%s' = %d\n", word, argv[i], r);
		if(r > 0) return 0;
	}

	// nenhum item encontrado
	return 2;
}



