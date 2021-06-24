#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define check_error(cond, msg)\
	do{\
		if(!(cond)){\
			fprintf(stderr, "Greska: %s\n", msg);\
			exit(EXIT_FAILURE);\
		}\
	}while(0)

// ./a.out src dest
int main (int argc, char **argv){
	check_error(argc == 3, "./a.out src dest");

	int srcfd = open(argv[1], O_RDONLY);
	check_error(srcfd != -1, "open");

	int destfd = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	check_error(destfd != -1, "open2");

	char *bafer = malloc(2048);
	check_error(bafer != NULL, "malloc");

	int procitano = 0;
	while((procitano = read(srcfd, bafer, 2048)) > 0){
		check_error(write(destfd, bafer, procitano) != -1, "write");
	}
	check_error(procitano != -1, "read");

	free(bafer);
	close(srcfd);
	close(destfd);	

	exit(EXIT_SUCCESS);
}