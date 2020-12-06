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
	
	check_error(3 == argc, "Upotreba: ./chmod filename pravapristupa");

	int prava = strtol(argv[2], NULL, 8);

	int fd = open(argv[1], O_CREAT, prava);
	check_error(-1 != fd, "greska");

	close(fd);

	check_error(chmod(argv[1], prava) != -1, " greska");
 
	return 0;
}