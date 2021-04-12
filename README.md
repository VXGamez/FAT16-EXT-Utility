# SOA

<p align="center">
  <a href="http://linkedin.com/">
  <img src="https://img.shields.io/badge/FASE-1-brightgreen?style=for-the-badge&logo=c">
  </a>
</p>
  
<details open="open">
  <summary><h2 style="display: inline-block">Taula de Continguts</h2></summary>
  <ol>
    <li><a href="#com-compilar">Com Compilar</a></li>
    <li><a href="#explicació-dels-sistemes-de-fitxers">Explicació dels Sistemes de Fitxers</a></li>
    <li>
      <a href="#explicació-de-la-pràctica">Explicació de la pràctica</a>
      <ul>
        <li><a href="#requeriments">Requeriments</a></li>
        <li><a href="#disseny">Disseny</a></li>
        <li><a href="#estructures-de-dades-usades">Estructures de dades Usades</a></li>
        <li><a href="#proves-realitzades">Proves realitzades</a></li>
        <li><a href="#problemes-observats">Problemes observats</a></li>
        <li><a href="#estimació-temporal">Estimació Temporal</a></li>
      </ul>
    </li>
    <li><a href="#explicació-i-valoració-del-git">Explicació i valoració del GIT</a></li>
    <li><a href="#conclusions-generals">Conclusions generals</a></li>
  </ol>
</details>

## Com Compilar
1. Ara per ara, només caldrà netejar anteriors execucions:
   ```sh
   make clean
   ```
2. Compilar novament emprant la comanda make:
   ```sh
   make
   ```
3. Per executar emprar el executable p1 de la següent manera:
   ```sh
   ./p1 <operacio> <nom_volum>
   ```

## Explicació dels Sistemes de Fitxers


## Explicació de la pràctica

### Requeriments

### Disseny

### Estructures de dades Usades
   ```c
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
   ```
### Proves realitzades

### Problemes observats

### Estimació Temporal

## Explicació i valoració del GIT

## Conclusions generals


