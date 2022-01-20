#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <sys/time.h>

#define check_error(expr,userMsg)\
	do {\
		if (!(expr)) { \
			perror(userMsg); \
			exit(EXIT_FAILURE);\
		}\
	} while (0)
	
int main(int argc, char** argv) {
	time_t now;
	check_error(time(&now) != -1, "time");

	printf("%d\n", (int)now);
	
	struct tm* brokenTime = localtime(&now);
	check_error(brokenTime != NULL, "localtime");

	// pomerimo 1god unapred
	brokenTime->tm_year++;
	// local time nije pametan
	// za najbolje rezultate, dodajemo na br sekundi kako bi datum bio tacan

	time_t newTime = mktime(brokenTime); //br sekundi od brokentimea
	printf("%s", ctime(&newTime)); //pakovanje br sekundi u citljivi string

	// strftime - formatiranje vremena i datuma
	char timeString[1024];
	strftime(timeString, 1024, "Trenutno vreme: %H:%M:%S", brokenTime);
	printf("%s\n", timeString);

	// preciznije racunanje vremena
	// get time of day (radi u mikrosekundama)
	struct timeval preciseTime;
	check_error(gettimeofday(&preciseTime, NULL) != -1, "gettimeofday");

	if(now == preciseTime.tv_sec){
		printf("Jednako vreme\n");
	}else{
		printf("Nije jednako\n");
	}

	printf("Mikrosekunde: %d\n", (int)preciseTime.tv_usec);
	
	exit(EXIT_SUCCESS);
}
