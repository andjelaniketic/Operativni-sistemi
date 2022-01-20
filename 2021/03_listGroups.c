#define _XOPEN_SOURCE 700
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <grp.h>

#define check_error(expr,userMsg)\
	do {\
		if (!(expr)) { \
			perror(userMsg); \
			exit(EXIT_FAILURE);\
		}\
	} while (0)
	
int main(int argc, char** argv) {
	(void) argv;
	check_error(argc == 1, "args");
	
	setgrent();

	struct group *groupInfo;
	while((groupInfo = getgrent()) != NULL){
		printf("Group name: %s\n", groupInfo->gr_name);
		printf("Group passwd: %s\n", groupInfo->gr_passwd);
		printf("Group ID: %d\n", (int)groupInfo->gr_gid);
		
		for(int i = 0; groupInfo->gr_mem[i] != NULL; i++){
			printf("\t%s\n", groupInfo->gr_mem[i]);
		}

		printf("\n********************\n");	
	}

	endgrent();

	exit(EXIT_SUCCESS);
}
