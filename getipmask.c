/*
	Recebe o numero de bits da mascara ipv4 e retorna em ip decimal
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static void parse_args( int argc, char** argv );

//varivel para cor da fonte
int bitmask = 24;

// mascara de saida
// char _decimal_mask[16];

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
		if(!strcmp(argv[argn],"0")){ printf("0.0.0.0\n"); bitmask = 1; continue; }
		if(!strcmp(argv[argn],"1")){ printf("128.0.0.0\n"); bitmask = 1; continue; }
		if(!strcmp(argv[argn],"2")){ printf("192.0.0.0\n"); bitmask = 2; continue; }
		if(!strcmp(argv[argn],"3")){ printf("224.0.0.0\n"); bitmask = 3; continue; }
		if(!strcmp(argv[argn],"4")){ printf("240.0.0.0\n"); bitmask = 4; continue; }
		if(!strcmp(argv[argn],"5")){ printf("248.0.0.0\n"); bitmask = 5; continue; }
		if(!strcmp(argv[argn],"6")){ printf("252.0.0.0\n"); bitmask = 6; continue; }
		if(!strcmp(argv[argn],"7")){ printf("254.0.0.0\n"); bitmask = 7; continue; }
		if(!strcmp(argv[argn],"8")){ printf("255.0.0.0\n"); bitmask = 8; continue; }
		if(!strcmp(argv[argn],"9")){ printf("255.128.0.0\n"); bitmask = 9; continue; }
		if(!strcmp(argv[argn],"10")){ printf("255.192.0.0\n"); bitmask = 10; continue; }
		if(!strcmp(argv[argn],"11")){ printf("255.224.0.0\n"); bitmask = 11; continue; }
		if(!strcmp(argv[argn],"12")){ printf("255.240.0.0\n"); bitmask = 12; continue; }
		if(!strcmp(argv[argn],"13")){ printf("255.248.0.0\n"); bitmask = 13; continue; }
		if(!strcmp(argv[argn],"14")){ printf("255.252.0.0\n"); bitmask = 14; continue; }
		if(!strcmp(argv[argn],"15")){ printf("255.254.0.0\n"); bitmask = 15; continue; }
		if(!strcmp(argv[argn],"16")){ printf("255.255.0.0\n"); bitmask = 16; continue; }
		if(!strcmp(argv[argn],"17")){ printf("255.255.128.0\n"); bitmask = 17; continue; }
		if(!strcmp(argv[argn],"18")){ printf("255.255.192.0\n"); bitmask = 18; continue; }
		if(!strcmp(argv[argn],"19")){ printf("255.255.224.0\n"); bitmask = 19; continue; }
		if(!strcmp(argv[argn],"20")){ printf("255.255.240.0\n"); bitmask = 20; continue; }
		if(!strcmp(argv[argn],"21")){ printf("255.255.248.0\n"); bitmask = 21; continue; }
		if(!strcmp(argv[argn],"22")){ printf("255.255.252.0\n"); bitmask = 22; continue; }
		if(!strcmp(argv[argn],"23")){ printf("255.255.254.0\n"); bitmask = 23; continue; }
		if(!strcmp(argv[argn],"24")){ printf("255.255.255.0\n"); bitmask = 24; continue; }
		if(!strcmp(argv[argn],"25")){ printf("255.255.255.128\n"); bitmask = 25; continue; }
		if(!strcmp(argv[argn],"26")){ printf("255.255.255.192\n"); bitmask = 26; continue; }
		if(!strcmp(argv[argn],"27")){ printf("255.255.255.224\n"); bitmask = 27; continue; }
		if(!strcmp(argv[argn],"28")){ printf("255.255.255.240\n"); bitmask = 28; continue; }
		if(!strcmp(argv[argn],"29")){ printf("255.255.255.248\n"); bitmask = 29; continue; }
		if(!strcmp(argv[argn],"30")){ printf("255.255.255.252\n"); bitmask = 30; continue; }
		if(!strcmp(argv[argn],"31")){ printf("255.255.255.254\n"); bitmask = 31; continue; }
		if(!strcmp(argv[argn],"32")){ printf("255.255.255.255\n"); bitmask = 32; continue; }

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



