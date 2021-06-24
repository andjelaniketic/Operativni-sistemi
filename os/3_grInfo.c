#define _XOPEN_SOURCE 700
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <grp.h>
#include <stdio.h>
#include <stdlib.h>

#define check_error(cond, msg)\
	do{\
		if(!(cond)){\
			fprintf(stderr, "Greska: %s\n", msg);\
			exit(EXIT_FAILURE);\
		}\
	}while(0)

// ./a.out grpname
int main (int argc, char **argv){
	check_error(argc == 2, "./a.out grpname");

	struct group *groupInfo = getgrnam(argv[1]);
	check_error(groupInfo != NULL, "getgrnam");

	printf("Group name: %s\n", groupInfo->gr_name);
	printf("Group password: %s\n", groupInfo->gr_passwd);
	printf("Group ID: %d\n", (int)groupInfo->gr_gid);
	for(int i=0; groupInfo->gr_mem[i] != NULL; i++){

		printf("\t%s\n", groupInfo->gr_mem[i]);
	}

	exit(EXIT_SUCCESS);
}