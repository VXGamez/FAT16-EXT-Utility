#include "moduls/utils.h"
#include "moduls/fat.h"
#include "moduls/ext.h"


int main(int argc, char*argv[]){
    int bytes;
    int systemType=0;
    int operationType=0;

    if(argc<2){
        write(1, ERR_ARGUMENTS, strlen(ERR_ARGUMENTS));
    }else{
        if( (operationType = UTILS_checkOperation(argv[1])) == 0){
            write(1, ERR_OP_NO_PERMESA, strlen(ERR_OP_NO_PERMESA));
            return 0;
        }else{
            if(operationType == 1 && argc !=3){
                write(1, ERR_ARGUMENTS, strlen(ERR_ARGUMENTS));
                return 0;
            }else if(operationType == 2 && argc !=4){
                write(1, ERR_ARGUMENTS, strlen(ERR_ARGUMENTS));
                return 0;
            }
        }

        int fdFitxer = open(UTILS_fileInDirectory("fitxers", argv[2]), O_RDONLY);
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

            if(FAT_checkIfFat16(bs)){
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
                char* token = UTILS_cleanLabel(bs->BS_VolLab);
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