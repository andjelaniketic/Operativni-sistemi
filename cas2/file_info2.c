#define _XOPEN_SOURCE 700
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include <time.h>

#define MAX_SIZE (512)

#define osAssert(cond, msg) osErrorFatal(cond, msg, __FILE__, __LINE__)

void osErrorFatal(bool cond, const char *msg, const char *fname, int line){
	if(!cond){
		perror(msg);
		fprintf(stderr, "%s:%d\n", fname, line);
		exit(EXIT_FAILURE);
	}
}

char *get_username(uid_t uid){
	struct passwd *userinfo = getpwuid(uid);
	osAssert(NULL != userinfo, "greska pri dohvatanju usernamea");
	
	char *name = malloc(strlen(userinfo->pw_name) + 1);
	osAssert(NULL != name, "greska pri alokaciji");

	strcpy(name, userinfo->pw_name);
	return name;
}

char *get_groupname(gid_t gid){
	struct group *grinfo = getgrgid(gid);
	osAssert(NULL != grinfo, "greska pri dohvatanju usernamea");
	
	char *name = malloc(strlen(grinfo->gr_name) + 1);
	osAssert(NULL != name, "greska pri alokaciji");

	strcpy(name, grinfo->gr_name);
	return name;
}

void file_info(char *filePath, char **s){
	errno = EINVAL;
	osAssert(*s == NULL, "inicijalizacija");
	errno = 0;

	struct stat finfo;
	osAssert(-1 != stat(filePath, &finfo), "Dohvatanje informacija o fajlu nije uspelo");

	char *buffer = malloc(MAX_SIZE*sizeof(char));
	osAssert(NULL != buffer, "Greska pri alokaciji bafera");

	*s = buffer;

	char prava_pristupa[11];
	strcpy(prava_pristupa, "-rwxrwxrwx"); //assume da imamo sva prava pristupa

	//kog tipa je fajl
	//S_IFMT <- info o formatu fajla
	switch(finfo.st_mode & S_IFMT){
		case S_IFREG:
			prava_pristupa[0] = '-';
			break;
		case S_IFDIR:
			prava_pristupa[0] = 'd';
			break;
		case S_IFCHR:
			prava_pristupa[0] = 'c';
			break;
		case S_IFBLK:
			prava_pristupa[0] = 'b';
			break;
		case S_IFLNK:
			prava_pristupa[0] = 'l';
			break;
		case S_IFIFO:
			prava_pristupa[0] = 'p';
			break;
		case S_IFSOCK:
			prava_pristupa[0] = 's';
			break;
	}

	//prepravka prava pristupa
	if(!(finfo.st_mode & S_IRUSR))
		prava_pristupa[1] = '-';
	if(!(finfo.st_mode & S_IWUSR))
		prava_pristupa[2] = '-';
	if(!(finfo.st_mode & S_IXUSR))
		prava_pristupa[3] = '-';
		
	if(!(finfo.st_mode & S_IRGRP))
		prava_pristupa[4] = '-';
	if(!(finfo.st_mode & S_IWGRP))
		prava_pristupa[5] = '-';
	if(!(finfo.st_mode & S_IXGRP))
		prava_pristupa[6] = '-';
	
	if(!(finfo.st_mode & S_IROTH))
		prava_pristupa[7] = '-';
	if(!(finfo.st_mode & S_IWOTH))
		prava_pristupa[8] = '-';
	if(!(finfo.st_mode & S_IXOTH))
		prava_pristupa[9] = '-';
	
	//kopiramo prava pristupa u bafer	
	int bytes = sprintf(buffer, "%s ", prava_pristupa);

	//ime vlasnika i grupe
	char *username = get_username(finfo.st_uid);
	bytes += sprintf(buffer + bytes, "%s ", username);
	free(username);

	char *grname = get_groupname(finfo.st_gid);
	bytes += sprintf(buffer + bytes, "%s ", grname);
	free(grname);

	//velicina fajla
	bytes += sprintf(buffer + bytes, "%d ", (int)finfo.st_size);

	//vreme poslednje modifikacije
	char *vreme = ctime(&(finfo.st_mtime));
	bytes += sprintf(buffer + bytes, "%s", vreme);
	buffer[--bytes] = 0;

	//putanja
	bytes += sprintf(buffer + bytes, " %s ", filePath);
 }

int main(int argc, char **argv){
	osAssert(2 == argc, "Upotreba: ./fileinfo file_path");

	char *s = NULL;
	file_info(argv[1], &s);

	printf("%s\n", s);
	free(s);
	return 0;
}