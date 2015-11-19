
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void help_std(){
	printf("\n");
	printf("in_list\n");
	printf("Verifica se o primeiro parametro se encontra na lista (demais parametros) por similaridade, no-case-sensitive\n");
	printf("\n");
	printf("Autor: Patrick Brandao <patrickbrandao@gmail.com>, todos os direitos reservados\n");
	printf("\n");
	printf("Use: in_list_like (word) [item1] [item2] [...]\n");
	printf("\n");
	exit(1);
}


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


void str_to_lower(char *str){
	int len = strlen(str);
	register int i;
	for(i=0;i<len;i++) str[i] = tolower(str[i]);
}

// comprar strings sem case-sensitive
int string_comp_nocase_like(char *word, char *strtotest){
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

	// comparar similaridade
    ret = strmathcount(_word, _strtotest, 0);

	// cleanup
	free(_word);
	free(_strtotest);

	return ret;
}

// 0 1 2
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
		int r = string_comp_nocase_like(word, argv[i]);
		// printf("Test '%s' com '%s' = %d\n", word, argv[i], r);
		if(r > 0) return 0;
	}

	// nenhum item encontrado
	return 2;
}



















