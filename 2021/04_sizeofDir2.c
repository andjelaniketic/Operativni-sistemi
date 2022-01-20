#define _XOPEN_SOURCE 700

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>

#define check_error(expr,userMsg)\
	do {\
		if (!(expr)) { \
			perror(userMsg); \
			exit(EXIT_FAILURE);\
		}\
	} while (0)
	
void traverse_dir(char *path, unsigned *size);

int main(int argc, char** argv) {
	
	check_error(argc == 2, "args");

	struct stat finfo;
	check_error(stat(argv[1], &finfo) != -1, "stat");
	check_error(S_ISDIR(finfo.st_mode), "not a dir");

	unsigned size = 0;
	traverse_dir(argv[1], &size);

	printf("size of dir: %dB\n", size);
	exit(EXIT_SUCCESS);
}

void traverse_dir(char *path, unsigned *size){
	struct stat finfo;
	check_error(lstat(path, &finfo) != -1, "lstat");

	*size += finfo.st_size;

	if(!S_ISDIR(finfo.st_mode))
		return;

	char *dirPath = realpath(path, NULL);
	printf("Dir: (%s)\n", dirPath);
	free(dirPath);

	DIR *dir = opendir(path);
	check_error(dir != NULL, "opendir");

	// ulazimo u direktorijum
	check_error(chdir(path) != -1, "chdir");

	struct dirent *dirEntry = NULL;
	errno = 0;
	while((dirEntry = readdir(dir)) != NULL){
		// ispis
		char *file = realpath(dirEntry->d_name, NULL);
		printf("\tFajl: %s: %s\n", file, dirEntry->d_name);
		free(file);
		//	
		
		if(!strcmp(dirEntry->d_name, ".") || !strcmp(dirEntry->d_name, "..")){
			check_error(stat(dirEntry->d_name, &finfo) != -1, "stat");
			*size += finfo.st_size;
			errno = 0;
			continue;
		}
		traverse_dir(dirEntry->d_name, size);
		errno = 0;
	}

	check_error(errno != EBADF, "readdir");
	check_error(chdir("..") != -1, "chdir out"); // vracamo se u parent
	check_error(closedir(dir) != -1, "closedir");
}

// char * dirPath = realpath(path, NULL)
// stampa putanju gde smo atm, celu putanju
// free(dirPath);