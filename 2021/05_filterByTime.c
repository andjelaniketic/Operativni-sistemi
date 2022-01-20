#define _XOPEN_SOURCE 700
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>

#include <ftw.h>
#include <time.h>

#define check_error(expr,userMsg)\
	do {\
		if (!(expr)) { \
			perror(userMsg); \
			exit(EXIT_FAILURE);\
		}\
	} while (0)

time_t now;
int processFile(const char *fpath, const struct stat *sb, int typeflag, struct FTW* ftwbuf){
// trazimo sve fajlove modifikovane u zadnjih 7 dana
	if(typeflag == FTW_F){
		// reg fajl
		time_t diff = (now - sb->st_mtime)/(24*60*60);
		if(diff < 7){
			// u zadnjih 7 dana
			printf("%s: %s\n", fpath, fpath + ftwbuf->base);
		}
	}
	return 0;
}

int main(int argc, char** argv) {
	// ./a.out path
	check_error(argc == 2, "args");

	struct stat fInfo;
	check_error(stat(argv[1], &fInfo) != -1, "stat");
	check_error(S_ISDIR(fInfo.st_mode), "not dir");
// vreme u sekundama od epohe
	check_error(time(&now) != -1, "time");

	check_error(nftw(argv[1], processFile, 50, 0) != -1, "nftw");

	
	exit(EXIT_SUCCESS);
}
