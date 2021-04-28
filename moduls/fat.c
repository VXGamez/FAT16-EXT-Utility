#include "fat.h"


int FAT_checkIfFat16(BootSector *bs){
    short TotSec, FATSz;
    int dataSec, countOfClusters;

    if(bs->BPB_FATSz16 != 0){
        FATSz = bs->BPB_FATSz16;
    }else{
        return 0;
    }

    if(bs->BPB_TotSec16 != 0){
        TotSec = bs->BPB_TotSec16;
    }else{
        TotSec = bs->BPB_TotSec32;
    }

    dataSec = (TotSec - (bs->BPB_RsvdSecCnt + (bs->BPB_NumFATs * FATSz) + (((bs->BPB_RootEntCnt * 32) + (bs->BPB_BytsPerSec - 1)) / bs->BPB_BytsPerSec) ));
    countOfClusters = dataSec / bs->BPB_SecPerClus;

    if(countOfClusters < 4085) {
       return 0;
    } else if(countOfClusters < 65525) {
        return 1;
    } else {
       return 0;
    }

}


void FAT_printBootSector(BootSector *bs){
    /*
    Filesystem: FAT16
    System Name: SALLEFAT
    Mida del sector: 1024
    Sectors Per Cluster: 1
    Sectors reservats: 1
    Número de FATs: 2
    MaxRootEntries: 100
    Sectors per FAT: 2
    Label: EXEMPLE2
    */
    printf("System Name: %s\n", bs->BS_OEMName);
    printf("Mida del sector: %hu\n", bs->BPB_BytsPerSec);
    printf("Sectors Per Cluster: %u\n", bs->BPB_SecPerClus);
    printf("Sectors reservats: %hu\n", bs->BPB_RsvdSecCnt);
    printf("Número de FATs: %u\n", bs->BPB_NumFATs);
    printf("MaxRootEntries: %hu\n", bs->BPB_RootEntCnt);
    printf("Sectors per FAT: %hu\n", bs->BPB_FATSz16);
    char* token = SYS_cleanLabel(bs->BS_VolLab);
    printf("Label: %s\n\n\n", token);
}

int FAT_findFile(char* fitxer, int fdFitxer, BootSector *bs, char* ext){

    //long ll = (bs->BPB_RsvdSecCnt+(bs->BPB_NumFATs*bs->BPB_FATSz16))*bs->BPB_BytsPerSec;
    lseek(fdFitxer, (bs->BPB_RsvdSecCnt+(bs->BPB_NumFATs*bs->BPB_FATSz16))*bs->BPB_BytsPerSec ,SEEK_SET);

    int bytes=-1;

    dir_entry de;
    for(int i=0; i<(bs->BPB_RootEntCnt*32)/(bs->BPB_BytsPerSec) ;i++){
        read(fdFitxer, &de, sizeof(dir_entry));
        SYS_clearFATvalue(de.long_name, 8);
        SYS_clearFATvalue(de.extension, 3);
        if(strlen(de.long_name)>0 && de.fileAttr>0 && ((int)de.fSize)>0 ){
            if(strcmp(de.long_name, fitxer)==0 && strcmp(de.extension, ext)==0){
                bytes = de.fSize;
            }
        }
    }


    return bytes;
}
