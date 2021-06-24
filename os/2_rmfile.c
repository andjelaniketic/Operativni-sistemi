#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define check_error(cond, msg)\
	do{\
		if(!(cond)){\
			fprintf(stderr, "Greska: %s\n", msg);\
			exit(EXIT_FAILURE);\
		}\
	}while(0)

// ./prog -f|-d putanja
int main (int argc, char **argv){

	check_error(argc == 3, "./prog -f|-d putanja");

	if(strcmp(argv[1], "-f") == 0){
		// file
		check_error(unlink(argv[2]) != -1, "unlink");

	}else if(strcmp(argv[1], "-d") == 0){
		// dir, ocekuje prazan dir
		check_error(rmdir(argv[2]) != -1, "rmdir");
	
	}else{
		check_error(0, "silly");
	}


	exit(EXIT_SUCCESS);
}