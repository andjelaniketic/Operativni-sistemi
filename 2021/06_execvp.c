#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>

#include <sys/wait.h>
#include <string.h>

#define check_error(expr,userMsg)\
	do {\
		if (!(expr)) { \
			perror(userMsg); \
			exit(EXIT_FAILURE);\
		}\
	} while (0)
	
int main(int argc, char** argv) {
	check_error(argc > 1, "args");

	// gcc -std=c99 exec.c -o exec
	// char** niz = ["gcc", "-std=c99", "exec.c", "-o", "exec", NULL];
	// execvp(niz[0], niz);
	
	char **niz = malloc(argc*sizeof(char*));
	check_error(niz != NULL, "malloc");

	for(int i=1; i<argc; i++){
		niz[i-1] = malloc(strlen(argv[i] + 1));
		check_error(niz[i-1] != NULL, "...");
		strcpy(niz[i-1], argv[i]);
	}
	niz[argc - 1] = NULL;

	pid_t childPID = fork();
	check_error(childPID != -1, "fork");

	if(childPID == 0){
		check_error(execvp(niz[0], niz) != -1, "execvp");
		exit(EXIT_FAILURE);
	}

	int status = 0;
	check_error(wait(&status) != -1, "wait");

	if(WIFEXITED(status) && (WEXITSTATUS(status) == EXIT_SUCCESS)){
		printf("good\n");
	}else
		printf("bad\n");
	exit(EXIT_SUCCESS);
}
