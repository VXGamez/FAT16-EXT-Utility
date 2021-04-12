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


typedef struct {
  uint32_t s_inodes_count;      // Total number of inodes
  uint32_t s_blocks_count;      // Total number of blocks
  uint32_t s_r_blocks_count;    // Number of reserved blocks
  uint32_t s_free_blocks_count; // Number of free blocks (including reserved)
  uint32_t s_free_inodes_count; // Number of free inodes
  uint32_t s_first_data_block;  // First data block (block containing superblock)
  uint32_t s_log_block_size;    // Block size = 1024 << s_log_block_size
  int32_t  s_log_frag_size;     // Fragment size = 1024 << s_log_frag_size
  uint32_t s_blocks_per_group;  // Total blocks per block group
  uint32_t s_frags_per_group;   // Fragments per block group
  uint32_t s_inodes_per_group;  // Total inodes per block group
  uint32_t s_mtime;             // Last mount time
  uint32_t s_wtime;             // Last write-access time
  uint16_t s_mnt_count;         // Mount count since last verified
  uint16_t s_max_mnt_count;     // Maximum mounts before a full check
  uint16_t s_magic;             // Must be equal to EXT2_SUPER_MAGIC
  uint16_t s_state;             // FS was unmounted cleanly (see above)
  uint16_t s_errors;            // Action if errors detected (see above)
  uint16_t s_minor_rev_level;   // Minor version level
  uint32_t s_lastcheck;         // Last file system check time
  uint32_t s_checkinterval;     // Maximum time between file system checks
  uint32_t s_creator_os;        // OS that created file system (see above)
  uint32_t s_rev_level;         // Major version level
  uint16_t s_def_resuid;        // User ID for reserved blocks (eg root)
  uint16_t s_def_resgid;        // Group ID for reserved blocks (eg root)

  uint32_t s_first_ino;         // First inode for standard files
  uint16_t s_inode_size;        // Inode size
  uint16_t s_block_group_nr;    // Block group number (for backups in groups)
  uint32_t s_feature_compat;    // Compatible features (may be ignored)
  uint32_t s_feature_incompat;  // Required features for mounting
  uint32_t s_feature_ro_compat; // Required features for writing
  uint8_t  s_uuid[16];          // UUID (unique ID)
  char     s_volume_name[16];   // Volume name
  unsigned char     s_last_mounted[64];  // Path where FS was last mounted
  uint32_t s_algo_bitmap;       // Compression algorithm support

} SB;

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


