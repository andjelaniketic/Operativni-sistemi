#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>

#define check_error(expr,userMsg)\
	do {\
		if (!(expr)) { \
			perror(userMsg); \
			exit(EXIT_FAILURE);\
		}\
	} while (0)
	
int main(int argc, char** argv) {
	
	// chmod menja prava prisutpa postojeceg fajla
	// postavlja prava bas ona koja zelimo
	// ./a.out imefajla pravapristupa
	check_error(argc == 3, "args");

	int prava = strtol(argv[2], NULL, 8);

	int fd = open(arv[1], O_CREAT, prava);
	check_error(fd != -1, "open");

	close(fd);

	// ovde menjamo prava pristupa, nakon sto znamo da fajl postoji

	check_error(chmod(argv[1], prava) != -1, "chmod");


	exit(EXIT_SUCCESS);
}
