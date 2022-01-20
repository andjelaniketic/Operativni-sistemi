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
	
	pid_t childPID = fork();
	
	if(childPID == 0){// child
		// na adresni prostor childa se ucitava novi program
		// uz pomoc execlp funkcije
		// check_error(execlp("gcc", "gcc", "-std=c99", "06_pipe.c", "-o", "pipe", NULL) != -1, "execlp");
		// to je kompilacija
		// a ako zelimo da izvrsimo program onda ovako:
		check_error(execlp("/home/adeline/Desktop/os/2021/pipe", "pipe", NULL) != -1, "execlp");
		
		exit(EXIT_FAILURE); // nece se desiti al eto nek bleji
	}

	int status = 0;
	check_error(wait(&status) != -1, "wait");

	if(WIFEXITED(status) && (WEXITSTATUS(status) == EXIT_SUCCESS)){
		printf("Uspeh\n");
	}else
		printf("Neuspeh\n");

	exit(EXIT_SUCCESS);
}
