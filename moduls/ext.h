#ifndef _EXT_
#define _EXT_

#include "utils.h"
#include "sistema.h"

void EXT_printSuperblock(SB* superblock);
int EXT_findFile(char* fitxer, int fdVolum, SB* sb, int inici, int flag);
inodo EXT_trobaInode(int fdVolum, SB* sb, int inode);
void EXT_deleteFile(int fdVolum, SB* sb, int inode, inodo ff);

#endif
