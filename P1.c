#include "moduls/sistema.h"
#include "moduls/ext.h"
#include "moduls/fat.h"


int main(int argc, char*argv[]){
    int bytes;
    int systemType=0;
    int operationType=0;

    if(argc<2){
        write(1, ERR_ARGUMENTS, strlen(ERR_ARGUMENTS));
    }else{
        if( (operationType = SYS_checkOperation(argv[1])) == 0){
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

        int fdFitxer = open(SYS_fileInDirectory("fitxers", argv[2]), O_RDONLY);
        if(fdFitxer<0){
            SYS_fileNotFound(fdFitxer,operationType);
            return 0;
        }

        write(1, TITLE, strlen(TITLE));
        
        SB *superblock;
        BootSector *bs;

        systemType = SYS_getSystemType(fdFitxer, &superblock, &bs);

    
        if(operationType == 1){
            switch(systemType){
                case 1:
                    printf("\nFilesystem: EXT2\n");
                    EXT_printSuperblock(superblock);
                    break;
                case 2:
                    printf("\nFilesystem: FAT16\n");
                    FAT_printBootSector(bs);                    
                    break;
                default:
                    write(1, ERR_NO_ES_FORMAT_VALID, strlen(ERR_NO_ES_FORMAT_VALID));
                    break;

            }
        }else if(operationType == 2){
            if(systemType!=2 && systemType!=1){
                write(1, ERR_NO_ES_FORMAT_VALID_FIND, strlen(ERR_NO_ES_FORMAT_VALID_FIND));
            }else{
                printf("Estem en mode /find\n");

                //argv[3]
            }
            
        }
       
        close(fdFitxer);
        if(bs!=NULL){
            free(bs);
        }
        free(superblock);

       
    }
}