#include <stdio.h>
#include <stdlib.h>

#include <sys/stat.h>

int g_total = 0;

void get_filesize_and_sum(char *filename){
	int size = 0;
	struct stat st;
	stat(filename, &st);
	size = st.st_size;
	g_total += size;
}


void help_std(){
	printf("\n");
	printf("sumfilesize\n");
	printf("Soma tamanho dos arquivo e retorna total em bytes\n");
	printf("\n");
	printf("Autor: Patrick Brandao <patrickbrandao@gmail.com>, todos os direitos reservados\n");
	printf("\n");
	printf("Use: sumfilesize (file) [file] [file] [...]\n");
	printf("\n");
	exit(1);	
}

int main(int argc, char **argv){
	int i;
    if(argc<2) help_std();
	for(i=1;i<argc;i++) get_filesize_and_sum(argv[i]);

	printf("%d\n", g_total);
}
