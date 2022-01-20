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
	
int main(int argc, char** argv) {
	// fcntl - za upravljanje  fajl deskriptorima
	// F_GETFL F_SETFL - citanje i menjanje flagova
	// na vec otvorenom fd ne mozemo da menjamo rdonly i wronly flegove

	// saznamo flegove
	char *msg = "Pisem po stdin\n";
	int stdinFlags = fcntl(STDIN_FILENO, F_GETFL);
	check_error(stdinFlags != -1, "fcntl");

	int accessMode = stdinFlags & O_ACCMODE; // prava
	if(accessMode == O_WRONLY || accessMode == O_RDWR){
		
		check_error(write(STDIN_FILENO, msg, strlen(msg)) != -1, "write");

	}

	int fd = open("1.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
	check_error(fd != -1, "open");
	
	check_error(write(fd, msg, strlen(msg)) != -1, "write2");
	
	int ff = fcntl(fd, F_GETFL);
	check_error(ff != -1, "fcntl2");
	// ff |= O_APPEND;
	// check_error(fcntl(fd, F_SETFL, ff) != -1, "set flags");
	// check_error(lseek(fd, 0, SEEK_SET) != -1, "lseek");

	// check_error(write(fd, msg, strlen(msg)) != -1, "write3");

	ff &= ~O_WRONLY; // nece se desiti
	ff |= O_RDONLY;

	check_error(fcntl(fd, F_SETFL, ff) != -1, "set");
	check_error(write(fd, msg, strlen(msg)) != -1, "write4");


	close(fd);

	exit(EXIT_SUCCESS);
}
