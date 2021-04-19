#include "ext.h"



void EXT_printSuperblock(SB* superblock){
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
}


int EXT_findFile(char* fitxer, int fdVolum, SB* sb){
    //inode ino = EXT_readIno(fdVolum, superblock->s_first_ino, superblock->s_first_ino);
    //lseek(fdVolum, SUPERBLOCK_START, SEEK_SET);
    lseek(fdVolum, SUPERBLOCK_START + 1024 + 3*(sb->s_log_block_size), SEEK_SET);
    lseek(fdVolum, sb->s_first_ino*sb->s_inode_size, SEEK_CUR);
    inode ino;
    read(fdVolum, &ino, sizeof(inode));
    lseek(fdVolum, SUPERBLOCK_START + 1024 + 3*(sb->s_log_block_size), SEEK_SET);
    lseek(fdVolum, 214*(sb->s_log_block_size),SEEK_CUR);
    printf("Block size: %d\n", ino.i_blocks);
    char kk;
    printf("Enter per continuar: ");
    scanf("%c", &kk);
    for(int i=0; i<12 ;i++){
        printf("Anant al block: %d...\n", ino.i_block[i]);
        lseek(fdVolum, ino.i_block[i]*1024, SEEK_CUR);
        ino_block* blockActual = malloc(sizeof(ino_block));;
        read(fdVolum, blockActual, sizeof(ino_block));
        char*name = malloc(blockActual->name_len+1);
        read(fdVolum, name, blockActual->name_len);
        name[blockActual->name_len] = '\0';
        printf("Inode: %d\n", blockActual->inode);
        printf("Rec_Len: %d\n", blockActual->rec_len);
        printf("Name_len: %d\n", blockActual->name_len);
        printf("file_type: %hhu\n", blockActual->file_type);
        printf("Name del fitxer: %s\n", name);
        free(name);
        free(blockActual);
        
        lseek(fdVolum, SUPERBLOCK_START + 1024 + 3*(sb->s_log_block_size), SEEK_SET);
        lseek(fdVolum, 214*(sb->s_log_block_size),SEEK_CUR);
    }
    
    /*for(int i=0; i<ino.i_blocks ;i++){
        ino_block blockActual;
        read(fdVolum, &blockActual, sizeof(ino_block));
        char*name = malloc(blockActual.name_len+1);
        read(fdVolum, name, blockActual.name_len);
        name[blockActual.name_len] = '\0';
        printf("Name del fitxer: %s\n", name);
        free(name);
        lseek(fdVolum, 512-(sizeof(ino_block)+blockActual.name_len), SEEK_CUR);
    }*/

    return 0;
}
