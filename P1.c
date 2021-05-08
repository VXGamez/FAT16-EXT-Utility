#include "moduls/sistema.h"
#include "moduls/ext.h"
#include "moduls/fat.h"


int fdFitxer=-1;
SB *superblock=NULL;
BootSector *bs=NULL;

void freeMem(){
  if(fdFitxer>0){
      close(fdFitxer);
  }
  if(bs!=NULL){
      free(bs);
  }
  if(superblock!=NULL){
      free(superblock);
  }
}

int main(int argc, char*argv[]){
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

        fdFitxer = open(SYS_fileInDirectory("fitxers", argv[2]), O_RDWR);
        if(fdFitxer<0){
            SYS_fileNotFound(fdFitxer,operationType);
            freeMem();
            return 0;
        }


        systemType = SYS_getSystemType(fdFitxer, &superblock, &bs);

        if(operationType == 1){
            write(1, TITLE, strlen(TITLE));
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
        }else if(operationType == 2 ||operationType == 3){
            int bytes;

            if(systemType!=2 && systemType!=1){
                write(1, ERR_NO_ES_FORMAT_VALID_FIND, strlen(ERR_NO_ES_FORMAT_VALID_FIND));
            }else{
                char* extension = SYS_getExtension(argv[3]);
                if(strlen(extension)>0 &&  SYS_checkExtension(extension)==0){
                    printf("Error. Extensió no vàlida.\n");
                    freeMem();
                    return 0;
                }
                SYS_removeExtension(argv[3]);
                switch(systemType){
                    case 1:
                        bytes = EXT_findFile(argv[3], fdFitxer, superblock, 2, operationType-2);
                        if(bytes < 0){
                            if(bytes == -2){
                                printf("\nError. Fitxer no trobat però sí un directori pel mateix nom.\n\n");
                            }else{
                                printf("\nError. Fitxer no trobat.\n\n");
                            }
                        }else{
                            if(operationType == 2){
                                printf("\nFitxer trobat. Ocupa %d bytes.\n\n" ,bytes);
                            }else if(operationType == 3) {
                                printf("\nEl fitxer %s ha estat eliminat.\n\n", argv[3]);
                            }
                        }

                        break;
                    case 2:
                        bytes = FAT_findFile(argv[3], fdFitxer, bs, extension, 2);
                        if(bytes < 0){
                            printf("\nError. Fitxer no trobat.\n\n");
                        }else{
                            printf("\nFitxer trobat. Ocupa %d bytes.\n\n" ,bytes);
                        }
                        break;
                }
            }

        }

        freeMem();

    }
}
