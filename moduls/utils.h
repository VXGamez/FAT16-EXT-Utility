#ifndef _SYS_
#define _SYS_

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
#include <inttypes.h>


#define ERR_ARGUMENTS "El número d'arguments és incorrecte\n"
#define ERR_OP_NO_PERMESA "Operació no permesa\n"
#define ERR_NO_EXISTEIX "Fitxer no existeix\n"
#define ERR_NO_FIND_EXISTEIX "\nError. Volum no trobat.\n\n"
#define ERR_NO_ES_FORMAT_VALID_FIND "Error. Volum no formatat en FAT16 ni EXT2\n\n"
#define ERR_NO_ES_FORMAT_VALID "Sistema d’arxius no és ni EXT2 ni FAT16.\n\n"
#define TITLE "\n\n------ Filesystem Information ------\n"
#define INFO_INODE "\nINFO INODE\n"
#define INFO_BLOC "\nINFO BLOC\n"
#define INFO_VOLUM "\nINFO VOLUM\n"
#define SUPERBLOCK_START 1024
#define GROUP_DESCRIPTORS_SIZE 32
#define ACCEPTED_OPERATIONS 2
#define ACCEPTED_EXTENSIONS 10

static const char acceptedOperations[2][20] = {"/info", "/find"};
static const char acceptedExtensions[10][20] = {"dat", "bin", "h", "c", "txt", "jpg", "png", "zip", "hex", "so.0.9.8"};


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

}SB;

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
    //int BS_VolID;                   //Volume Serial Number
    char BS_VolLab[11];               //Volume Label
    char BS_FilSysType[8];            //File System Type
}BootSector;

typedef struct {
    char long_name[8];                    //The file's name, truncated to 31 characters.
    char extension[3];                    //The file's attributes. Mask of the FAT16_ATTRIB_* constants.
    uint8_t fileAttr;
    uint8_t reserved[10];
    uint16_t tChange;
    uint16_t dChange;
    uint16_t firstCluster;
    uint32_t fSize;
}dir_entry;

typedef struct{
    uint32_t bg_block_bitmap;
    uint32_t bg_inode_bitmap;
    uint32_t bg_inode_table;
    uint16_t bg_free_blocks_count;
    uint16_t bg_free_inodes_count;
    uint16_t bg_used_dirs_count;
    uint16_t bg_pad;
    unsigned char bg_reserved[12];
}GroupDescriptor;

typedef struct {
  uint16_t i_mode;        // Mode (type of file and permissions)
  uint16_t i_uid;         // Owner's user ID
  uint32_t i_size;        // File size (least significant 32 bits)
  uint32_t i_atime;       // Last access time
  uint32_t i_ctime;       // Creation time
  uint32_t i_mtime;       // Last modification time
  uint32_t i_dtime;       // Deletion time
  uint16_t i_gid;         // Owner's group ID
  uint16_t i_links_count; // Reference counter (number of hard links)
  uint32_t i_blocks;      // Number of 512-byte blocks reserved for this inode
  uint32_t i_flags;       // Flags
  uint32_t i_osd1;        // OS-dependant value
  uint32_t i_block[15];   //Bloqueh
  uint32_t i_generation;  // File version (used for NFS)
  uint32_t i_file_acl;    // Block number for extended attributes
  uint32_t i_dir_acl;     // File size (most significant 32 bits)
  uint32_t i_faddr;       // Location of file fragment (deprecated)
  unsigned char i_osd2[12];
} inodo;

typedef struct{
  uint32_t inode;
  uint16_t rec_len;
  uint8_t name_len;
  uint8_t file_type;
}ino_block;

#endif
