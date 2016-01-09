#include <stdio.h>
#include <string.h>
#include <stdlib.h>


// testar se string esta no formato ipv4
// porta na mesma declaracao
int is_ipv4(char *str, int is_prefix, int have_port){
	char atch;
	char lastch;
	char nextch;
	register int i, j;

	int prefix = 0;
	int port = 0;


	int len;

	// string vazia
	if(!str || !str[0]) return 1;

	// pedido impossivel
	if(is_prefix && have_port) return 2;

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

	// COM PREFIXO
	// menor formato: 0.0.0.0/0
	// maior formato: 255.255.255.255/32
	if(is_prefix && ( len < 9 || len > 18 ) ) return 4;

	// COM PORTA
	// menor formato: 0.0.0.0:1
	// maior formato: 255.255.255.255:65123
	if(have_port && ( len < 9 || len > 21 ) ) return 5;


	// Loop byte a byte

	// controles
	int dotcount = 0;
	int barcount = 0;
	int comacount = 0;
	int bytes[5];
	bytes[0]=bytes[1]=bytes[2]=bytes[3]=bytes[4]=-1;
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
		if( ! is_prefix && atch=='/' ) return 7;
		if( ! have_port && atch==':' ) return 8;

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
		if(atch == '.'){
			dotcount++;
			if(dotcount > 3) return 12;
			continue;
		}

		// barra
		if(atch == '/'){
			barcount++;
			if(barcount > 1 ) return 21;
			if(!is_prefix) return 31;
		}

		// barra
		if(atch == ':'){
			comacount++;
			if(comacount > 1 ) return 22;
			if(!have_port) return 32;
		}

		// se eu ler um coma ou barra sem ler os bytes IP, temos um problema
		if( byteidx < 4 && (comacount || barcount)) return 23;

		// ler numero
		if( isdigit(atch) ){
			int k = 0;
			num = 0;

			for(j=i; j<len && k < 5; j++, k++){
				char ch = str[j];
				if(!isdigit(ch)) break;
				// novo numero, o anterior vale 10
				num *= 10;
				// adicionar numero
				num += (ch - 48);
				i = j;
			}

			// acabou a string
			if(!str[i]) break;

			// encontramos um divisor
			if(have_port && str[i]==':'){
				i--;
				continue;
			}
			if(is_prefix && str[i]=='/'){
				i--;
				continue;
			}

			// muitos numeros, indice 5 em diante nao existe
			if(byteidx > 4) return 40;

			// numero de porta ou prefixo
			if(byteidx == 4){
				if(is_prefix){
					// esquisito, o prefixo ja foi lido
					if(prefix) return 13;
					// coletar prefixo
					prefix = num;
				}else{
					// esquisito, a porta ja foi lida
					if(port) return 14;
					// coletar porta
					port = num;
				}
			}

			// coletar byte
			bytes[byteidx]=num;	
			byteidx++;
		}

	} // for i

	//printf("BYTES: %d.%d.%d.%d | %d, byteidx: %d\n", bytes[0], bytes[1], bytes[2], bytes[3], bytes[4], byteidx);

	// nao informou 4 bytes
	// printf("final byteidx: %d\n", byteidx);
	if(byteidx < 4) return 15;

	// pediu prefix mas nao informou o prefixo
	if(is_prefix && (prefix < 0 || prefix > 32)) return 16;

	// pediu prefixo mas nao informou /
	if(is_prefix && !barcount) return 21;

	// pediu prefix mas nao informou o prefixo
	if(have_port && (!port || port > 65535)) return 17;

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
		int r = is_ipv4(argv[i], 0, 0);
		if(r>0) return r;
	}
	return 0;
}



















