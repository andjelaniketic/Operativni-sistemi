#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //close, read, write
#include <stdint.h> //fiksni tipovi

#define osAssert(cond, msg) osErrorFatal(cond, msg, __FILE__, __LINE__)
#define BUF_SIZE (4096)

void osErrorFatal(bool cond, const char *msg, const char *fname, int line){
	if(!cond){
		perror(msg); //user message: system message
		fprintf(stderr, "%s:%d\n", fname, line);
		exit(EXIT_FAILURE);
	}
}


int main(int argc, char **argv){

	osAssert(2 == argc, "Upotreba: ./fsize putanja_do_fajla");
	
	int fd = open(argv[1], O_RDONLY);
	osAssert(-1 != fd, "Otvaranje fajla nije uspelo");

	//off_t lseek(int fd, off_t offset, int whence);
	//typedef *long int* off_t;
	off_t endOfFilePos = lseek(fd, 0, SEEK_END);
	osAssert(-1 != endOfFilePos, "Pomeranje nije uspelo");

	printf("Velicina fajla: %jd\n", (intmax_t)endOfFilePos);
	//najsiri int oznaceni tip
	close(fd);
	return 0;
}