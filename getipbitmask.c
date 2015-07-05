/*
	Recebe a mascara ipv4 decimal e retorna o numero de bits
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static void parse_args( int argc, char** argv );

//varivel para cor da fonte
int bitmask = 24;

static char* argv0;

static void usage( void ) {
	fprintf( stderr, "Autor: Patrick Brandao <contato@tmsoft.com.br>\nUsage: %s (bits in the mask)\n", argv0 );
	exit( 1 );
}

static void parse_args( int argc, char** argv ){
	int argn = 0;
	int cl = 0;
	while( ++argn < argc){
		
		// help
		if(!strcmp(argv[argn],"-h") || !strcmp(argv[argn],"--help")) usage();
		
		// mascaras
		bitmask = 0;
		if(!strcmp(argv[argn],"128.0.0.0")) bitmask = 1;
		if(!strcmp(argv[argn],"192.0.0.0")) bitmask = 2;
		if(!strcmp(argv[argn],"224.0.0.0")) bitmask = 3;
		if(!strcmp(argv[argn],"240.0.0.0")) bitmask = 4;
		if(!strcmp(argv[argn],"248.0.0.0")) bitmask = 5;
		if(!strcmp(argv[argn],"252.0.0.0")) bitmask = 6;
		if(!strcmp(argv[argn],"254.0.0.0")) bitmask = 7;
		if(!strcmp(argv[argn],"255.0.0.0")) bitmask = 8;
		if(!strcmp(argv[argn],"255.128.0.0")) bitmask = 9;
		if(!strcmp(argv[argn],"255.192.0.0")) bitmask = 10;
		if(!strcmp(argv[argn],"255.224.0.0")) bitmask = 11;
		if(!strcmp(argv[argn],"255.240.0.0")) bitmask = 12;
		if(!strcmp(argv[argn],"255.248.0.0")) bitmask = 13;
		if(!strcmp(argv[argn],"255.252.0.0")) bitmask = 14;
		if(!strcmp(argv[argn],"255.254.0.0")) bitmask = 15;
		if(!strcmp(argv[argn],"255.255.0.0")) bitmask = 16;
		if(!strcmp(argv[argn],"255.255.128.0")) bitmask = 17;
		if(!strcmp(argv[argn],"255.255.192.0")) bitmask = 18;
		if(!strcmp(argv[argn],"255.255.224.0")) bitmask = 19;
		if(!strcmp(argv[argn],"255.255.240.0")) bitmask = 20;
		if(!strcmp(argv[argn],"255.255.248.0")) bitmask = 21;
		if(!strcmp(argv[argn],"255.255.252.0")) bitmask = 22;
		if(!strcmp(argv[argn],"255.255.254.0")) bitmask = 23;
		if(!strcmp(argv[argn],"255.255.255.0")) bitmask = 24;
		if(!strcmp(argv[argn],"255.255.255.128")) bitmask = 25;
		if(!strcmp(argv[argn],"255.255.255.192")) bitmask = 26;
		if(!strcmp(argv[argn],"255.255.255.224")) bitmask = 27;
		if(!strcmp(argv[argn],"255.255.255.240")) bitmask = 28;
		if(!strcmp(argv[argn],"255.255.255.248")) bitmask = 29;
		if(!strcmp(argv[argn],"255.255.255.252")) bitmask = 30;
		if(!strcmp(argv[argn],"255.255.255.254")) bitmask = 31;
		if(!strcmp(argv[argn],"255.255.255.255")) bitmask = 32;

		if(bitmask) printf("%d\n", bitmask);

		// parametro desconhecido
		// ignorar
	}
    if ( argn != argc ) usage();
}

// principal
void main(int argc, char *argv[]){
	argv0 = argv[0];

	// processar cada parametro e imprimir mascara correspondente
	parse_args( argc, argv );

}



