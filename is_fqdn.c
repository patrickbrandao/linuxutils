#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// testar se string esta no formato ipv4
// porta na mesma declaracao
int is_fqdn(char *str){
	register int i;
	int test = 0;
	int len = strlen(str);
	int bcount = 0, dcount = 0;
	int have_dot = 0;

	// impossivel ter um nome menor que 3 bytes: x.y
	if(len < 3) return 8;
	// impossivel ter um mac maior que 256 bytes
	if(len > 254) return 9;

// analise de caracteres
	for(i=0;i<len;i++){
		char ch = str[i];
		char bf = 0;

		// caracter anterior
		if(i) bf = str[i-1];

		// converter para minusculo
		ch = tolower(ch);

		// tem ponto?
		if(ch=='.') have_dot++;

		// caracter proibido
		if(ch != '.' && ch != '-' && !isalnum(ch)) return 7;

		// nao pode comecar com ponto ou traco
		if(!i && (ch=='.'||ch=='-')) return 6;

		// nao pode terminar com traco
		// (pode terminar com ponto, FQDN de dns termina com ponto)
		if(i+1==len && ch=='-') return 5;

		// nao pode ter ponto ou traco duplo
		if( (ch=='.' || ch=='-') && bf == ch ) return 4;

	}

	// sem ponto, nome simples nao pode ser FQDN
	if(!have_dot) return 3;

	// passou por todas as verificacoes de erros
	return 0;

}


void help_std(){
	printf("\n");
	printf("is_fqdn\n");
	printf("Verifica se os parametros estao no formato FQDN (nome de host valido no DNS)\n");
	printf("Retorno stdno=0 em caso de correto ou >0 se estiver incorreto\n");
	printf("\n");
	printf("Exemplos reais esquisitos:\n");
	printf("\n");
	printf("\t3.141592653589793238462643383279502884197169399375105820974944592.com\n");
	printf("\twww.thelongestlistofthelongeststuffatthelongestdomainnameatlonglast.com\n");
	printf("\twww.openoffshorebankaccountsfornonresidents.com\n");
	printf("\twww.llanfairpwllgwyngyllgogerychwyrndrobwllllantysiliogogogoch.co.uk\n");
	printf("\twww.abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijk.com\n");
	printf("\n");
	printf("Exemplos comuns:\n");
	printf("\tintranet.br\n");
	printf("\twww.google.com.br\n");
	printf("\tintranet.acme.yyy\n");
	printf("\n");
	printf("Autor: Patrick Brandao <patrickbrandao@gmail.com>, todos os direitos reservados\n");
	printf("\n");
	printf("Use: is_fqdn (fqdn) [fqdn] [fqdn] [...]\n");
	printf("\n");
	exit(1);
}

int main(int argc, char **argv){
	int i = 0;
    if(argc<2) help_std();
	for(i=1;i<argc;i++){
		int r = is_fqdn(argv[i]);
		if(r>0) return r;
	}
	return 0;
}



















