#define _XOPEN_SOURCE 700
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

	pid_t childPid = fork();
	check_error(childPid != -1, "child process creation failed");

	if(childPid > 0){
		// parent
		printf("Parent:\n\tMy ID: %d\n\tChild ID: %d\n", (int) getpid(), (int)childPid);
	}else {
		// child
		printf("Child:\n\tMy ID: %d\n\tParent ID: %d\n", (int) getpid(), (int)getppid());
		exit(EXIT_SUCCESS); // zavrsava se CHILD
	}

	// deljeni deo koda
	printf("stampaju oba procesa\n");

	// cekanje
	int status = 0;
	check_error(wait(&status) != -1, "wait");

	if(WIFEXITED(status)){
		// da li se program zavrsio kontrolisano
		// tj samostalno je pozvao exit
		// provera exit koda
		if(WEXITSTATUS(status) == EXIT_SUCCESS){
			printf("Uspeh\n");
		}else{
			printf("Neuspeh\n");
		}
	}else{
		// nema smisla provera exit koda ako proces nije
		// ni pozvao exit
		printf("Neuspeh\n");
	}


	printf("Child se zavrsio.\n");

	exit(EXIT_SUCCESS);
}
