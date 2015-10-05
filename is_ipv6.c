#include <stdio.h>
#include <string.h>
#include <stdlib.h>


// testar se string esta no formato ipv6
// se is_prefix for 1, sera verificado se a mascara foi informada em numero de bits
// se have_port for 1, sera verificado se a porta foi informada, mas nao e' possivel ter o prefixo e a
// porta na mesma declaracao
int is_ipv6(char *str, int is_prefix, int have_port){
	char atch;
	char lastch;
	char nextch;
	char n2xtch;

	char address[32];
	int addrlen = 0;
	char *rest=0;

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
	// ::
	// 1::
	// ::1
	// 2::2
	// 2000::1234
	// 1:2:3:4:5:6:7:8
	// 2001:2002:2003:2004:2005:2006:2007:2008
	// 2abc:2def:2abc:2ddd:2eee:2fff:2ded:dead
	//
	// ::/0
	// 1::/7
	// ::1/128
	// 2::2/64
	// 2000::1234/126
	// 1:2:3:4:5:6:7:8/120
	// 2001:2002:2003:2004:2005:2006:2007:2008/64
	// 2abc:2def:2abc:2ddd:2eee:2fff:2ded:dead/126

	// [::]:1
	// [::]:80
	// [1::]:22
	// [::1]:3209
	// [2::2]:5900
	// [2000::1234]:8080
	// [1:2:3:4:5:6:7:8]:65535
	// [2001:2002:2003:2004:2005:2006:2007:2008]:22
	// [2abc:2def:2abc:2ddd:2eee:2fff:2ded:dead]:65535

	// []
	// 0.0.0.0
	// 0.0.0.0:65123
	// 255.255.255.255:65535
	// 0.0.0.0/0
	// 255.255.255.255/32

	// GERAL
	// menor formato: ::
	// maior formato: [2abc:2def:2abc:2ddd:2eee:2fff:2ded:dead]:65535
	if(len<2 || len > 47) return 3;

	// COM PREFIXO
	// menor formato: ::/0
	// maior formato: 2abc:2def:2abc:2ddd:2eee:2fff:2ded:dead/126
	if(is_prefix && ( len < 4 || len > 43 ) ) return 4;

	// COM PORTA
	// menor formato: [::]:1
	// maior formato: [2abc:2def:2abc:2ddd:2eee:2fff:2ded:dead]:65535
	if(have_port && ( len < 6 || len > 47 ) ) return 5;



	// obter parte do endereco para facilitar leitura de prefixo e porta
	//address = calloc(1, len+1);
	//if(!address) exit(1);

	// controles
	int jumpcount = 0;
	int comacount = 0;

	// COLETAR ENDERECO IPV6
	// loop
	// printf("ENTRADA: '%s' [%d]\n", str, len);
	for(i=0;i<len;i++){
		// ler byte atual, proximo e anterior
		atch = str[i];
		lastch = nextch = n2xtch = 0;
		if(i+1<len) nextch = str[i+1];
		if(i+2<len) n2xtch = str[i+2];

		// printf("> %c\n", atch);

		// em modo porta, mas primeiro byte nao abrir endereco ipv6 []
		if(have_port){
			// precido que o primeiro byte abra o endereco
			if(!i && atch != '[') return 6;
			// abertura
			if(!i && atch == '[') continue;
		}

		// se for byte valido, copiar
		if( isxdigit(atch) || atch==':' ){
			address[addrlen++] = atch;
			continue;
		}

		// detectar caracter proibido
		if(atch!=']' && atch != ':' && atch != '/') return 7;

		// esse byte nao faz mais parte do endereco
		// marcar fim da parte de enderecos
		if(atch==']' && nextch == ':'){
			i+=2;
		}else{
			if(atch == '/' || atch == ']') i++;
		}
		rest = (str+i);
		break;
	}

	// printf("Endereco: '%s' resto '%s'\n", address, rest);

	// Loop byte a byte do endereco e valida-lo
	for(i=0;i<addrlen;i++){
		// ler byte atual, proximo e anterior
		atch = address[i];
		lastch = nextch = n2xtch = 0;
		if(i+1<len) nextch = address[i+1];
		if(i+2<len) n2xtch = address[i+2];

		if(i) lastch = address[i-1];

		// temos um JUMP?
		if(atch==':' && nextch == ':'){
			// tentou dar jump 2 vezes
			if(jumpcount) return 8;
			jumpcount++;

			// especial:
			// se o byte apos o jump for outro coma, temos uma violacao de formato
			if(n2xtch ==':') return 9;

			// ignorar o proximo
			i++;
			continue;

		}

		// separador
		if(atch==':'){
			comacount++;
			// excedeu 8 grupos (7 separadores)
			if(comacount>7 || (comacount > 5 && jumpcount) ) return 10;
		}

		// ler numero, so precisamos que ele seja coerente
		if( isxdigit(atch) ){
			int k = 0;
			for(j=i; j<len; j++, k++){
				char ch = address[j];
				if(!isxdigit(ch)) break;
				// posicionar em ultimo hexadecimal lido
				i = j;
			}
			// tem mais de 4 digitos nesse numero
			if(k>4) return 11;
			continue;
		}

	} // for i

	// grupos de menos
	if(comacount != 7 && !jumpcount) return 12;


	// ler numero apos endereco
	if( (is_prefix || have_port)){
		int num, rlen;
		num = 0;

		//printf("REST: %s\n", rest);

		// sem bytes para processar
		if(!rest || !rest[0]) return 13;
		rlen = strlen(rest);

		// transformar em numero
		for(i=0;i<rlen;i++){
			char ch;
			ch = rest[i];

			// nao e numero
			if(!isdigit(ch)) return 14;
			//printf(" RB> %c / %d\n", ch, num);

			// numero
			num *= 10;
			num += (ch - 48);
		}
		port = num;
		prefix = num;
		//printf("Rest num: %d\n", num);
	}

	//printf("POST=%d PREFIX=%d\n", port, prefix);


	// pediu prefix mas nao informou o prefixo
	if(is_prefix && (prefix < 0 || prefix > 128)) return 15;

	// pediu prefix mas nao informou o prefixo
	if(have_port && (!port || port > 65535)) return 16;

	return 0;
}



void help_std(){
	printf("\n");
	printf("is_ipv6\n");
	printf("Verifica se os parametros estao no formato de um endereco ipv6, retorno stdno=0 em caso de correto ou >0 se estiver incorreto\n");
	printf("\n");
	printf("Autor: Patrick Brandao <patrickbrandao@gmail.com>, todos os direitos reservados\n");
	printf("\n");
	printf("Use: is_ipv6 (ipv6) [ipv6] [ipv6] [...]\n");
	printf("\n");
	exit(1);
}

int main(int argc, char **argv){
	int i = 0;
    if(argc<2) help_std();
	for(i=1;i<argc;i++){
		int r = is_ipv6(argv[i], 0, 0);
		if(r>0) return r;
	}
	return 0;
}



















