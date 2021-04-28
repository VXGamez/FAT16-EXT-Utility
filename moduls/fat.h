#ifndef _FAT_
#define _FAT_

#include "utils.h"
#include "sistema.h"

int FAT_checkIfFat16(BootSector *bs);
void FAT_printBootSector(BootSector *bs);
int FAT_findFile(char* fitxer, int fdFitxer, BootSector *bs, char* ext);

#endif
