#ifndef _UTILS_
#define _UTILS_

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
#define ERR_NO_ES_FORMAT_VALID "Sistema d’arxius no és ni EXT2 ni FAT16.\n\n"
#define TITLE "\n\n------ Filesystem Information ------\n"
#define INFO_INODE "\nINFO INODE\n"
#define INFO_BLOC "\nINFO BLOC\n"
#define INFO_VOLUM "\nINFO VOLUM\n"
#define SUPERBLOCK_START 1024

char* UTILS_cleanLabel(char cadena[11]);
char* UTILS_fileInDirectory(char* directory, char* fileName);

#endif