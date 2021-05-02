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


int EXT_findFile(char* fitxer, int fdVolum, SB* sb, int inici){

    inodo ino = EXT_trobaInode(fdVolum, sb, inici);
    int byt=-1;
    uint16_t prevLen = 0;
    int block_size = 1024 << sb->s_log_block_size;
    int totalLen=0;
    int inner= 0;
    ino_block blockActual;
    for(int j=0; totalLen<ino.i_size ;j++){
        inner = 0;
        prevLen = 0;
        lseek(fdVolum, block_size + block_size*(ino.i_block[j]-1)+totalLen, SEEK_SET);
        while(!inner){
            pread(fdVolum, &blockActual, sizeof(ino_block), block_size + block_size*(ino.i_block[j]-1)+totalLen);
            char*name=NULL;
            if(blockActual.name_len>0){
                name = malloc(blockActual.name_len+1);
                pread(fdVolum, name, blockActual.name_len, block_size + block_size*(ino.i_block[j]-1)+totalLen+sizeof(ino_block));
                name[blockActual.name_len] = '\0';
                prevLen = blockActual.rec_len;
                totalLen = totalLen + prevLen;
                if(prevLen == 0){
                    inner = 1;
                }else{
                    if(strcmp(fitxer, name)==0 && blockActual.file_type == 1){
                        inner = 1;
                        totalLen = ino.i_size + 1000;
                        inodo ff = EXT_trobaInode(fdVolum, sb, blockActual.inode);
                        byt = ff.i_size;
                    }else if(blockActual.file_type == 2 && strcmp(name, ".")!=0 && strcmp(name, "..")!=0 && strcmp(name, "lost+found")!=0){
                        byt = EXT_findFile(fitxer, fdVolum, sb, blockActual.inode);
                        if(byt>0){
                            inner = 1;
                            totalLen = ino.i_size + 1000;
                        }
                    }
                }
            }else{
                inner=1;
            }
            if(name!=NULL){
                free(name);
            }
        }
    }

    return byt;
}



inodo EXT_trobaInode(int fdVolum, SB* sb, int inode){

    GroupDescriptor groupbuffer;

    int block_size = 1024 << sb->s_log_block_size;
    lseek(fdVolum, (sb->s_first_data_block+1)*block_size, SEEK_SET);
    read(fdVolum, &groupbuffer, sizeof(GroupDescriptor));
    int block_group = (inode - 1) / sb->s_inodes_per_group;
    int index = (inode - 1) % sb->s_inodes_per_group;
    int offset = index*sb->s_inode_size;
    int group_table =  groupbuffer.bg_inode_table + block_group * sb->s_blocks_per_group;
    inodo ino;
    pread(fdVolum, &ino, sizeof(inodo), (group_table*block_size)+offset);
    return ino;

}
