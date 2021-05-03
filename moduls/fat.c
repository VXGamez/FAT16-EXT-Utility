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

int FAT_findFile(char* fitxer, int fdFitxer, BootSector *bs, char* ext, int N){

    long ll = (bs->BPB_RsvdSecCnt+(bs->BPB_NumFATs*bs->BPB_FATSz16))*bs->BPB_BytsPerSec;
    //lseek(fdFitxer, (bs->BPB_RsvdSecCnt+(bs->BPB_NumFATs*bs->BPB_FATSz16))*bs->BPB_BytsPerSec ,SEEK_SET);

    //2A000
    //172032

    //i686 == La 168 == 2A000
    //hdparm == 158 ==


    uint32_t rootDir = bs->BPB_RootEntCnt * 32;

    uint32_t firstDataSector =  (bs->BPB_RsvdSecCnt+(bs->BPB_NumFATs*bs->BPB_FATSz16))* bs->BPB_BytsPerSec ;

    uint32_t final = ((N-2) * bs->BPB_SecPerClus * bs->BPB_BytsPerSec) + firstDataSector ;
    if(N!=2){
        final = final + rootDir;
    }

    printf("Final: %d - %x\n", final, final);
    printf("Offset Que tenia: %ld - %lx\n", ll, ll);

    lseek(fdFitxer, final ,SEEK_SET);

    int bytes=-1;

    dir_entry de;
    int isDone = 0;
    for(int i=0; !isDone ;i++){
        pread(fdFitxer, &de, sizeof(dir_entry), final+i*sizeof(dir_entry));
        if(de.long_name[0]==0x00){
            isDone = 1;
        }else{
            SYS_clearFATvalue(de.long_name, 8);
            SYS_clearFATvalue(de.extension, 3);
            printf("Long Name: %s\n",de.long_name );

            if(de.fileAttr == 16 && strcmp(de.long_name, ".") !=0 && strcmp(de.long_name, "..") !=0){
                printf("Es directori\n");
                printf("First Cluster: %d\n",de.firstCluster );
                printf("Entro a la carpeta %s\n", de.long_name);
                int byt = FAT_findFile(fitxer, fdFitxer, bs, ext, de.firstCluster);
                printf("Torno a la carpeta anterior\n");
                printf("Bytes: %d\n", byt);
                if(byt>0){
                    isDone = 1;
                }
            }else{
                if(strlen(de.long_name)>0 && de.fileAttr>0 && ((int)de.fSize)>0 ){
                    if(strcmp(de.long_name, fitxer)==0 && strcmp(de.extension, ext)==0){
                        bytes = de.fSize;
                    }
                }
            }


        }

    }


    return bytes;
}
