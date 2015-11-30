#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#include <sys/types.h>
#include <sys/param.h>
#include <arpa/inet.h>
#include <ctype.h>

/*
	
	Receber ip no primeiro parametro e realiza operacao de adicionar

	Exemplo......: ipv4_add 192.168.50.20 80
		Retorno..: 192.168.50.100

	Exemplo......: ipv4_add 192.168.50.20 -7
		Retorno..: 192.168.50.13

	Exemplo......: ipv4_add 192.168.50.20 -21
		Retorno..: 192.168.49.255

*/

void help_std(){
	printf("\n");
	printf("ipv4_op\n");
	printf("Recebe um endereco IPv4 e executa uma operacao de adicionar ou subtrair,\n");
	printf("\tobtendo assim o ip resultante.\n");
	printf("\n");
	printf("Autor: Patrick Brandao <patrickbrandao@gmail.com>, todos os direitos reservados\n");
	printf("\n");
	printf("Use: ipv4_op (ipv4) (+-number) [+-number] [...]\n");
	printf("\n");
	printf("Exemplo: 'ipv4_op 100.64.0.0 10' retorna 100.64.0.10\n");
	printf("Exemplo: 'ipv4_op 100.64.0.80 -30 5' retorna 100.64.0.55\n");
	printf("Exemplo: 'ipv4_op 100.64.0.1 -2' retorna 100.63.255.255\n");
	printf("\n");
	exit(1);
}

void print_ip(int ip){
    unsigned char bytes[4];
    bytes[0] = ip & 0xFF;
    bytes[1] = (ip >> 8);
    bytes[2] = (ip >> 16);
    bytes[3] = (ip >> 24);	
    printf("PRINT_IP: %d.%d.%d.%d\n", bytes[3], bytes[2], bytes[1], bytes[0]);        
}

#define STR_IP		argv[1]
#define STR_NUM		argv[2]
int main(int argc, char **argv){
	long int i;

	// leigura do ip
    int a=0, b=0, c=0, d=0, ma=0, mb=0, mc=0, md=0;
	int num = 0;

	// argumentos insuficientes
	if(argc<3) help_std();

	// obter numero, permitir multiplos numeros como parametros
	for(i=2; i < argc; i++){
		long int _num = atoi(argv[i]);
		num += _num;

	}
	// nada a fazer
	// imprimir ip de entrada
	if(num==0){
		printf("%s\n", STR_IP);
		return 0;
	}

	// Ler IP x.x.x.x ou x.x.x.x/m ou x.x.x.x/m.m.m.m
	if (
			(sscanf(STR_IP, "%u.%u.%u.%u", &a, &b, &c, &d) == 4) ||
			(sscanf(STR_IP, "%u.%u.%u.%u", &a, &b, &c, &d) == 4) ||
			(sscanf(STR_IP, "%u.%u.%u.%u/%u", &a, &b, &c, &d, &ma) == 5) ||
			(sscanf(STR_IP, "%u.%u.%u.%u/%u.%u.%u.%u", &a, &b, &c, &d, &ma, &mb, &mc, &md) == 8)
	){

		// formato correto (apenas numeros de 0-255)
		if(a >= 0 && a < 256 && b >= 0 && b < 256 && c >= 0 && c < 256 && d >=0 && d < 256){

			// subtrair
			if(num < 0){
				while(num++ < 0){
					d--;
					if(d < 0){ c--; d = 255; }
					if(c < 0){ b--; c = 255; }
					if(b < 0){ a--; b = 255; }
					if(a < 0){ a=255; b=255; c=255; d=255; }
				}
			}

			// somar
			if(num > 0){
				while(num-- > 0){
					d++;
					if(d > 255){ c++; d = 0; }
					if(c > 255){ b++; c = 0; }
					if(b > 255){ a++; b = 0; }
					if(a > 255){ a=0; b=0; c=0; d=0; }
				}
			}
			

		}else{
			// erro na leitura, entrada ruim
			return 1;
		}
	}else{
		// erro de leitura, nenhum formato valido informado
		return 2;
	}

	// imprimir resultado
	printf("%u.%u.%u.%u\n", a, b, c, d);


}








