#define _XOPEN_SOURCE 700
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <dirent.h> //opendir
#include <errno.h>
#include <limits.h>

#define osAssert(cond, msg) osErrorFatal(cond, msg, __FILE__, __LINE__)

void osErrorFatal(bool cond, const char *msg, const char *fname, int line){
	if(!cond){
		perror(msg);
		fprintf(stderr, "%s: %d\n", fname, line);
		exit(EXIT_FAILURE);
	}
}

void osTraverseDir(const char *fpath, unsigned *psize){
	char *abspath = realpath(fpath, NULL);
	osAssert(NULL != abspath, "greska");

	fprintf(stderr, "putanja: %s\n", abspath);
	free(abspath);

	struct stat finfo;
	osAssert(-1 != lstat(fpath, &finfo), "dohvatanje neuspelo");

	*psize += finfo.st_size;

	if(S_ISDIR(finfo.st_mode))
	{
		DIR *pDir = opendir(fpath);
		osAssert(NULL != pDir, "otvaranje neuspelo");
		osAssert(-1 != chdir(fpath), "promena radnog dir nije uspela");

		struct dirent *pEntry = NULL;
		while(NULL != (pEntry = readdir(pDir))){
			if(!strcmp(".", pEntry->d_name) || 
				!strcmp("..", pEntry->d_name))
				continue;

			osTraverseDir(pEntry->d_name, psize);
		}
		osAssert(0 == errno, "greska pri izlistavanju");
		osAssert(-1 != chdir(".."), "greska");

		closedir(pDir);
	} else
		return;

}

int main(int argc, char **argv){

	osAssert(2 == argc, "Upotreba: ./sizeofdir putanja");

	struct stat finfo;
	osAssert(-1 != lstat(argv[1], &finfo), "dohvatanje info o direktorijumu nije uspelo");

	osAssert(S_ISDIR(finfo.st_mode), "nije putanja do dir");

	unsigned size = 0U;
	osTraverseDir(argv[1], &size);

	printf("Ukupna velicina: %u\n", size);
	return 0;
}