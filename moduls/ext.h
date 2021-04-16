#ifndef _EXT_
#define _EXT_

#include "utils.h"

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


#endif