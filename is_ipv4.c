#include <stdio.h>
#include <string.h>
#include <stdlib.h>


// testar se string esta no formato ipv4
// porta na mesma declaracao
int is_ipv4(char *str){
	char atch;
	char lastch;
	char nextch;
	register int i, j;

	int len;

	// string vazia
	if(!str || !str[0]) return 1;

	// tamanho da string
	len=strlen(str);

	// Formatos validos esperados:
	// 255.255.255.255
	// 0.0.0.0
	// 0.0.0.0:65123
	// 255.255.255.255:65535
	// 0.0.0.0/0
	// 255.255.255.255/32

	// GERAL
	// menor formato: 0.0.0.0
	// maior formato: 255.255.255.255:65123
	if(len<7 || len > 21) return 3;

	// Loop byte a byte

	// controles
	int dotcount = 0;
	int barcount = 0;
	int comacount = 0;
	int bytes[4];
	bytes[0]=bytes[1]=bytes[2]=bytes[3]=-1;
	int byteidx = 0;

	// loop
	for(i=0;i<len;i++){
		int num;
		num = 0;

		// ler byte atual, proximo e anterior
		atch = str[i];
		lastch = nextch = 0;
		if(i+1<len) nextch = str[i+1];
		if(i) lastch = str[i-1];

		// se for ponto, ler novo numero
		// printf("CHAR: %c\n", atch);


		// nao pode comecar caracter diferente de numero
		if(!i && !isdigit(atch)) return 6;

		// caracter proibidos de acordo com o tipo de requisicao
		if( atch=='/' ) return 7;
		if( atch==':' ) return 8;

		// sequencias proibidas
		if(lastch){
			// duplo ponto
			if(lastch=='.' && atch=='.') return 9;
			// dupla barra
			if(lastch=='/' && atch=='/') return 10;
			// duplo coma
			if(lastch==':' && atch==':') return 11;
		}

		// ponto
		if(atch == '.') dotcount++;
		if(dotcount > 4) return 12;

		// barra
		if(atch == '/') barcount++;
		if(barcount > 1 ) return 21;

		// barra
		if(atch == ':') comacount++;
		if(comacount > 1 ) return 22;

		// se eu ler um coma ou barra sem ler os bytes IP, temos um problema
		if( byteidx != 4 && (comacount || barcount)) return 23;

		// ler numero
		if( isdigit(atch) ){
			int k = 0;
			num = 0;
			for(j=i; j<len && k < 3; j++, k++){
				char ch = str[j];
				if(!isdigit(ch)) break;
				// novo numero, o anterior vale 10
				num *= 10;
				// adicionar numero
				num += (ch - 48);
				i = j;
			}
			// parte IP
			bytes[byteidx]=num;	
			byteidx++;
			continue;
		}
	} // for i


	printf("BYTES: %d.%d.%d.%d\n", bytes[0], bytes[1], bytes[2], bytes[3]);

	// nao informou 4 bytes
	// printf("final byteidx: %d\n", byteidx);
	if(byteidx != 4) return 15;

	// nenhum byte pode ser maior que 255
	for(i=0;i<4;i++) if( bytes[i] < 0 || bytes[i] > 255 )return 120+i;

	return 0;
}


void help_std(){
	printf("\n");
	printf("is_ipv4\n");
	printf("Verifica se os parametros estao no formato de um endereco ipv4, retorno stdno=0 em caso de correto ou >0 se estiver incorreto\n");
	printf("\n");
	printf("Autor: Patrick Brandao <patrickbrandao@gmail.com>, todos os direitos reservados\n");
	printf("\n");
	printf("Use: is_ipv4 (ipv4) [ipv4] [ipv4] [...]\n");
	printf("\n");
	exit(1);
}

int main(int argc, char **argv){
	int i = 0;
    if(argc<2) help_std();
	for(i=1;i<argc;i++){
		int r = is_ipv4(argv[i]);
		if(r>0) return r;
	}
	return 0;
}



















