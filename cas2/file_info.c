#define _XOPEN_SOURCE 700

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //close, read, write
#include <stdint.h> //fiksni tipovi
#include <pwd.h> //passwd struct

#define osAssert(cond, msg) osErrorFatal(cond, msg, __FILE__, __LINE__)

void osErrorFatal(bool cond, const char *msg, const char *fname, int line){
	if(!cond){
		perror(msg); //user message: system message
		fprintf(stderr, "%s:%d\n", fname, line);
		exit(EXIT_FAILURE);
	}
}


int main(int argc, char **argv){

	osAssert(2 == argc, "Upotreba: ./file_info putanja");
	
	struct stat finfo;
	osAssert(-1 != stat(argv[1], &finfo), "Dohvatanje informacija o fajlu nije uspelo");

	char buf[11] = {0}; //inicijalizacija bloka na 0

	if(S_ISREG(finfo.st_mode))
	{
		buf[0] = '-';
	}else if (S_ISDIR(finfo.st_mode))
	{
		buf[0] = 'd';
	}else if (S_ISSOCK(finfo.st_mode))
	{
		buf[0] = 's';
	}else if (S_ISLNK(finfo.st_mode))
	{
		buf[0] = 'l';
	}else if (S_ISFIFO(finfo.st_mode))
	{
		buf[0] = 'p';
	}else if (S_ISBLK(finfo.st_mode))
	{
		buf[0] = 'b';
	}else if (S_ISCHR(finfo.st_mode))
	{
		buf[0] = 'c';
	}

	buf[1] = finfo.st_mode & S_IRUSR ? 'r' : '-';
	buf[2] = finfo.st_mode & S_IWUSR ? 'w' : '-';
	buf[3] = finfo.st_mode & S_IXUSR ? 'x' : '-';

	buf[4] = finfo.st_mode & S_IRGRP ? 'r' : '-';
	buf[5] = finfo.st_mode & S_IWGRP ? 'w' : '-';
	buf[6] = finfo.st_mode & S_IXGRP ? 'x' : '-';

	buf[7] = finfo.st_mode & S_IROTH ? 'r' : '-';
	buf[8] = finfo.st_mode & S_IWOTH ? 'w' : '-';
	buf[9] = finfo.st_mode & S_IXOTH ? 'x' : '-';

	printf("%s\n", buf);
	return 0;
}