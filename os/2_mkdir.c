#include <sys/stat.h>
#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>

#define check_error(cond, msg)\
	do{\
		if(!(cond)){\
			fprintf(stderr, "Greska: %s\n", msg);\
			exit(EXIT_FAILURE);\
		}\
	}while(0)

// int mkdir(const char *pathname, mode_t mode);
// ./2_mkdir putanja prava_pristupa
int main (int argc, char **argv){

	check_error(argc == 3, "./2_mkdir putanja prava_pristupa");

// long int strtol(const char *nptr, char **endptr, int base);
	int mode = strtol(argv[2], NULL, 8);
	int retval = mkdir(argv[1], mode);

	check_error(retval != -1, "mkdir error");
	exit(EXIT_SUCCESS);
}