#define _XOPEN_SOURCE 700
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>

#include <pwd.h>

#define check_error(expr,userMsg)\
	do {\
		if (!(expr)) { \
			perror(userMsg); \
			exit(EXIT_FAILURE);\
		}\
	} while (0)
	
int main(int argc, char** argv) {
	check_error(argc == 1, "args");

	setpwent(); 
		// otvara fajl, postavlja offset na pocetak

	struct passwd *userinfo;
	while( (userinfo = getpwent()) != NULL ){
			// cita narednu liniju iz passwd fajl
			// raspakuje liniju u strukturu i vraca pokazivac
		printf("Username: %s\n", userinfo->pw_name);
		printf("User ID: %d\n", (int)userinfo->pw_uid);
		printf("Group ID: %d\n", (int)userinfo->pw_gid);
		
		printf("Other info: %s\n", userinfo->pw_gecos);
		printf("Home Dir: %s\n", userinfo->pw_dir);
		printf("Shell: %s\n", userinfo->pw_shell);

		printf("\n************************\n");
	}
	endpwent();
		// zatvara passwd fajl

	exit(EXIT_SUCCESS);
}
