#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //close


#define osAssert(cond, msg) osErrorFatal(cond, msg, __FILE__, __LINE__)

void osErrorFatal(bool cond, const char *msg, const char *fname, int line){
	if(!cond){
		perror(msg); //user message: system message
		fprintf(stderr, "%s:%d\n", fname, line);
		exit(EXIT_FAILURE);
	}
}

bool osOpenFile(const char *fpath, const char *mode, int *pFd){
	// int open(const char *pathname, int flags, mode_t mode);
	mode_t accessMode = 0666; //octal, prava pristupa
	
	int flags = 0;
	if(!strcmp(mode, "r"))
	{
		flags |= O_RDONLY;
	}
	else if(!strcmp(mode, "w"))
	{
		flags |= O_WRONLY;
		flags |= O_TRUNC;
		flags |= O_CREAT;
	}
	else if(!strcmp(mode, "a"))
	{
		flags |= O_WRONLY;
		flags |= O_APPEND;
		flags |= O_CREAT;
				
	}
	else if(!strcmp(mode, "r+"))
	{
		flags |= O_RDWR;
	}
	else if(!strcmp(mode, "w+"))
	{
		flags |= O_RDWR;
		flags |= O_APPEND;
		flags |= O_CREAT;
	}
	else if(!strcmp(mode, "a+"))
	{
		flags |= O_RDWR;
		flags |= O_APPEND;
		flags |= O_CREAT;
	}
	
	*pFd = open(fpath, flags, accessMode); //file descriptor
	return *pFd != -1; //true/false, open u slucaju greske vraca -1
}

int main(int argc, char **argv){

	osAssert(3 == argc, "Upotreba: ./mkfile putanja mod (r, w, ...)");
	
	int fd = 0;
	osAssert(osOpenFile(argv[1], argv[2], &fd), "Otvaranje/kreiranje fajla nije uspelo:");
	
	printf("Fajl je uspesno kreiran/otvoren\n");
	
	close(fd);

	return 0;
}