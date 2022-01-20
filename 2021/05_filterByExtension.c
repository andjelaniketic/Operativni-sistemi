#define _XOPEN_SOURCE 700
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>

#include <ftw.h>
#include <string.h>

#define check_error(expr,userMsg)\
	do {\
		if (!(expr)) { \
			perror(userMsg); \
			exit(EXIT_FAILURE);\
		}\
	} while (0)

char *Ext = NULL;

// ova fja se poziva za svaki fajl
int processFile(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf){
	(void*)typeflag;
	
	if(S_ISREG(sb->st_mode)){
		// provera ekstenzije
		char *ime = fpath + ftwbuf->base;
		char *ext = strrchr(ime, '.');
		// tacka ne mora da postoji
		if(ext != NULL){
			// onda mozemo da radimo
			if(strcmp(ext, Ext) == 0){
				// extenzija zadata sa tackom, inace: ext+1
				printf("%s: %s\n", fpath, fpath+ftwbuf->base);
			}
		} 
	}
	return 0;
}
int main(int argc, char** argv) {
	// da li je direktorijum
	// ako nije onda nema smisla obilaziti
	check_error(argc == 3, "args");
	Ext = argv[2]; //pamtimo trazenu ext

	struct stat fInfo;
	check_error(stat(argv[1], &fInfo) != -1, "stat");
	check_error(S_ISDIR(fInfo.st_mode), "not dir");

	// obilazak ugradjenom fjom nftw
	check_error(nftw(argv[1], processFile, 50, 0) != -1, "nftw");
	// 50 je max br fajl deskriptora koje moze  da drzi otvorenim istovremeno
	// 0 (flag polje) ponasaj se po defaulte, prati simbolicke linkove, pravi putanje peske
	exit(EXIT_SUCCESS);
}
