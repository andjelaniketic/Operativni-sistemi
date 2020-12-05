#define _XOPEN_SOURCE 700 //POSIX 2008

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
#include <errno.h>

#define osAssert(cond, msg) osErrorFatal(cond, msg, __FILE__, __LINE__)

void osErrorFatal(bool cond, const char *msg, const char *fname, int line){
	if(!cond){
		perror(msg); //user message: system message
		fprintf(stderr, "%s:%d\n", fname, line);
		exit(EXIT_FAILURE);
	}
}

void osPrintUserInfo(const struct passwd *pUserInfo){
	printf("\n");
	printf("%s\n%jd\n%jd\n%s\n%s\n%s\n", 
			pUserInfo->pw_name,
			(intmax_t)pUserInfo->pw_uid,
			(intmax_t)pUserInfo->pw_gid, 
			pUserInfo->pw_gecos, 
			pUserInfo->pw_dir,
			pUserInfo->pw_shell);
}

int main(int argc, char **argv){

	osAssert(1 == argc, "Upotreba: ./listusers");
	
	setpwent();

	struct passwd *pUserInfo = NULL;
	while((pUserInfo = getpwent()) != NULL)
	{
		osPrintUserInfo(pUserInfo);
	}
	osAssert(0 == errno, "Dohvatanje narednog korisnika nije uspelo");

	endpwent();
	return 0;
}