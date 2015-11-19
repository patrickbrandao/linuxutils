#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>

// testar combinacao ruim
#define REGTEST_QUIT_OK(A, B)	test = test_regex(A, B); if(!test) return 0
#define REGTEST_QUIT_ER(A, B)	test = test_regex(A, B); if(test) return test

// retornar:
// 0 - OK
// 1 - nao pegou
// 2 - falha de execucao
int test_regex(const char *exp, const char *str){
	int ret = 0;

	/* aloca espaço para a estrutura do tipo regex_t */
	regex_t reg;

	// Criar objeto para teste
	if (regcomp(&reg , exp, REG_EXTENDED|REG_NOSUB) != 0) {

		// falha de execucao
		ret = 2;

	}else{
		/* tenta casar a ER compilada (&reg) com a entrada (argv[2]) 
		 * se a função regexec retornar 0 casou, caso contrário não */
		if ( (regexec(&reg, str, 0, (regmatch_t *)NULL, 0)) == 0){
			// pegou
			ret = 0;
		}else{
			// nao pegou
			ret = 1;
		}
	}
	return ret;

}

// testar se string esta no formato ipv4
// porta na mesma declaracao
int is_mac(char *str){
	register int i;
	int test = 0;
	int len = strlen(str);
	int bcount = 0, dcount = 0;

	// impossivel ter um mac menor que 12 bytes: 0:1:2:3:4:5
	if(len < 6) return 8;
	// impossivel ter um mac maior que 17 bytes: 00_11_22_33_44_55
	if(len > 17) return 9;

// analise de caracteres
	for(i=0;i<len;i++){
		char ch = str[i];
		// e' digito?
		int isd = isxdigit(ch);
		// e' divisor?
		int iss = ch=='.' || ch == '-' || ch == ':';

		if(isd) bcount++;
		if(iss) dcount++;

		if(isd || iss) continue;

		// caracter do mal, vaza
		return 6;
	}
	// numero de bytes lidos nao pode ser menor que 6, exemplo: 0:1:2:a:b:c
	// numero de bytes lidos nao pode ser menor que 12
	if(bcount < 6) return 5;
	if(bcount > 12) return 4;

	// nao podem haver mais que 5 divisores
	if(dcount > 5) return 11;


// combinacoes aceitas como 100% corretas

	// HEX puro
	if(bcount==12 && dcount == 0) return 0;

	// LINUX: 00:11:22:aa:bb:cc
	REGTEST_QUIT_OK("^([0-9A-Fa-f]{2}:){5}([0-9A-Fa-f]{2})$", str);

	// WIN: 00-11-22-aa-bb-cc
	REGTEST_QUIT_OK("^([0-9A-Fa-f]{2}-){5}([0-9A-Fa-f]{2})$", str);

	// CISCO: 001122-aabbcc
	REGTEST_QUIT_OK("^([0-9A-Fa-f]{2}){3}-([0-9A-Fa-f]{2}){3}$", str);


	// Mikrotik: 0:0:a:c:ff:af
	REGTEST_QUIT_OK("^([0-9A-Fa-f]{1,2}:){5}([0-9A-Fa-f]{1,2})$", str);

	return 7;

}


void help_std(){
	printf("\n");
	printf("is_mac\n");
	printf("Verifica se os parametros estao no formato de um endereco MAC, retorno stdno=0 em caso de correto ou >0 se estiver incorreto\n");
	printf("Formatos aceitos: 00:11:22:33:44:55, 00-11-22-33-44-55, 001122334455, 001122-334455\n");
	printf("\n");
	printf("Autor: Patrick Brandao <patrickbrandao@gmail.com>, todos os direitos reservados\n");
	printf("\n");
	printf("Use: is_mac (mac-address) [mac-address] [mac-address] [...]\n");
	printf("\n");
	exit(1);
}

int main(int argc, char **argv){
	int i = 0;
    if(argc<2) help_std();
	for(i=1;i<argc;i++){
		int r = is_mac(argv[i]);
		if(r>0) return r;
	}
	return 0;
}



