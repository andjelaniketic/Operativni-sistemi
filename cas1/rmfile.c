#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //close


#define osAssert(cond, msg) osErrorFatal(cond, msg, __FILE__, __LINE__)

void osErrorFatal(bool cond, const char *msg, const char *fname, int line){
	if(!cond){
		perror(msg); //user message: system message
		fprintf(stderr, "%s:%d\n", fname, line);
		exit(EXIT_FAILURE);
	}
}


int main(int argc, char **argv){

	osAssert(3 == argc, "Upotreba: ./rmfile opcija(-d, -f) putanja");
	
	// int unlink(const char *pathname);
	// int rmdir(const char *pathname); samo prazan dir
	// rm -r <-rekurzivno brisanje praznih dir

	const char *option = argv[1];
	const char *fpath = argv[2];

	if(!strcmp(option, "-f")){
		osAssert(-1 != unlink(fpath), "Brisanje regularnog fajla nije uspelo"); 
	}
	else if(!strcmp(option, "-d"))
	{
		osAssert(-1 != rmdir(fpath), "Brisanje direktorijuma nije uspelo");
	}
	else {
		osAssert(false, "Neispravna opcija.");
	}
	return 0;
}