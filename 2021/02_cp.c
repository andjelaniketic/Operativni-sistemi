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

#define MAX_SIZE (2048)
// ./a.out src dest	
int main(int argc, char** argv) {
	check_error(argc == 3, "args");

	int src = open(argv[1], O_RDONLY);	
	check_error(src != -1, "open src");

	int dest = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	check_error(dest != -1, "open dest");

	char buff[MAX_SIZE];
	int readbytes = 0;

	while((readbytes = read(src, buff, MAX_SIZE)) > 0){
		check_error(write(dest, buff, readbytes) != -1, "write");
	}

	check_error(readbytes != -1, "read");

	close(src);
	close(dest);
	exit(EXIT_SUCCESS);
}