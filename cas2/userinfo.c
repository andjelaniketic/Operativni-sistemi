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

	osAssert(2 == argc, "Upotreba: ./userinfo username");
	
	//struct passwd *getpwnam(const char *name);
	struct passwd *pUserInfo = getpwnam(argv[1]);
	//ne sme da se radi free jer nije dinamicka memorija
	osAssert(NULL != pUserInfo, "Doslo je do greske prilikom dohvatanja korisnika");
	
	printf("%s\n%jd\n%jd\n%s\n%s\n%s\n", 
			pUserInfo->pw_name,
			(intmax_t)pUserInfo->pw_uid,
			(intmax_t)pUserInfo->pw_gid, 
			pUserInfo->pw_gecos, 
			pUserInfo->pw_dir,
			pUserInfo->pw_shell);

	return 0;
}