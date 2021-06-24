#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#define MAX (2048)
#define check_error(cond, msg)\
	do{\
		if(!(cond)){\
			fprintf(stderr, "Greska: %s\n", msg);\
			exit(EXIT_FAILURE);\
		}\
	}while(0)

// ./a.out putanja 
int main (int argc, char **argv){
	check_error(argc == 2, "./a.out putanja");

	int fd = open(argv[1], O_RDONLY);
	check_error(fd != -1, "open");

	char bafer[MAX];
	int procitano = 0;

// ssize_t read(int fd, void *buf, size_t count);
	while((procitano = read(fd, bafer, MAX)) > 0){
// ssize_t write(int fd, const void *buf, size_t count);
		check_error(write(STDOUT_FILENO, bafer, procitano) != -1, "write");
	}

	check_error(procitano != -1, "read");
	
	close(fd);

	exit(EXIT_SUCCESS);
}