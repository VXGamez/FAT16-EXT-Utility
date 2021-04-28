#ifndef _SISTEMA_
#define _SISTEMA_

#include "utils.h"
#include "fat.h"

char* SYS_cleanLabel(char cadena[11]);
char* SYS_fileInDirectory(char* directory, char* fileName);
int SYS_checkOperation(char* operation);
int SYS_getSystemType(int fdFitxer, SB **superblock, BootSector **bs);
void SYS_fileNotFound(int fdFile, int opType);
void SYS_removeExtension(char* file);
void SYS_clearFATvalue(char *name, int limit);
void SYS_toLower(char* str);
int SYS_checkExtension(char* extensions);
char* SYS_getExtension(char* file);

#endif
