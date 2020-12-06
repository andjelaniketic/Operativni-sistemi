#define _XOPEN_SOURCE 700
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <utime.h>

#define osAssert(cond, msg) osErrorFatal(cond, msg, __FILE__, __LINE__)

void osErrorFatal(bool cond, const char *msg, const char *fname, int line){
	if(!cond){
		perror(msg);
		fprintf(stderr, "%s: %d\n", fname, line);
		exit(EXIT_FAILURE);
	}
}
int main(int argc, char **argv){

	osAssert(3 == argc, "Upotreba: ./umask_demo putanja prava");

	const char *fpath = argv[1];
	const char *accesStr = argv[2];

	char *endptr = NULL;
	mode_t accessmode = (mode_t)strtol(accesStr, &endptr, 8);
	osAssert(0 == endptr, "prosledjeni arg nije broj");

	mode_t old_mask = umask(0);

	int fd = open (fpath, O_CREAT, accessmode);
	osAssert(-1 != fd, "kreiranje fajla nije uspelo");
	close(fd);

	umask(old_mask);

	return 0;
}