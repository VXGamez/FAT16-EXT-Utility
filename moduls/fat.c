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

    dataSec = -1*(TotSec - (bs->BPB_RsvdSecCnt + (bs->BPB_NumFATs * FATSz) + (((bs->BPB_RootEntCnt * 32) + (bs->BPB_BytsPerSec - 1)) / bs->BPB_BytsPerSec) ));

    countOfClusters = dataSec / bs->BPB_SecPerClus;  
    
    /*
    printf("DATASEC: %d\n", dataSec);
    printf("%hu - (%u + %u * %hu) + ((%hu * 32) + (%hu - 1)) / %hu )\n", TotSec, bs->BPB_RsvdSecCnt, bs->BPB_NumFATs, FATSz, bs->BPB_RootEntCnt, bs->BPB_BytsPerSec, bs->BPB_BytsPerSec);
    
    printf("\nCount of Clusters: %d\n", countOfClusters);
    printf("%d / %u\n\n", dataSec,bs->BPB_SecPerClus);
    */
    if(countOfClusters < 4085) {
       return 0;
    } else if(countOfClusters < 65525) {
        return 1;
    } else {
       return 0;
    }

}
