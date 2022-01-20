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

int main(int argc, char** argv) {
	
	check_error(argc == 2, "argumenti");

	int fd = open(argv[1], O_RDONLY);
	check_error(fd != -1, "open");

	char buff[MAX_SIZE];
	int read_bytes = 0;

	while((read_bytes = read(fd, buff, MAX_SIZE)) > 0){
		// ispis
		check_error((write(STDOUT_FILENO, buff, read_bytes)) != -1, "write");
		//STDOUT_FILENO je fd za stdout
	}

	check_error(read_bytes != -1, "read");

	close(fd);

	exit(EXIT_SUCCESS);
}
