#define _XOPEN_SOURCE 700
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#define check_error(expr,userMsg)\
	do {\
		if (!(expr)) { \
			perror(userMsg); \
			exit(EXIT_FAILURE);\
		}\
	} while (0)
	
#define MAX_SIZE (1024)

void get_fileinfo(char *path, char **s);

char *getUserName(uid_t u);

char *getGroupName(gid_t g);

int main(int argc, char** argv) {
	check_error(argc == 2, "args");
	// sve info o fajlu

	char *s = NULL;
	get_fileinfo(argv[1], &s);

	printf("%s\n", s);

	free(s);	
	
	exit(EXIT_SUCCESS);
}
// ls -l
void get_fileinfo(char *path, char **s){
	check_error(*s == NULL, "...");

	char *buff = malloc(MAX_SIZE * sizeof(char));
	check_error(buff != NULL, "malloc");
	*s = buff;

	struct stat fInfo;
	check_error(stat(path, &fInfo) != -1, "stat");

	char prava[11];
	strcpy(prava, "-rwxrwxrwx");
	// sprintf(prava, "-rwxrwxrwx");
	// pretpostavaljamo da ima sva prava

	// - st_mode, & S_IFMT izdvaja bitove koji definisu format fajla
	switch(fInfo.st_mode & S_IFMT){
		case S_IFREG:
			prava[0] = '-';
			break;
		case S_IFDIR:
			prava[0] = 'd';
			break;
		case S_IFCHR:
			prava[0] = 'c';
			break;
		case S_IFBLK:
			prava[0] = 'b';
			break;
		case S_IFLNK:
			prava[0] = 'l';
			break;
		case S_IFIFO:
			prava[0] = 'p'; // cesto se zovu i pipes
			break;
		case S_IFSOCK:
			prava[0] = 's';
			break;
	}

	if(!(fInfo.st_mode & S_IRUSR)) // da li korisnik ima pravo citanja
		prava[1] = '-';
	if(!(fInfo.st_mode & S_IWUSR)) // da li korisnik ima pravo pisanja
		prava[2] = '-';
	if(!(fInfo.st_mode & S_IXUSR)) // da li korisnik ima execute pravo
		prava[3] = '-';

	if(!(fInfo.st_mode & S_IRGRP)) // da li grupa ima pravo citanja
		prava[4] = '-';
	if(!(fInfo.st_mode & S_IWGRP)) // da li grupa ima pravo pisanja
		prava[5] = '-';
	if(!(fInfo.st_mode & S_IXGRP)) // da li grupa ima execute pravo
		prava[6] = '-';

	if(!(fInfo.st_mode & S_IROTH)) // da li ostali ima pravo citanja
		prava[7] = '-';
	if(!(fInfo.st_mode & S_IWOTH)) // da li ostali ima pravo pisanja
		prava[8] = '-';
	if(!(fInfo.st_mode & S_IXOTH)) // da li ostali ima execute pravo
		prava[9] = '-';

// userinfo i prava
	int bajtovi = sprintf(buff, "%s ", prava);
	char *string = getUserName(fInfo.st_uid);
	bajtovi += sprintf(buff + bajtovi, "%s ", string);
	free(string);

// grupa
	string = getGroupName(fInfo.st_gid);
	bajtovi += sprintf(buff + bajtovi, "%s ", string);
	free(string);

// velicina fajla
	bajtovi += sprintf(buff + bajtovi, "%dB ", (int) fInfo.st_size);

// vreme ali s varanjem
	char *vreme = ctime(&(fInfo.st_mtime));
	bajtovi += sprintf(buff + bajtovi, "%s", vreme);
	buff[-- bajtovi] = 0;

	bajtovi += sprintf(buff + bajtovi, " %s ", path);
}

// stat
// velicinu fajla odredjujemo iskljucivo statovanjem
// last status change ne mozemo mi da menjamo
// S_IFMT, man 7 inode
// S_IS...

char *getUserName(uid_t u){
	struct passwd *userInfo = getpwuid(u);
	check_error(userInfo != NULL, "getpwuid");

	char* username = malloc(strlen(userInfo->pw_name) + 1);
	check_error(username != NULL, "malloc");

	strcpy(username, userInfo->pw_name);

	return username;
}

char *getGroupName(gid_t g){
	struct group *groupInfo = getgrgid(g);
	check_error(groupInfo != NULL, "getgrgid");

	char *group_name = malloc(strlen(groupInfo->gr_name) + 1);
	check_error(group_name != NULL, "malloc");

	strcpy(group_name, groupInfo->gr_name);

	return group_name;
}