#include <stdio.h>

int main(int argc, char **argv){
	long int decimalNumber = 0;
	if(argc!=2) return 1;

	decimalNumber = atoi(argv[1]);
	printf("%X\n",decimalNumber);

	return 0;

}
