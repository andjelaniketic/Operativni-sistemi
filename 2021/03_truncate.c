#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <utime.h>

#define check_error(expr,userMsg)\
	do {\
		if (!(expr)) { \
			perror(userMsg); \
			exit(EXIT_FAILURE);\
		}\
	} while (0)
	
int main(int argc, char** argv) {
	check_error(argc == 2, "args");

	struct stat fInfo;
	check_error(stat(argv[1], &fInfo) != -1, "stat");

	FILE* f = fopen(argv[1], "w"); // brise prethodni sadrzaj fajla
	fclose(f);
	
	// man 2 utime
	struct utimbuf oldTime;
	oldTime.actime = fInfo.st_atime;
	oldTime.modtime = fInfo.st_mtime;

	check_error(utime(argv[1], &oldTime) != -1, "utime");

	exit(EXIT_SUCCESS);
}
