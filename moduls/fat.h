#ifndef _FAT_
#define _FAT_

#include "utils.h"
#include "sistema.h"

int FAT_checkIfFat16(BootSector *bs);
void FAT_printBootSector(BootSector *bs);

#endif