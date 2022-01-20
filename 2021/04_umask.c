#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>

#define check_error(expr,userMsg)\
	do {\
		if (!(expr)) { \
			perror(userMsg); \
			exit(EXIT_FAILURE);\
		}\
	} while (0)
	
int main(int argc, char** argv) {
	
	// kreiramo fajl sa pravima pristupa koja zeli korisnik
	// ./umask imefajla pravapristupa
	// umask su bitovi koje sistem gasi prilikom kreiranja fajlova
	// gasi - prava koja ne zeli da da
	// ako fajl postoji - umask nema smisla
	// umask se koristi samo prilikom KREIRANJA fajla
	// zelja - 0777 (111 111 111)
	// umask - 0002 (000 000 010)
	// kako radi umask?
	// ~umask = 111 111 101
	// zelja & (~umask) = 111 111 101 = 0775
	// mi cemo probati da zaobidjemo to
	// menjamo umask pre kreiranja i vracamo na staro posle

	check_error(argc == 3, "args");

	int pravapristupa = strtol(argv[2], NULL, 8);

	mode_t old_umask = umask(0); //umask nikad ne puca, ne proveravamo
						// prima novi umask kao arg
						// vraca prethodnu vrednost kao rezultat
	int fd = open(argv[1], O_CREAT | O_EXCL, pravapristupa);
									// prijavljuje gresku ako fajl postoji
	check_error(fd != -1, "open");

	close(fd);
	umask(old_umask); // vracamo na staro

	exit(EXIT_SUCCESS);
}
