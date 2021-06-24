#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

//int open(const char *pathname, int flags);

#define check_error(cond, msg)\
	do{\
		if(!(cond)){\
			fprintf(stderr, "Greska: %s\n", msg);\
			exit(EXIT_FAILURE);\
		}\
	}while(0)


int my_fopen(const char *putanja, const char *mode){
	
	int flags = 0;

	switch (mode[0]){
		case 'r':
			flags |= mode[1] == '+' ? O_RDWR : O_RDONLY;
			break;
		case 'w':
			flags |= mode[1] == '+' ? O_RDWR : O_WRONLY;
			flags |= O_CREAT;
			flags |= O_TRUNC;
			break;
		case 'a':
			flags |= mode[1] == '+' ? O_RDWR : O_WRONLY;
			flags |= O_CREAT;
			flags |= O_APPEND;
			break;
		default:
			return -1;
	}
	mode_t prava_pristupa = 0777;

	int file = open(putanja, flags, prava_pristupa);

	if(file == -1){
		fprintf(stderr, "file open failed\n");
		exit(EXIT_FAILURE);
	}

	return file;

}
// ocekivan ulaz s komandne linije: ./ putanja mod
int main (int argc, char **argv){
	check_error(argc == 3, "./2_fopen putanja mode");

	int fd = my_fopen(argv[1], argv[2]);
	check_error(fd != -1, "Fajl nije otvoren");

	close(fd);

	exit(EXIT_SUCCESS);
}