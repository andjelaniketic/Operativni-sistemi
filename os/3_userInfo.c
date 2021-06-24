#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>

#define check_error(cond, msg)\
	do{\
		if(!(cond)){\
			fprintf(stderr, "Greska: %s\n", msg);\
			exit(EXIT_FAILURE);\
		}\
	}while(0)

// ./a.out username
int main (int argc, char **argv){
	check_error(argc == 2, "./a.out username");

	struct passwd *userInfo = getpwnam(argv[1]);
	check_error(userInfo != NULL, "getpwnam");

	fprintf(stdout, "username: %s \n", userInfo->pw_name);
	fprintf(stdout, "user ID: %d \n", (int)userInfo->pw_uid);
	fprintf(stdout, "group ID: %d \n", (int)userInfo->pw_gid);
	fprintf(stdout, "user info: %s \n", userInfo->pw_gecos);
	fprintf(stdout, "home: %s \n", userInfo->pw_dir);
	fprintf(stdout, "shell: %s \n", userInfo->pw_shell);



	exit(EXIT_SUCCESS);
}