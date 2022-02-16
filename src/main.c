#include <stdio.h>
#include <stdlib.h>
#include "include/compiler.h"

int main(int argc, char *argv[]) {
	if(argc != 2) {
		printf("usage: elly <file>\n");	
		return 1;
	}
		
	compile(argv[1]);

	return 0;
}
