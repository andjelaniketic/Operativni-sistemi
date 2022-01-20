#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <sys/wait.h>

#define check_error(expr,userMsg)\
	do {\
		if (!(expr)) { \
			perror(userMsg); \
			exit(EXIT_FAILURE);\
		}\
	} while (0)

#define RD_END (0)
#define WR_END (1)

#define MAX_SIZE (1024)

int main(int argc, char** argv) {
	int childToParent[2];
	check_error(pipe(childToParent) != -1, "pipe child to parent");

	int parentToChild[2];
	check_error(pipe(parentToChild) != -1, "pipe parent to child");

	pid_t childPID = fork();
	check_error(childPID != -1, "child fork failed");

	if(childPID > 0){// parent
		// zatvaramo krajeve pajpova koje ne koristimo
		close(parentToChild[RD_END]); // jer pisemo od parenta ka childu
		close(childToParent[WR_END]); // jer child cita

		char buffer[MAX_SIZE];
		sprintf(buffer, "hello");

		check_error(write(parentToChild[WR_END], buffer, strlen(buffer)) != -1, "write");

		// citanej sta je prosledio child
		int readBytes = read(childToParent[RD_END], buffer, MAX_SIZE - 1);

		check_error(readBytes != -1, "citanje iz parenta");
		// terminacija poruke
		buffer[readBytes] = 0;
		printf("%s\n", buffer);

		// ili check_error(write(STDOUT_FILENO, buffer, readBytes) != -1, "...");

		// zatvaranje koriscenih grana
		close(parentToChild[WR_END]);
		close(childToParent[RD_END]);

	}else { //child
		// zatvaranje grana
		close(parentToChild[WR_END]);
		close(childToParent[RD_END]);
		//child hvata poruku
		char poruka[MAX_SIZE];
		int readBytes = read(parentToChild[RD_END], poruka, MAX_SIZE - 1);
			// sa read enda parent to child pajpa, citamo u poruku, najvise duzine max-1
		check_error(readBytes != -1, "citanje iz childa");
		// pbavezno terminacija poruke
		poruka[readBytes] = 0;

		char buffer[MAX_SIZE];
		sprintf(buffer, "poruka od parenta: %s\n", poruka);

		check_error(write(childToParent[WR_END], buffer, strlen(buffer)) != -1, "pisanje iz childa");

		// zatvaramo koriscene grane
		close(parentToChild[RD_END]);
		close(childToParent[WR_END]);
		exit(EXIT_SUCCESS);
	}

	check_error(wait(NULL) != -1, "wait");



	exit(EXIT_SUCCESS);
}
