#include <stdio.h>

int main(int argc, char *argv[]){

	int i;
	char *filename = "foo.baz";

	FILE *f = fopen(filename, "r");
	
	if(f == NULL){
		printf("The file \"foo.baz\" was not found in this directory!\n");
		return 1;
	}

	
	while((i = fgetc(f))!=EOF) {
		printf("%c", i);
	}
	
	fclose(f);
	
	return 0;
}
