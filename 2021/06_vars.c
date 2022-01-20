#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>

#include <sys/wait.h>

#define check_error(expr,userMsg)\
	do {\
		if (!(expr)) { \
			perror(userMsg); \
			exit(EXIT_FAILURE);\
		}\
	} while (0)
	
int main(int argc, char** argv) {
	int x = 10;
	int y = 5;

	pid_t child = fork();
	check_error(child != -1, "fork");
	// prave se kopije promenljivih
	// parent i child imaju potpuno odvojene memorijske prostore
	// ne uticu jedno drugom na vr promenljivih
	
	if(child > 0){
		// parent
		x += 20;
		y -= 20;
		printf("Parent: x = %d y = %d\n", x, y);
	}else {
		// child
		x -= 20;
		y += 20;
		printf("Child: x = %d y = %d\n", x, y);

		exit(EXIT_SUCCESS);
	}
	
	printf("Parent: x = %d y = %d\n", x, y);

	int status = 0;
	check_error(wait(&status) != -1, "wait");

	if(WIFEXITED(status) && (WEXITSTATUS(status) == EXIT_SUCCESS)){
		printf("good\n");
	}else
		printf("bad\n");

	exit(EXIT_SUCCESS);
}
