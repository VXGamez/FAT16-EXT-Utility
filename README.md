# SOA

<p align="center">
  <a href="https://github.com/VXGamez/SOA/releases/tag/FASE1">
  <img src="https://img.shields.io/badge/FASE-1-brightgreen?style=for-the-badge&logo=c">
  </a>
</p>
  
<details open="open">
  <summary><h2 style="display: inline-block">Taula de Continguts</h2></summary>
  <ol>
    <li><a href="#com-compilar">Com Compilar</a></li>
    <li><a href="#explicació-dels-sistemes-de-fitxers">Explicació dels Sistemes de Fitxers</a>
      <ul>
        <li><a href="#fat12">FAT12</a></li>
        <li><a href="#ext">EXT</a></li>
      </ul>
    </li>
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

De cara a aquesta entrega se'ns ha demanat que explorem l'organització dels fitxers de dos sistemes de fitxers diferents: FAT12 i EXT. Per assolir aquesta exploració se'ns han proveït dos fitxers, un EXT i l'altre FAT12, així com una explicació detallada de com fer-ne més tot i que jo no n'he realitzat cap per aquesta entrega.

### FAT12

La taula d'assignació de fitxers (File Allocation Table - FAT) és una taula emmagatzemada en un disc dur o disquet que indica el fitxer estat i ubicació de tots els clústers de dades que hi ha al disc. La taula d'assignació de fitxers pot ser es considera la "taula de continguts" d'un disc. Si la taula d'assignació de fitxers es fa malbé o es perd, llavors a el disc no es pot llegir. En el cas de FAT12, és el sistema de fitxers d’un disquet. El número "12" es deriva del fet que el FAT consta d'entrades de 12 bits.

