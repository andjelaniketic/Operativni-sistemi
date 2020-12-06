#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>

#include <errno.h>

#define check_error(cond, msg)\
	do{\
		if(!(cond)){\
			perror(msg);\
			exit(EXIT_FAILURE);\
		}\
	}while(0)

int main(int argc, char **argv){
	
	check_error(3 == argc, "Upotreba: ./umask filename pravapristupa");

	//long int strtol(const char *nptr, char **endptr, int base);
	int prava = strtol(argv[2], NULL, 8);
	
	mode_t oldumask = umask(0);

	int fd = open(argv[1], O_CREAT | O_EXCL, prava);

	check_error(-1 != fd, "greska pri kreiranju fajla");

	close(fd);
	umask(oldumask);

	return 0;
}