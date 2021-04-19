#ifndef _EXT_
#define _EXT_

#include "utils.h"

void EXT_printSuperblock(SB* superblock);
int EXT_findFile(char* fitxer, int fdVolum, SB* sb);

#endif