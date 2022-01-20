#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define check_error(expr,userMsg)\
	do {\
		if (!(expr)) { \
			perror(userMsg); \
			exit(EXIT_FAILURE);\
		}\
	} while (0)
// ./a.out -f|-d path
	//     fajl ili dir
int main(int argc, char** argv) {
	check_error(argc == 3, "args");

	if (strcmp(argv[1], "-f") == 0){
		// fajl
		check_error(unlink(argv[2]) != -1, "unlink");
	}
	else if (strcmp(argv[1], "-d") == 0){
		// dir
		check_error(rmdir(argv[2]) != -1, "rmdir");
	}
	else{
		check_error(0, "nepostojeca opcija");
	}

	exit(EXIT_SUCCESS);
}