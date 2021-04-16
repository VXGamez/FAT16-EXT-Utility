#ifndef _FAT_
#define _FAT_

#include "utils.h"

typedef struct {
    unsigned char BS_jmpBoot[3];      //Code to jump to the boostrap code
    char BS_OEMName[8];               //OEM ID - Name of th eformatting OS
    unsigned short BPB_BytsPerSec;    //Bytes per Sector
    unsigned char BPB_SecPerClus;     //Sectors per Cluster
    unsigned short BPB_RsvdSecCnt;    //Reserved Sectors
    unsigned char BPB_NumFATs;        //Numer of FAT copies
    unsigned short BPB_RootEntCnt;    //Number of possible root entries
    unsigned short BPB_TotSec16;      //Small number of sectors
    unsigned char BPB_Media;          //Media Descriptor
    unsigned short BPB_FATSz16;       //Sectors per FAT
    unsigned short BPB_SecPerTrk;     //Sectors per Track
    unsigned short BPB_NumHeads;      //Number of Heads
    int BPB_HiddSec;                  //Hidden sectors
    int BPB_TotSec32;                 //Large number of sectors
    unsigned char BS_DrvNum;          //Drive Number
    unsigned char BS_Reserved1;       //Reserved
    unsigned char BS_BootSig;         //Extended Boot Signatura
    int BS_VolID;                     //Volume Serial Number
    char BS_VolLab[11];               //Volume Label
    char BS_FilSysType[8];            //File System Type
} BootSector;


int FAT_checkIfFat16(BootSector *bs);


#endif