int checkIfFat16(BootSector *bs){
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


char* cleanLabel(char cadena[11]){
    char *token;
    char s[2] = " ";
    token = strtok(cadena, s);
   return token;
}


int main(int argc, char*argv[]){
    int bytes;
    int systemType=0;
    if(argc<3){
        write(1, ERR_ARGUMENTS, strlen(ERR_ARGUMENTS));
    }else{
        if(strcmp(argv[1], "/intro") != 0){
            write(1, ERR_OP_NO_PERMESA, strlen(ERR_OP_NO_PERMESA));
            return 0;
        }
        int fdFitxer = open(argv[2], O_RDONLY);
        if(fdFitxer<0){
            write(1, ERR_NO_EXISTEIX, strlen(ERR_NO_EXISTEIX));
            close(fdFitxer);
            return 0;
        }

        write(1, TITLE, strlen(TITLE));
        
        SB *superblock;
        BootSector *bs;

        lseek(fdFitxer, SUPERBLOCK_START, SEEK_SET);
        superblock = malloc(sizeof(SB));
        bytes = read(fdFitxer, superblock, sizeof(SB));

        if(superblock->s_magic == 0xEF53){
            systemType = 1;
        }else{
            lseek(fdFitxer, 0, SEEK_SET);
            bs = malloc(sizeof(BootSector));
            bytes = read(fdFitxer, bs, sizeof(BootSector));

            if(checkIfFat16(bs)){
                systemType=2;
            }
        }  


        switch(systemType){
            case 1:
                printf("\nFilesystem: EXT2\n");

                write(1, INFO_INODE, strlen(INFO_INODE));
                /*
                INFO INODE
                Mida Inode: 128 
                Num Inodes: 64 
                Primer Inode: 11 
                Inodes Grup: 64 
                Inodes Lliures: 38
                */
                printf("Mida Inode: %u\n", superblock->s_inode_size);
                printf("Num Inodes: %u\n", superblock->s_inodes_count);
                printf("Primer Inode: %u\n", superblock->s_first_ino);
                printf("Inodes Grup: %u\n", superblock->s_inodes_per_group);
                printf("Inodes Lliures: %u\n", superblock->s_free_inodes_count);

                write(1, INFO_BLOC, strlen(INFO_BLOC));
                /*
                INFO BLOC
                Mida Bloc: 1024 
                Blocs Reservats: 25 
                Blocs Lliures: 471 
                Total Blocs: 512 
                Primer Bloc: 1 
                Blocs grup: 8192 
                Frags grup: 8192
                */

                printf("Mida Bloc: %u\n", 1024 << superblock->s_log_block_size);                   
                printf("Blocs Reservats: %u\n", superblock->s_r_blocks_count);             
                printf("Blocs Lliures: %u\n", superblock->s_free_blocks_count);            
                printf("Total Blocs: %u\n", superblock->s_blocks_count);                   
                printf("Primer Bloc: %u\n", superblock->s_first_data_block);
                printf("Blocs grup: %u\n", superblock->s_blocks_per_group);                       
                printf("Frags grup: %u\n", superblock->s_frags_per_group);                        

                write(1, INFO_VOLUM, strlen(INFO_VOLUM));
                /*
                INFO VOLUM
                Nom volum: ext2fs_prova1
                Ultima comprov: Tue Apr 24 18:41:24 2007 
                Ultim muntatge: Tue Apr 24 19:33:11 2007 
                Ultima escriptura: Tue Apr 24 19:33:16 2007
                */

                time_t muntatge, escriptura, comprov;
                char* muntatge_string, *escriptura_string, *comprov_string;
                
                muntatge = (time_t)superblock->s_mtime;
                escriptura = (time_t)superblock->s_wtime;
                comprov = (time_t)superblock->s_lastcheck;
                muntatge_string = ctime(&muntatge);
                escriptura_string = ctime(&escriptura);
                comprov_string = ctime(&comprov);
                if(strlen(superblock->s_volume_name)==0){
                    printf("Nom volum: <EMPTY>\n");       
                }else{
                    printf("Nom volum: %s\n",superblock->s_volume_name);       
                }
                printf("Ultima comprov: %s",comprov_string);               
                printf("Ultim muntatge: %s",muntatge_string);              
                printf("Ultima escriptura: %s\n\n",escriptura_string);       

                break;
            case 2:
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
                printf("\nFilesystem: FAT16\n");
                printf("System Name: %s\n", bs->BS_OEMName);
                printf("Mida del sector: %hu\n", bs->BPB_BytsPerSec);
                printf("Sectors Per Cluster: %u\n", bs->BPB_SecPerClus);
                printf("Sectors reservats: %hu\n", bs->BPB_RsvdSecCnt);
                printf("Número de FATs: %u\n", bs->BPB_NumFATs);
                printf("MaxRootEntries: %hu\n", bs->BPB_RootEntCnt);
                printf("Sectors per FAT: %hu\n", bs->BPB_FATSz16);
                char* token = cleanLabel(bs->BS_VolLab);
                printf("Label: %s\n\n\n", token);
                
                break;

            default:
                write(1, ERR_NO_ES_FORMAT_VALID, strlen(ERR_NO_ES_FORMAT_VALID));
                break;

        }
        

       
        close(fdFitxer);
        if(bs!=NULL){
            free(bs);
        }
        free(superblock);

       
    }
}