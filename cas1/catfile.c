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

	osAssert(2 == argc, "Upotreba: ./catfile putanja");
	
	int fd = open(argv[1], O_RDONLY);
	osAssert(-1 != fd, "Otvaranje fajla nije uspelo");

	//ssize_t read(int fd, void *buf, size_t count);
	char buf[BUF_SIZE];
	int bytesRead = 0;

	while((bytesRead = read(fd, buf, sizeof buf)) > 0 )
	{
		//ssize_t write(int fd, const void *buf, size_t count);
		osAssert(-1 != write(STDOUT_FILENO, buf, bytesRead), "Pisanje nije uspelo");
	}

	osAssert(-1 != bytesRead, "Doslo je do greske prilikom citanja"); //read vrati -1 znaci greska
	
	close(fd);
	return 0;
}