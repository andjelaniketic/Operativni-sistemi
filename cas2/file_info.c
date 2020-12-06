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
#include <pwd.h> //passwd struct, getpwuid
#include <grp.h> // group struct, getgrgid
#include <time.h> //ctime()

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
	char *filePath = argv[1];

	osAssert(-1 != stat(filePath, &finfo), "Dohvatanje informacija o fajlu nije uspelo");

	char prava_pristupa[11] = {0}; //inicijalizacija bloka na 0

	if(S_ISREG(finfo.st_mode))
	{
		prava_pristupa[0] = '-';
	}else if (S_ISDIR(finfo.st_mode))
	{
		prava_pristupa[0] = 'd';
	}else if (S_ISSOCK(finfo.st_mode))
	{
		prava_pristupa[0] = 's';
	}else if (S_ISLNK(finfo.st_mode))
	{
		prava_pristupa[0] = 'l';
	}else if (S_ISFIFO(finfo.st_mode))
	{
		prava_pristupa[0] = 'p';
	}else if (S_ISBLK(finfo.st_mode))
	{
		prava_pristupa[0] = 'b';
	}else if (S_ISCHR(finfo.st_mode))
	{
		prava_pristupa[0] = 'c';
	}

	prava_pristupa[1] = finfo.st_mode & S_IRUSR ? 'r' : '-';
	prava_pristupa[2] = finfo.st_mode & S_IWUSR ? 'w' : '-';
	prava_pristupa[3] = finfo.st_mode & S_IXUSR ? 'x' : '-';

	prava_pristupa[4] = finfo.st_mode & S_IRGRP ? 'r' : '-';
	prava_pristupa[5] = finfo.st_mode & S_IWGRP ? 'w' : '-';
	prava_pristupa[6] = finfo.st_mode & S_IXGRP ? 'x' : '-';

	prava_pristupa[7] = finfo.st_mode & S_IROTH ? 'r' : '-';
	prava_pristupa[8] = finfo.st_mode & S_IWOTH ? 'w' : '-';
	prava_pristupa[9] = finfo.st_mode & S_IXOTH ? 'x' : '-';

	struct passwd *pUserInfo = getpwuid(finfo.st_uid);
	osAssert(NULL != pUserInfo, "Dohvatanje podataka o vlasniku nije uspelo");

	struct group *pGroupInfo = getgrgid(finfo.st_gid);
	osAssert(NULL != pGroupInfo, "DOhvatanje podataka o vlasnickoj grupi nije uspelo");

	char *mtimeStr = ctime(&finfo.st_mtime); //vreme poslednje izmene

	printf("%s %jd %s %s %jd %s %s", 
		prava_pristupa, (intmax_t)finfo.st_nlink, 
		pUserInfo->pw_name, pGroupInfo->gr_name,
		(intmax_t)finfo.st_size, filePath, mtimeStr);

	return 0;
}