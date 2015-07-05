#include <stdio.h>
#include <stdlib.h>

#include <sys/stat.h>

void get_filesize(char *filename){
	int size = 0;
	struct stat st;
	stat(filename, &st);
	size = st.st_size;
	printf("%d\n", size);
}


void help_std(){
	printf("\n");
	printf("filesize\n");
	printf("Informa tamanho do arquivo em bytes\n");
	printf("\n");
	printf("Autor: Patrick Brandao <patrickbrandao@gmail.com>, todos os direitos reservados\n");
	printf("\n");
	printf("Use: filezie (file)\n");
	printf("\n");
	exit(1);	
}

int main(int argc, char **argv){
	int i;
    if(argc<2) help_std();
	for(i=1;i<argc;i++) get_filesize(argv[i]);
}
