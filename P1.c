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
#define TITLE "------ Filesystem Information ------\n\n"
#define INFO_INODE "\n\nINFO INODE\n"
#define INFO_BLOC "\n\nINFO BLOC\n"
#define INFO_VOLUM "\n\nINFO VOLUM\n"
#define SUPERBLOCK_START 1024


typedef struct superblock {
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
  char     s_last_mounted[64];  // Path where FS was last mounted
  uint32_t s_algo_bitmap;       // Compression algorithm support

} superblock_t;


int main(int argc, char*argv[]){
    int bytes;

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
        
        superblock_t *superblock;
        lseek(fdFitxer, SUPERBLOCK_START, SEEK_SET);
        superblock = malloc(sizeof(superblock_t));
        bytes = read(fdFitxer, superblock, sizeof(superblock_t));

        if(superblock->s_magic != 0xEF53){
            write(1, ERR_NO_ES_FORMAT_VALID, strlen(ERR_NO_ES_FORMAT_VALID));
            return 0;
        }
        
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

        printf("Mida Bloc: %u\n", superblock->s_log_block_size);                   
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

        printf("Nom volum: %s\n",superblock->s_volume_name);       
        printf("Ultima comprov: %s",comprov_string);               
        printf("Ultim muntatge: %s",muntatge_string);              
        printf("Ultima escriptura: %s\n",escriptura_string);       

       
        close(fdFitxer);
    }
}