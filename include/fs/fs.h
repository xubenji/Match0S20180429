#ifndef _CLOCK_H
#define _CLOCK_H
#include "kernel/types.h"

#include"lib/bitmap.h"

#define SECTOR_SIZE 512
#define BLOCK_SIZE SECTOR_SIZE

#define HD_SECTORS (20*1024*1024)/512
#define BLOCK_NAME_LEN 16
#define BLOCK_TYPE_DIRECTORY 1
#define BLOCK_TYPE_UNKNOWN 0

#define BLOCK_TYPE_REGULAR 2

#define BLOCK_DATA_ITEMS 128

#define FS_START_LBA 0

// 系统有了，改变一下名字，后面换回来
#define SEEK_SET 1
#define SEEK_CUR 2
#define SEEK_END 3

// #define HD_SECTORS 2
/* typedef unsigned int uint32_t; typedef unsigned short uint16_t; typedef
   unsigned char uint8_t; */
struct super_block {
  uint32_t magic;
  uint32_t sector_counts;
  uint32_t block_size;
  uint32_t block_bitmap_lba;
  uint32_t block_bitmap_sectors;
  uint32_t block_start_lba;
  uint32_t root_dir_block_id;
  uint8_t reserved[512 - 4 * 7];
};
#define BLOCK_PANE_NR 112
#define FILE_ALL_BLOCKS (BLOCK_PANE_NR*BLOCK_DATA_ITEMS*4)

struct block {
  uint8_t name[BLOCK_NAME_LEN];
  uint32_t type;
  uint32_t flags;
  uint32_t id;
  uint32_t parent_id;
  uint32_t size;
  uint32_t pos;
  uint8_t reserved[512 - BLOCK_PANE_NR * 4 - 4 * 10];
  uint32_t panes[BLOCK_PANE_NR];
};
#define MAX_FILE_OPEN 32

struct file {
  uint32_t fd_pos;
  uint32_t fd_flags;
  struct block *fd_block;
};
struct file file_table[MAX_FILE_OPEN];

#define O_RDONLY 1
#define O_WRONLY 2
#define O_RDWR 3
#define O_CREAT 4

#define FILE_TYPE_DIRECTORY 1
#define FILE_TYPE_UNKNOWN 0

#define FILE_TYPE_REGULAR 2



#define MAX_PATH_LEN 512


struct path_search_record {
  char searched_path[MAX_PATH_LEN];
  struct block *parent_dir;
  char file_type;
};


/* 文件属性结构体 */
struct stat {
  uint32_t st_block_id;         // block编号
  uint32_t st_size;             // 尺寸
  char st_filetype;
};

void fs_format();
void fs_mount();
int32_t block_bitmap_alloc();
void bitmap_sync(uint32_t bit_idx);
int32_t alloc_fd_global();
void open_root_dir();
void block_sync(struct block *block, void *io_buf);
int32_t block_id2lba();
struct block *block_open(uint32_t block_id);
void block_close(struct block *block);
void block_init();
int32_t block_release(uint32_t block_id);

void creat_dir_entry(char *filename, uint32_t parent_id, uint32_t block_id,
                     uint32_t file_type, struct block *dir_entry);
bool sync_dir_entry(struct block *parent_dir, struct block *dir_entry,
                    void *io_buf);
void dir_close(struct block *dir);
int32_t sys_open(const char *pathname, uint32_t flags);
static char *path_parse(char *pathname, char *name_store);
int32_t path_depth_cnt(char *pathname);
static int search_file(const char *pathname,
                       struct path_search_record *searched_record);
bool search_dir_entry(struct block *pdir, const char *name,
                      struct block *dir_entry);
struct block *dir_open(uint32_t block_id);
void init_fs();
int32_t file_open(uint32_t block_id, uint32_t flags);
int32_t file_close(struct file *file);
int32_t close(int32_t fd);
int32_t file_write(struct file *file, const void *buf, uint32_t count);
int32_t sys_write(int32_t fd, const void *buf, uint32_t count);
int32_t sys_read(int32_t fd, void *buf, uint32_t count);
int32_t file_read(struct file *file, void *buf, uint32_t count);
int32_t sys_lseek(int32_t fd, int32_t offset, uint8_t whence);
int32_t stat(const char *path, struct stat *buf);
int32_t sys_unlink(const char *pathname);
bool delete_dir_entry(struct block *pdir, uint32_t block_id, void *io_buf);
int32_t sys_mkdir(const char *pathname);
void creat_dir_entry(char *filename, uint32_t parent_id, uint32_t block_id,
                     uint32_t file_type, struct block *dir_entry);
int32_t sys_closedir(struct block *dir);
struct block *sys_opendir(const char *name);
struct block *dir_read(struct block *dir);
struct block *sys_readdir(struct block *dir);
void sys_rewinddir(struct block *dir);
bool dir_is_empty(struct block *dir);
int32_t dir_remove(struct block *parent_dir, struct block *child_dir);
int32_t sys_rmdir(const char *pathname);
void fs_main();

void sys_list_folder(char *name);

int32_t feof(int32_t fd);


#endif

