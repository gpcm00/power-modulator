#include<stdio.h>


int main(int argc, char** argv)
{
	for(int i = 0; i < argc; i++) {
		printf("%s\t", argv[i]);
	}

	printf("\nHello World!\n");
	return 0;
}