L’espai d’emmagatzematge d’un disquet es divideix en unitats anomenades sectors. En dispositius d'emmagatzematge més grans, a un conjunt de sectors formen un clúster. No obstant això, per al disquet, el nombre de sectors en un clúster és un. A més, la mida d'un sector (i, per tant, d'un clúster) és de 512 bytes per a un disquet.

El disseny d'un disquet FAT-12 consta de quatre seccions principals: el Boot Sector, les taules FAT, el directori root i l'àrea de dades.

El Boot Sector, o sector d'arrencada, és en el primer sector (sector 0) del volum o del disc. Conté informació específica sobre la resta d'organitzacions del sistema de fitxers, per exemple quantes còpies de taules FAT hi ha, com de gran és un sector, quants sectors hi ha en un clúster, etc.

Les taules FAT contenen punters a tots els clústers del disc e indiquen el número del següent clúster de la cadena de clústers actual així com el final de la cadena de clúster, si un clúster està buit o si aquest té errors. Les taules FAT són l'únic mètode per trobar la ubicació de fitxers i directoris a la resta del disc. Normalment, hi ha dues còpies redundants de la taula FAT al disc per finalitats de seguretat i recuperació de dades.

El directori root és el directori principal del disc. A diferència d'altres directoris situats a l'àrea de dades del disc, el directori root té una mida finita (per a FAT12, 14 sectors * 16 entrades de directori per sector = 224 entrades possibles), restringint la quantitat total de fitxers o directoris que es poden crear-hi.

A l'àrea de dades, el primer sector o clúster de l'àrea de dades correspon al clúster 2 del fitxer sistema. L'àrea de dades conté dades de fitxers i directoris i abasta la resta de sectors del disc.

### EXT

Els sistemes de fitxers ampliats (Extended Filesystems - EXT) es troben a moltes distribucions de Linux que van des d’Ubuntu fins a Mac OS fins a Android. A més, aquests sistemes de fitxers contenen inodes de tal manera que tots els fitxers o directoris estan representats per un inode. Cada inode conté informació sobre el tipus de fitxer, els drets d’accés, els propietaris, les marques de temps, la mida i els indicadors de blocs de dades.

Un sistema de fitxers EXT es divideix en una sèrie de grups de blocs. Per reduir les dificultats de rendiment a causa de la fragmentació, l'assignació de blocs s'esforça molt per mantenir els blocs de cada fitxer dins del mateix grup, reduint així els temps de cerca. La mida d’un grup de blocs s’especifica als blocs superblock.s_blocks_per_group, tot i que també es pot calcular com a 8 * block_size_in_bytes. Amb la mida de bloc predeterminada de 4KiB, cada grup contindrà 32.768 blocs, per a una longitud de 128 MB. El nombre de grups de blocs és la mida del dispositiu dividida per la mida d’un grup de blocs.

Per al cas especial del grup de blocs 0 (superblock) , els primers 1024 bytes no s’utilitzen d'aquesta manera el superblock començarà amb un desplaçament de 1024 bytes, sigui el bloc que sigui (normalment 0). 

El controlador ext4 funciona principalment amb el superblock i els descriptors de grups que es troben al grup de blocs 0. La ubicació de la taula d’inodes ve donada per bg_inode_table. És un rang continu de blocs prou gran com per contenir s_inodes_per_group * sb.s_inode_size bytes.

Pel que fa a l’ordenació d’elements d’un grup de blocs, generalment s’estableix que el superbloc i la taula de descriptors de grups, si hi ha, estaran al principi del grup de blocs. Els mapes de bits i la taula d’inodes poden estar a qualsevol lloc i és molt possible que els mapes de bits arribin després de la taula d’inodes o que tots dos estiguin en grups diferents (flex_bg). L’espai sobrant s’utilitza per a blocs de dades de fitxers, mapes de blocs indirectes, blocs d’arbre d’extensió i atributs ampliats.

## Explicació de la pràctica

### Requeriments

### Disseny

Pel que fa al disseny d'aquesta primera fase ha estat molt senzill. Tot i que el enunciat ho especifica, encara no he emprat disseny modular ja que no m'ha calgut ni li he vist una utilitat per aquesta primera fase. Més endevant sí dividiré les funcions d'utilitat per mòduls, però de cara a aquesta primera entrega no m'ha fet falta.

De cara al funcionament, ha estat senzill. Inicialment tenia intenció de desplaçar el cursor del fitxer binari a les posicions indicades pels documents de teoria i anar fent lectures dels tamanys indicats, però me'n vaig adonar bastant dora de que això suposaria un volum de codi elevat i no era gaire òptim estar pujant i baixant el cursor cada cop per cuadrar adequadament els offsets. És per això que el que he fet ha estat declarar dues estructures, una per el sistema de fitxers EXT i l'altre pel sistema de fitxers FAT12, i assegurar-me de que aquestes estructures tenen el tamany, i tamany de variables adequat per realitzar la lectura de la taula de cop, amb un sol read. 

És per això que puc dir que la meva pràctica funciona realitzant una simple lectura desde el offset 1024, en cas que el nombre s_magic de la estuctura no sigui equivalent a 0xEF53, considerem que no és EXT, pel que torno a desplaçar el cursor del fitxer binari a la posició desitjada per FAT12, i realitzo la lectura però aquest cop de la estructura del BootSector. A continuació realitzo la comprovació del número de clusters, i si aquesta és satisfactòria sabré que es tracta de un sistema FAT12, sinó, aviso al usuari mitjançant un missatge per pantalla.

### Estructures de dades Usades

Tal i com s'indica al disseny de la pràctica, s'han llegit les regions amb una sola lectura desant-se completament a les estrucutres, pel que he hagut de declarar totes les variables necessàries per poder contenir la taula sencera, ja sigui la del Boot Sector del sistema FAT12 o la del Superblock de EXT.

   ```c
    typedef struct {
      uint32_t s_inodes_count;      
      uint32_t s_blocks_count;      
      uint32_t s_r_blocks_count;    
      uint32_t s_free_blocks_count; 
      uint32_t s_free_inodes_count; 
      uint32_t s_first_data_block;  
      uint32_t s_log_block_size;    
      int32_t  s_log_frag_size;    
      uint32_t s_blocks_per_group;  
      uint32_t s_frags_per_group;   
      uint32_t s_inodes_per_group;  
      uint32_t s_mtime;             
      uint32_t s_wtime;             
      uint16_t s_mnt_count;         
      uint16_t s_max_mnt_count;     
      uint16_t s_magic;             
      uint16_t s_state;             
      uint16_t s_errors;            
      uint16_t s_minor_rev_level;   
      uint32_t s_lastcheck;         
      uint32_t s_checkinterval;     
      uint32_t s_creator_os;        
      uint32_t s_rev_level;         
      uint16_t s_def_resuid;        
      uint16_t s_def_resgid;        
      uint32_t s_first_ino;         
      uint16_t s_inode_size;        
      uint16_t s_block_group_nr;    
      uint32_t s_feature_compat;    
      uint32_t s_feature_incompat;  
      uint32_t s_feature_ro_compat; 
      uint8_t  s_uuid[16];          
      char     s_volume_name[16];   
      unsigned char     s_last_mounted[64];  
      uint32_t s_algo_bitmap;
    } SB;

    typedef struct {
      unsigned char BS_jmpBoot[3];     
      char BS_OEMName[8];              
      unsigned short BPB_BytsPerSec;    
      unsigned char BPB_SecPerClus;     
      unsigned short BPB_RsvdSecCnt;    
      unsigned char BPB_NumFATs;        
      unsigned short BPB_RootEntCnt;   
      unsigned short BPB_TotSec16;     
      unsigned char BPB_Media;          
      unsigned short BPB_FATSz16;       
      unsigned short BPB_SecPerTrk;     
      unsigned short BPB_NumHeads;      
      int BPB_HiddSec;                  
      int BPB_TotSec32;               
      unsigned char BS_DrvNum;          
      unsigned char BS_Reserved1;       
      unsigned char BS_BootSig;       
      int BS_VolID;                     
      char BS_VolLab[11];               
      char BS_FilSysType[8];            
    } BootSector;
   ```
### Proves realitzades

De cara a aquesta fase les úniques proves realitzades han estaat amb els fitxers proveïts al estudy. Aquests han estat els únics que he llegit i consultat al llarg del desenvolupament de la F1.

### Problemes observats

Pel que fa als problemes, hi han hagut uns quants. Començaria per comentar els de plantejament. Inicialment volia emprar la comanda lseek() de c, per poder desplaçar lliurement el cursor pel fitxer binari i anar agafant els valors que necessiti en cada moment.

### Estimació Temporal

El temps emprat per la realització d'aquesta fase no ha estat gaire elevat. Tot i que queda tot reflectit al git, la realització dels sistemes FAT i EXT ser de una tarda, aproximadament de unes 5h. Tot i així, això no reflecteix el nombre de commits que s'han anat realitzant, ja que he anat fent petites correccions al format en el que es mostraven les dades, i sobretot, realitzant aquesta documentació, que ha tingut un cost temporal aproximat a 2h.

## Explicació i valoració del GIT

## Conclusions generals


