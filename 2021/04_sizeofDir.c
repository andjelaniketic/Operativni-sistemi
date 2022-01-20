#define _XOPEN_SOURCE 700

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#include <errno.h>

#define check_error(expr,userMsg)\
	do {\
		if (!(expr)) { \
			perror(userMsg); \
			exit(EXIT_FAILURE);\
		}\
	} while (0)
	
void traverse_dir(char *path, unsigned *size);

int main(int argc, char** argv) {
	
	// obilazak direktorijuma
	// . je referenca na samog sebe
	// .. je referenca na parent dir
	// radi kao DFS
	// stat dereferencira soft links
	// fajlove tokom obilaska ispitujemo sa lstat
	// lstat daje bas informacije o simbolickim linkovima
		// ne dereferencira


	// ./a.out path
	// trazimo velicinu direktorijuma

	check_error(argc == 2, "args");

	// da li je argument zapravo direktorijum
	// statujemo
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

	// ako nije dir to je to
	if(!S_ISDIR(finfo.st_mode))
			return;

	// ako jeste dir
	DIR *dir = opendir(path);
	check_error(dir != NULL, "opendir");
	
	// listamo dir - readdir 
	struct dirent *dirEntry = NULL;
	errno = 0;
	//citamo rekurzivno
	while((dirEntry = readdir(dir)) != NULL){
		char *putanja = malloc(strlen(path)+strlen(dirEntry->d_name)+2);
		check_error(putanja != NULL, "malloc");

		strcpy(putanja, path);
		strcat(putanja, "/");
		strcat(putanja, dirEntry->d_name);
		// sprintf(putanja, "%s/%s", path, dirEntry->d_name);
		
		if(!strcmp(dirEntry->d_name, ".") || !strcmp(dirEntry->d_name, "..")){
			check_error(stat(putanja, &finfo) != -1, "stat");

			*size += finfo.st_size;

			free(putanja);
			continue;
		}

	 	traverse_dir(putanja, size);
	 	free(putanja);
	 	errno = 0;
	}
	check_error(errno != EBADF, "readdir");
	check_error(closedir(dir) != -1, "closedir");
}
