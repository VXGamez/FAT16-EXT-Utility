#define _GNU_SOURCE
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <termios.h>
#include <dirent.h>
#include <poll.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sys/types.h>
#include <time.h>


#define ERR_ARGUMENTS "No hi ha suficients arguments\n"
#define ERR_OP_NO_PERMESA "Operació no permesa\n"
#define ERR_NO_EXISTEIX "Fitxer no existeix\n"
#define TITLE "------ Filesystem Information ------\n\n"

int main(int argc, char*argv[]){
    if(argc<3){
        write(1, ERR_ARGUMENTS, strlen(ERR_ARGUMENTS));
    }else{
        if(strcmp(argv[1], "/intro") != 0){
            write(1, ERR_OP_NO_PERMESA, strlen(ERR_OP_NO_PERMESA));
            return 0;
        }
        int fdFitxer = open(argv[2], O_RDONLY);
        if(fdFitxer<0){
            write(1, ERR_NO_EXISTEIX, strlen(ERR_NO_EXISTEIX));
            close(fdFitxer);
            return 0;
        }
        


        close(fdFitxer);
    }
}