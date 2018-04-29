#include"fs/fs.h"
#include"driver/hd.h"
#include"kernel/memory.h"
#include"gui/gui.h"
#include"gui/layer.h"

/*Copyright XU&HU, 
  2018/1/15	
		  */

struct bitmap block_bitmap;
struct super_block sb;

struct block root_dir_block;

void sys_list_folder(char *name)
{
	struct block *dir_e;
	struct block *dir = sys_opendir(name);
	sys_rewinddir(dir);
	if (dir) {
		gprintk("-%s#  |#",name);
		char *type = NULL;
		dir_e = NULL;
		while ((dir_e = sys_readdir(dir))) {
			gprintk("   -%s#", dir_e->name);
		}
	} else {
		gprintk("%s open falied!#",name);
	}
}

void fs_main() {
  int i;
  char ch;
 int buf[4096];

  //layer_string(layer_desktop, 100,100, "fs begin.", COLOR_BLACK);
  //layer_integer(layer_desktop, 100,100+16, 10,sizeof(struct super_block), COLOR_WHITE);
  
  gprintk("block size:%d sb size:%d#", sizeof(struct block),sizeof(struct super_block));

  gprintk("----File System----#");
  init_fs();
  // gprintk("alloc block id:%d",block_bitmap_alloc());
  //char buf[4096];
  //char mb[1024 * 1024];
  char *s;

  // open("/.", O_CREAT);
  // sys_mkdir("/user/a");
/*
	sys_mkdir("/root");
	sys_mkdir("/user");
	sys_mkdir("/bin");
	sys_open("/foo", O_CREAT);
	sys_open("/bin/rm", O_CREAT);
	sys_open("/bin/dd", O_CREAT);
	sys_open("/bin/make", O_CREAT);
	*/
  //layer_string(layer_desktop, 100,100+16*4, "in init ok", COLOR_BLACK);

 // sys_mkdir("/user");
  //sys_mkdir("/bin");
  //sys_mkdir("/lib");
 
   //int fd = open("/kernel.bin", O_RDWR);
  //open("/user/a", O_CREAT);
  //gprintk("fd %d#",fd);
 
  //open("/README.RD", O_CREAT);
//  sys_rmdir("/libs");
 /* gprintk("try to delete nonempty directory /bin#");
  if (sys_rmdir("/bin") == -1) {
    gprintk("sys_rmdir: /bin delete fail!#");
  }
  gprintk("try to delete /bin/rm#");
  if (sys_rmdir("/bin/rm") == -1) {
    gprintk("sys_rmdir: /bin/rm delete fail!#");
  }
  if (unlink("/bin/rm") == 0) {
    gprintk("sys_unlink: /bin/rm delete done#");
  }

  gprintk("try to delete directory /bin again#");
  if (sys_rmdir("/bin") == 0) {
    gprintk("/bin delete done!#");
  }*/
  
    // struct block *dir = sys_opendir("/");
   
    // gprintk("creat %s",sys_mkdir("/dir/sub")==0?"done":"failed");
    // open("/bin/b", O_CREAT | O_RDWR);
   //struct stat obj_stat;

  //struct block *dir_e;

 /* struct block *root_dir = sys_opendir("/");

  sys_rewinddir(root_dir);
  if (root_dir) {
    gprintk("/ open done!#content:#");
    char *type = NULL;
    dir_e = NULL;
    while ((dir_e = sys_readdir(root_dir))) {
      if (dir_e->type == FILE_TYPE_REGULAR) {
        type = "regular";
      } else {
        type = "directory";
      }
      gprintk("      %s   %s#", type, dir_e->name);
	 
    }
    
  } else {
    gprintk("/ open fail!#");
  }*/
 /* buf[0] = 0xaa;
  buf[1] = 0x55;
  
  buf[512] = 0x66;
  buf[513] = 0x99;
  
  //s = "hello,world!";
  //strcpy(buf, s); 
  //s = "I am in second sector";
  //strcpy(buf + 512, s);
  
 int bytes = write(fd, buf,1024); 
  gprintk("written%d#", bytes);
	lseek(fd, 0, SEEK_SET);

    memset(buf, 0, 1024); 
	bytes = read(fd, buf, 512); 
	gprintk("read%d#",bytes); 
	gprintk("data%x %x#", buf[0], buf[1]);
  gprintk("data%x %x#", buf[512], buf[513]);
  
  
   close(fd);*/
  /*
  unlink("/kernel.bin");
  sys_rmdir("/bin");*/
 /* 
  sys_mkdir("/bin");
  open("/os", O_CREAT);
  
  */
 /* struct block *dir_e;

  struct block *root_dir = sys_opendir("/");

  
  
  
  
  sys_rewinddir(root_dir);
  if (root_dir) {
    gprintk("/ open done!#content:#");
    char *type = NULL;
    dir_e = NULL;
    while ((dir_e = sys_readdir(root_dir))) {
      if (dir_e->type == FILE_TYPE_REGULAR) {
        type = "regular";
      } else {
        type = "directory";
      }
      gprintk("      %s   %s#", type, dir_e->name);
	 
    }
    
  } else {
    gprintk("/ open fail!#");
  }
  */
   //layer_string(layer_desktop, 100,300+32, "fs init ok", COLOR_BLACK);
  /*
 root_dir = sys_opendir("/");
  sys_rewinddir(root_dir);
  if (root_dir) {
    gprintk("/ open done!#content:#");
    char *type = NULL;
    dir_e = NULL;
    while ((dir_e = sys_readdir(root_dir))) {
      if (dir_e->type == FILE_TYPE_REGULAR) {
        type = "regular";
      } else {
        type = "directory";
      }
      gprintk("      %s   %s#", type, dir_e->name);
    }

  } else {
    gprintk("/ open fail!#");
  }*/

  /* 
     fd = open("/a", O_CREAT | O_RDWR); gprintk("fd:%d#", fd);

     close(fd); fd2 = open("/b", O_CREAT | O_RDWR); gprintk("fd2:%d#", fd2);
     close(fd2); memset(buf, 0, 4096); memset(mb, 0, 1024 * 1024);



     s = "hello,world!"; strcpy(mb, s); s = "I am in second sector";
     strcpy(buf + 512, s);

     s = "/*!_!=,=_>!>,*_=!>_=!>_>_}_*"; strcpy(mb + 1024*100-1024, s);





     gprintk("ready write#");

     fd = open("/a", O_WRONLY); gprintk("fd:%d##", fd);

     fd2 = open("/b", O_RDONLY); gprintk("fd2:%d##", fd2);



     int bytes = write(fd, mb,1024*10); gprintk("written%d#", bytes);

     lseek(fd, 0, SEEK_SET);

     memset(buf, 0, 4096); bytes = read(fd, buf, 1024); gprintk("read%d#",
     bytes); gprintk("data%x#", *buf);


     close(fd); close(fd2); gprintk("fd:%d#", fd); gprintk("remove>#");
     unlink("/a"); fd = open("/bin", O_CREAT | O_RDWR); fd = open("/user",
     O_CREAT | O_RDWR); fd = open("/root", O_CREAT | O_RDWR); struct stat
     obj_stat; gprintk("ls>#"); memset(&obj_stat, 0, sizeof(struct stat ));
     stat("/", &obj_stat); gprintk("type:%s id:%d size:%d name:%s#",
     obj_stat.st_filetype == FILE_TYPE_REGULAR ? "regular" : "directory",
     obj_stat.st_block_id, obj_stat.st_size, "/"); memset(&obj_stat, 0,
     sizeof(struct stat )); stat("/a", &obj_stat); gprintk("type:%s id:%d
     size:%d name:%s#", obj_stat.st_filetype == FILE_TYPE_REGULAR ? "regular" 
     : "directory", obj_stat.st_block_id, obj_stat.st_size, "a");
     memset(&obj_stat, 0, sizeof(struct stat )); stat("/root", &obj_stat);
     gprintk("type:%s id:%d size:%d name:%s#", obj_stat.st_filetype ==
     FILE_TYPE_REGULAR ? "regular" : "directory", obj_stat.st_block_id,
     obj_stat.st_size, "root");

     unlink("/b"); */

  /* 
     lseek(fd, 0, SEEK_SET);

     s = "1MB IS HERE!"; strcpy(mb+4096, s);


     bytes = write(fd, mb, 1024*0124); gprintk("written%d#", bytes);

     lseek(fd, 0, SEEK_SET);

     memset(mb, 0, 1024*1024); bytes = read(fd, mb, 1024); gprintk("read%d#",
     bytes); */



  /* 
     fd = open("/a", O_CREAT); gprintk("fd:%d#",fd); fd = open("/b", O_CREAT);
     gprintk("fd:%d#",fd);

     fd = open("/b", O_CREAT); gprintk("fd:%d#",fd); */

  /* 
     memset(fb,0,SECTOR_SIZE); hd_read(0,0, fb,1); if(fb[0] == 5){ gprintk("disk
     has formatted."); }else{ memset(fb,0,SECTOR_SIZE);

     fb[0] = 5;

     hd_write(0,0, fb,1);

     memset(fb,0,SECTOR_SIZE); hd_read(0,0, fb,1); gprintk("%d",fb[0]); fb[0] =
     5;

     hd_write(0,1, fb,1); memset(fb,0,SECTOR_SIZE); hd_read(0,1, fb,1);

     gprintk("%d",fb[0]); } */
}

int32_t feof(int32_t fd)
{
	if(file_table[fd].fd_pos > file_table[fd].fd_block->size){
		return 1;
	}else {
		return 0;
	}
}

void init_fs() {
	//gprintk("init fs. start");
	//layer_string(layer_desktop, 100,100+16*1, "in init fs.", COLOR_BLACK);
  
  hd_read(0,1, (void *)&sb, 1);
  //layer_string(layer_desktop, 0, 300,"read", COLOR_WHITE);
  
  //gprintk("read");
  
  if (sb.magic == 0x19980325) {
    gprintk("disk has formatted.#  magic:%x#  all sectors:%x", sb.magic,
           sb.sector_counts);
	//layer_string(layer_desktop, 200, 0,"formatted", COLOR_WHITE);
  	fs_mount(&sb);
  } else {
	
    fs_format();
   fs_mount(&sb);
   
	//sys_mkdir("/root");
	//sys_mkdir("/user");
	//sys_mkdir("/bin");
	//sys_open("/foo", O_CREAT);
	//sys_open("/bin/rm", O_CREAT);
	//sys_open("/bin/dd", O_CREAT);
	//sys_open("/bin/make", O_CREAT);
   
  }
fs_mount(&sb);   
  
  
  uint32_t fd_idx = 0;
  while (fd_idx < MAX_FILE_OPEN) {
    file_table[fd_idx++].fd_block = NULL;

  }
  memset(&root_dir_block, 0, 512);
  open_root_dir();
	//layer_string(layer_desktop, 100,100+16*2, "in init fs. end", COLOR_BLACK);
}

int32_t close(int32_t fd) {
  int32_t ret = -1;
  if (fd >= 0) {
    ret = file_close(&file_table[fd]);
  }
  return ret;
}

int32_t file_open(uint32_t block_id, uint32_t flags) {
  int fd_idx = alloc_fd_global();

  if (fd_idx == -1) {
    gprintk("exceed max open files");
    return -1;
  }
  file_table[fd_idx].fd_block = (struct block *)block_open(block_id);
  file_table[fd_idx].fd_pos = 0;
  file_table[fd_idx].fd_flags = flags;

  if (flags & O_WRONLY || flags & O_RDWR) {
    // 判断是否有写操作

  }
  return fd_idx;
}


bool search_dir_entry(struct block * pdir, const char *name,
                      struct block * dir_entry) {
  uint32_t block_cnt = 112 * 128;

  uint32_t *all_blocks = (uint32_t *) sys_malloc(112 * 128 * 4);
  if (all_blocks == NULL) {
    gprintk("search alloc faild!");
    return false;
  }
  // gprintk("D 0#");
  uint32_t block_idx = 0;
  uint32_t pane_idx = 0;
  uint32_t *block_ptr = all_blocks;
  while (pane_idx < BLOCK_PANE_NR) {
    hd_read(0,pdir->panes[pane_idx], (void *)block_ptr, 1);
    block_ptr += 512 / 4;
    pane_idx++;
  }
  uint8_t *buf = (uint8_t *) sys_malloc(SECTOR_SIZE);
  struct block *temp_block = (struct block *)buf;


  while (block_idx < block_cnt) {
    if (all_blocks[block_idx] == 0) {
      block_idx++;
      continue;
    }
    hd_read(0,block_id2lba(all_blocks[block_idx]), buf, 1);
    if (!strcmp(temp_block->name, name)) {
      memcpy(dir_entry, temp_block, sizeof(struct block));
      sys_mfree(buf);
      sys_mfree(all_blocks);
      return true;
    }
    block_idx++;
    temp_block = (struct block *)buf;
    memset(buf, 0, SECTOR_SIZE);

  }
  sys_mfree(buf);
  sys_mfree(all_blocks);
  return false;
}


static int search_file(const char *pathname,
                       struct path_search_record *searched_record) {
  memset(searched_record->searched_path, 0, MAX_PATH_LEN);

  if (!strcmp(pathname, "/")) {
    searched_record->parent_dir = &root_dir_block;
    searched_record->file_type = FILE_TYPE_DIRECTORY;

    searched_record->searched_path[0] = 0;

    // gprintk("name find %s", pathname);
    return 0;
  }
  // gprintk("name find %s", pathname);
  uint32_t path_len = strlen(pathname);

  char *sub_path = (char *)pathname;

  struct block *parent_dir = &root_dir_block;
  struct block dir_entry;

  char name[BLOCK_NAME_LEN] = { 0 };

  searched_record->parent_dir = parent_dir;
  searched_record->file_type = FILE_TYPE_UNKNOWN;

  uint32_t parent_block_id = 0;
  // gprintk("name find %s#", pathname);
  memset(name, 0, BLOCK_NAME_LEN);
  // gprintk("###########[%s %s]#", sub_path);
  sub_path = path_parse(sub_path, name);

  // gprintk("###########[%s %s]#", sub_path, name);


  while (name[0]) {
    strcat(searched_record->searched_path, "/");
    strcat(searched_record->searched_path, name);

    if (search_dir_entry(parent_dir, name, &dir_entry)) {
      // gprintk("name find %s", name);
      memset(name, 0, BLOCK_NAME_LEN);
      if (sub_path) {
        sub_path = path_parse(sub_path, name);
      }
      if (dir_entry.type == FILE_TYPE_DIRECTORY) {
        parent_block_id = parent_dir->id;
        dir_close(parent_dir);
        parent_dir = (struct block *)dir_open(dir_entry.id);
        searched_record->parent_dir = parent_dir;
        continue;
      } else if (dir_entry.type == FILE_TYPE_REGULAR) {
        searched_record->file_type = FILE_TYPE_REGULAR;
        return dir_entry.id;
      }

    } else {
      // gprintk("name not find %s", name);
      return -1;
    }
  }
  dir_close(searched_record->parent_dir);
  searched_record->parent_dir = (struct block *)dir_open(parent_block_id);
  searched_record->file_type = FILE_TYPE_DIRECTORY;
  return dir_entry.id;
}


int32_t path_depth_cnt(char *pathname) {
  char *p = pathname;
  char name[BLOCK_NAME_LEN];

  uint32_t depth = 0;

  p = path_parse(p, name);

  while (name[0]) {
    depth++;
    memset(name, 0, BLOCK_NAME_LEN);
    if (p) {
      p = path_parse(p, name);
    }
  }
  return depth;
}

static char *path_parse(char *pathname, char *name_store) {
  if (pathname[0] == '/') {
    while (*(++pathname) == '/');

  }
  while (*pathname != '/' && *pathname != 0) {
    *name_store++ = *pathname++;
  }
  if (pathname[0] == 0) {
    return NULL;
  }
  return pathname;
}
int32_t sys_open(const char *pathname, uint32_t flags) {
  if (pathname[strlen(pathname) - 1] == '/') {
    gprintk("can't open a directory %s#", pathname);
    return -1;
  }
  // gprintk("On the path:%s#", pathname);
  int32_t fd = -1;

  struct path_search_record searched_record;
  memset(&searched_record, 0, sizeof(struct path_search_record));

  uint32_t pathname_depth = path_depth_cnt((char *)pathname);

  int block_id = search_file(pathname, &searched_record);

  gprintk("block_id %d#",block_id);


  bool found = block_id != -1 ? true : false;

  if (searched_record.file_type == FILE_TYPE_DIRECTORY) {
    gprintk("can't open a directory with open(), use opendir() to intead#");
    dir_close(searched_record.parent_dir);
    return -1;
  }

  uint32_t path_searched_depth = path_depth_cnt(searched_record.searched_path);

  if (pathname_depth != path_searched_depth) {
    gprintk("depth not equal#");
    dir_close(searched_record.parent_dir);
    return -1;
  }

  if (!found && !(flags & O_CREAT)) {
    gprintk("in path %s, file %s, is exist#", searched_record.searched_path,
           (strrchr(searched_record.searched_path, '/') + 1));
    dir_close(searched_record.parent_dir);
    return -1;
  } else if (found && flags & O_CREAT) {
    gprintk("%s has exist#", pathname);
    dir_close(searched_record.parent_dir);
    return -1;
  }

  switch (flags & O_CREAT) {
  case O_CREAT:
    gprintk("creating file#");
    fd =
      file_creat(searched_record.parent_dir, (strrchr(pathname, '/') + 1),
                 flags);
    dir_close(searched_record.parent_dir);

    break;
  default:
	gprintk("opening file#");
    fd = file_open(block_id, flags);
  }


  return fd;
}

int32_t file_close(struct file * file) {
  if (file == NULL) {
    return -1;
  }
  sys_mfree(file->fd_block);
  file->fd_block = NULL;
  return 0;
}


int32_t file_creat(struct block * parent_dir, char *filename, uint32_t flags) {


  // gprintk("**********[%s]#",filename);
  void *io_buf = (void *)sys_malloc(1024);
  if (io_buf == NULL) {
    gprintk("in file creat: sys_malloc for io_buf failed!#");
    return -1;
  }
  uint8_t rollback_step = 0;

  int32_t block_id = block_bitmap_alloc();
  // gprintk("block id:%d#", block_id);
  if (block_id == -1) {
    gprintk("in file creat: allocate block failed!#");
    return -1;
  }
  struct block *new_file_block = (struct block *)sys_malloc(sizeof(struct block));
  if (new_file_block == NULL) {
    gprintk("in file creat: sys_malloc for new file block failed!#");
    rollback_step = 1;
    goto rollback;
  }
  block_init(block_id, new_file_block);

  // gprintk("new block id:%d#", new_file_block->id);

  int fd_idx = alloc_fd_global();
  if (fd_idx == -1) {
    gprintk("exceed max open files!#");
    rollback_step = 2;
    goto rollback;
  }
  file_table[fd_idx].fd_block = new_file_block;
  file_table[fd_idx].fd_pos = 0;
  file_table[fd_idx].fd_flags = flags;

  struct block new_dir_entry;
  memset(&new_dir_entry, 0, sizeof(struct block));

  creat_dir_entry(filename, parent_dir->id, block_id, BLOCK_TYPE_REGULAR,
                  new_file_block);

  // gprintk("ready sync: name:%s id:%d#", new_file_block->name,
  // new_file_block->id);

  if (!sync_dir_entry(parent_dir, new_file_block, io_buf)) {
    gprintk("exceed max open files!#");
    rollback_step = 3;
    goto rollback;
  }

  memset(io_buf, 0, 1024);
  block_sync(parent_dir, io_buf);

  memset(io_buf, 0, 1024);
  block_sync(new_file_block, io_buf);

  bitmap_sync(block_id);

  sys_mfree(io_buf);
  return fd_idx;

rollback:
  switch (rollback_step) {
  case 3:
    memset(&file_table[fd_idx], 0, sizeof(struct file));
  case 2:
    sys_mfree(new_file_block);
  case 1:
    bitmap_set(&block_bitmap, block_id, 0);
    break;
  }
  sys_mfree(io_buf);
  return -1;
}


void block_init(uint32_t block_id, struct block *new_block) {
  new_block->id = block_id;
  new_block->size = 0;
  int i;
  for (i = 0; i < BLOCK_PANE_NR; i++) {
    new_block->panes[i] = 0;

  }


}

struct block *block_open(uint32_t block_id) {
  struct block *block_found;

  block_found = (struct block *)sys_malloc(sizeof(struct block));

  char *block_buf = (char *)sys_malloc(512);
  hd_read(0,block_id2lba(block_id), block_buf, 1);

  memcpy((void *)block_found, block_buf, sizeof(struct block));

  sys_mfree(block_buf);
  return block_found;
}

void block_close(struct block *block) {
  sys_mfree(block);
}



int32_t block_id2lba(int32_t id) {
  return sb.block_start_lba + id;
}
int32_t block_lba2id(int32_t lba) {
  return lba - sb.block_start_lba;
}


void block_sync(struct block *block, void *io_buf) {
  uint32_t block_id = block->id;

  struct block pure_block;
  memcpy(&pure_block, block, sizeof(struct block));

  char *block_buf = (char *)io_buf;

  memcpy(block_buf, &pure_block, sizeof(struct block));

  hd_write(0,block_id2lba(block_id), block_buf, 1);

}

struct block *dir_open(uint32_t block_id) {
  struct block *pdir = (struct block *)sys_malloc(sizeof(struct block));
  pdir = (struct block *)block_open(block_id);
  pdir->pos = 0;
  return pdir;
}
void open_root_dir() {
  // root_dir_block = NULL;
  struct block *dir_block = block_open(sb.root_dir_block_id);

  memcpy(&root_dir_block, dir_block, sizeof(struct block));
  root_dir_block.pos = 0;
  // int fd = open(".",O_CREAT|O_RDWR);
}
void dir_close(struct block *dir) {
  if (dir == &root_dir_block) {
    return;
  }
  block_close(dir);
}

void creat_dir_entry(char *filename, uint32_t parent_id, uint32_t block_id,
                     uint32_t file_type, struct block *dir_entry) {

  // gprintk("**********[%s]#",filename); 

  memcpy(dir_entry->name, filename, strlen(filename));

  // gprintk("strlen%d",strlen(filename));
  dir_entry->name[strlen(filename)] = 0;
  dir_entry->parent_id = parent_id;
  dir_entry->id = block_id;
  dir_entry->pos = 0;
  dir_entry->size = 0;
  
  dir_entry->type = file_type;
  gprintk("Create new dir entry name:%s block id:%d#", dir_entry->name,
         dir_entry->id);
}

bool sync_dir_entry(struct block *parent_dir, struct block *dir_entry,
                    void *io_buf) {
  struct block *dir_block = parent_dir;
  uint32_t dir_size = dir_block->size;

  gprintk("parent name:%s block id:%d#", parent_dir->name, parent_dir->id);
  gprintk("new dir entry name:%s block id:%d#", dir_entry->name,dir_entry->id);

  uint32_t block_idx = 0;
  uint32_t pane_idx = 0;

  uint32_t *data_buf = (uint32_t *) sys_malloc(BLOCK_SIZE);

  uint32_t *all_blocks = (uint32_t *) sys_malloc(4 * 128 * 112);

  int data_idx = 0;
  int block_lba = 0;
  uint32_t data_blocks = dir_block->size / 4 + 1;
  //gprintk("file data_block:%d#", data_blocks);

  while (data_blocks) {
    memset(data_buf, 0, SECTOR_SIZE);
    if (dir_block->panes[pane_idx] == 0) {
      block_lba = block_id2lba(block_bitmap_alloc());
      if (block_lba == -1) {
        gprintk("file_write: block_bitmap_alloc failed#");
        return -1;
      }
      dir_block->panes[pane_idx] = block_lba;
      bitmap_sync(block_lba2id(block_lba));
      hd_write(0,block_id2lba(dir_block->id), (void *)dir_block, 1);


      //gprintk("pane:%d data:%d#", pane_idx, dir_block->panes[pane_idx]);
    }

    hd_read(0,dir_block->panes[pane_idx], (void *)data_buf, 1);
    while (data_idx < 128) {
      if (data_buf[data_idx] == 0) {
        //gprintk("  data idx:%d data:%d#", pane_idx, data_buf[data_idx]);
        data_buf[data_idx] = dir_entry->id;
        hd_write(0,dir_block->panes[pane_idx], data_buf, 1);
        dir_block->size += 4;
        gprintk("  father dir size%d#", dir_block->size);
		//gprintk("@@@@@@@@@@@@@@@@%d", dir_block->size);

        memset(io_buf, 0, SECTOR_SIZE);
        hd_write(0,block_id2lba(dir_block->panes[pane_idx]), (void *)io_buf, 1);
        block_sync(dir_block, io_buf);

        sys_mfree(all_blocks);
        sys_mfree(data_buf);

        return true;
      }
      data_idx++;
    }


    data_idx = 0;

    pane_idx++;
    data_blocks--;
  }
  sys_mfree(all_blocks);
  sys_mfree(data_buf);
  return false;
}


int32_t alloc_fd_global() {
  uint32_t fd_idx = 0;
  while (fd_idx < MAX_FILE_OPEN) {
    if (file_table[fd_idx].fd_block == NULL) {
      break;
    }
    fd_idx++;
  }
  if (fd_idx == MAX_FILE_OPEN) {
    return -1;
  }
  return fd_idx;
}


void bitmap_sync(uint32_t bit_idx) {
  uint32_t off_sec = bit_idx / 4096;
  uint32_t off_size = off_sec * sb.block_size;
  uint32_t sec_lba;
  uint8_t *bitmap_off;
  sec_lba = sb.block_bitmap_lba + off_sec;
  bitmap_off = block_bitmap.bits + off_size;

  hd_write(0,sec_lba, bitmap_off, 1);

}

int32_t block_bitmap_alloc() {
  int32_t bit_idx = bitmap_scan(&block_bitmap, 1);
  if (bit_idx == -1) {
    return -1;
  }
  bitmap_set(&block_bitmap, bit_idx, 1);
  return bit_idx;
}

void fs_mount() {
  block_bitmap.bits =
    (uint8_t *) sys_malloc(sb.block_bitmap_sectors * SECTOR_SIZE);
  block_bitmap.btmp_bytes_len = sb.block_bitmap_sectors * SECTOR_SIZE;
  hd_read(0,sb.block_bitmap_lba, block_bitmap.bits, sb.block_bitmap_sectors);


}


void fs_format(){
	uint32_t boot_sector_lba = 0;
  uint32_t boot_sector_sectors = 1;
  uint32_t super_block_sectors = 1;
   uint32_t super_block_lba = boot_sector_lba + 1;
  uint32_t block_bitmap_sectors;

  uint32_t used_sectors = boot_sector_sectors + super_block_sectors;
  uint32_t sys_mfree_sectors = HD_SECTORS - used_sectors;



  block_bitmap_sectors = sys_mfree_sectors / 4096 + 1;

  uint32_t block_bitmap_bit_len = sys_mfree_sectors - block_bitmap_sectors;
  block_bitmap_sectors = block_bitmap_bit_len / 4096 + 1;

  sb.magic = 0x19980325;
  sb.sector_counts = HD_SECTORS;
  sb.block_size = SECTOR_SIZE;
  sb.block_bitmap_lba = super_block_lba+1;
  sb.block_bitmap_sectors = block_bitmap_sectors;
  sb.block_start_lba = sb.block_bitmap_lba + sb.block_bitmap_sectors;

  gprintk("fs_format...#");
  gprintk
    ("  magic:%x#  all sectors:%d#  block_bitmap_lba:%d#  block_bitmap_sectors:%d#  block_start_lba:%d#",
     sb.magic, sb.sector_counts, sb.block_bitmap_lba, sb.block_bitmap_sectors,
     sb.block_start_lba);
	 
  gprintk("  super_block_lba:%d#", 1);
  uint32_t buf_size = sb.block_bitmap_sectors * 512;
  uint8_t *buf = (uint8_t *) sys_malloc(buf_size);
  gprintk("  buf size:%d byte#", buf_size / 1024);

  buf[0] |= 0x01;               // 第一块留给根目录块
  uint32_t block_bitmap_last_byte = block_bitmap_bit_len / 8;
  uint32_t block_bitmap_last_bit = block_bitmap_bit_len % 8;
  uint32_t last_size = SECTOR_SIZE - (block_bitmap_last_byte % SECTOR_SIZE);

  memset(&buf[block_bitmap_last_byte], 0xff, last_size);

  uint8_t bit_idx = 0;
  while (bit_idx <= block_bitmap_last_bit) {
    buf[block_bitmap_last_byte] &= ~(1 << bit_idx++);
  }

  hd_write(0,sb.block_bitmap_lba, buf, sb.block_bitmap_sectors);
  
  //layer_string(layer_desktop, 100,100+16*5, "FIRST", COLOR_BLACK);
  
  
  /* memset(buf, 0, buf_size); hd_read(0,sb.block_bitmap_lba, buf,
     sb.block_bitmap_sectors); */
  // gprintk("%x#", buf[block_bitmap_last_byte]);
  memset(buf, 0, buf_size);

  struct block *root_block = (struct block *)buf;
  strcpy(root_block->name, "/");
  root_block->type = BLOCK_TYPE_DIRECTORY;
  root_block->flags = 0;
  root_block->id = 0;
  root_block->size = 0;

  sb.root_dir_block_id = root_block->id;

  hd_write(0,super_block_lba, (void *)&sb, 1);
  
  //char buf[512];
 //hd_read(0,super_block_lba, buf, 1);

  hd_write(0,block_id2lba(root_block->id), (void *)root_block, 1);

 // memset(root_block, 0, 512);
 // hd_read(0,sb.root_dir_block_id + sb.block_start_lba, (void *)root_block, 1);


  //layer_string(layer_desktop, 300, 0,16, root_block->type, COLOR_WHITE);
  
  
  gprintk("root directory block:#  name:%s#  type:%s#  size:%d#  id:%d#",
         root_block->name, root_block->type == 1 ? "directory" : "uknown",
         root_block->size, root_block->id);	 
	//layer_string(layer_desktop, 100,100+16*6, "2", COLOR_BLACK);
  	 
}

/* 把buf中的count个字节写入file,成功则返回写入的字节数,失败则返回-1 
 */
int32_t file_write(struct file *file, const void *buf, uint32_t count) {
  if ((file->fd_block->size + count) > (BLOCK_SIZE * 112 * 128)) { // 文件目前最大只支持512*140=71680字节
    gprintk("exceed max file_size 71680 bytes, write file failed#");
    return -1;
  }
  uint8_t *io_buf = (uint8_t *) sys_malloc(BLOCK_SIZE);
  if (io_buf == NULL) {
    gprintk("file_write: sys_malloc for io_buf failed#");
    return -1;
  }
  uint32_t *all_blocks = (uint32_t *) sys_malloc(FILE_ALL_BLOCKS);
  uint32_t *data_block = (uint32_t *) sys_malloc(BLOCK_SIZE);


  // 用来记录文件所有的块地址
  if (all_blocks == NULL) {
    gprintk("file_write: sys_malloc for all_blocks failed#");
    return -1;
  }

  const uint8_t *src = buf;     // 用src指向buf中待写入的数据 
  uint32_t bytes_written = 0;   // 用来记录已写入数据大小
  uint32_t size_left = count;   // 用来记录未写入数据大小
  int32_t block_lba = -1;       // 块地址
  uint32_t block_bitmap_idx = 0; // 用来记录block对应于block_bitmap中的索引,做为参数传给bitmap_sync
  uint32_t sec_idx;             // 用来索引扇区
  uint32_t sec_lba;             // 扇区地址
  uint32_t sec_off_bytes;       // 扇区内字节偏移量
  uint32_t sec_left_bytes;      // 扇区内剩余字节量
  uint32_t chunk_size;          // 每次写入硬盘的数据块大小
  int32_t indirect_block_table; // 用来获取一级间接表地址
  uint32_t block_idx;           // 块索引

gprintk("read write >#");
  int data_block_lba = 0;
  /* 判断文件是否是第一次写,如果是,先为其分配一个块 */
  if (file->fd_block->panes[0] == 0) {
    //gprintk("alloc first pane>#");
    block_lba = block_id2lba(block_bitmap_alloc());
    if (block_lba == -1) {
      gprintk("file_write: block_bitmap_alloc failed#");
      return -1;
    }
    file->fd_block->panes[0] = block_lba;
    //gprintk("first pane data:%d#", block_lba);
    bitmap_sync(block_lba2id(block_lba));
    hd_write(0,block_id2lba(file->fd_block->id), (void *)file->fd_block, 1);

    block_lba = block_id2lba(block_bitmap_alloc());
    data_block_lba = file->fd_block->panes[0];
    //gprintk("pane0 data:%d#", data_block_lba);

    hd_read(0,indirect_block_table, (void *)data_block, 1);

    if (block_lba == -1) {
      gprintk("file_write: block_bitmap_alloc failed#");
      return -1;
    }
    data_block[0] = block_lba;
    //gprintk("block data0 data:%d#", data_block[0]);

    block_bitmap_idx = block_lba2id(block_lba);
    /* 每分配一个块就将位图同步到硬盘 */
    bitmap_sync(block_bitmap_idx);
    hd_write(0,data_block_lba, (void *)data_block, 1);
    //gprintk("pane0 data%d block data%d#", data_block_lba, data_block[0]);

    memset(data_block, 0, 512);
    hd_read(0,file->fd_block->panes[0], (void *)data_block, 1);
    //gprintk("read at:%d info:%d#", file->fd_block->panes[0], data_block[0]);

    hd_read(0,data_block[0], (void *)all_blocks, 1);
    //gprintk("read at:%d info:%x#", data_block[0], all_blocks[0]);


  }

  /* 写入count个字节前,该文件已经占用的块数 */
  uint32_t file_has_used_blocks = file->fd_block->size / BLOCK_SIZE + 1;

  /* 存储count字节后该文件将占用的块数 */
  uint32_t file_will_use_blocks =
    (file->fd_block->size + count) / BLOCK_SIZE + 1;

  /* 通过此增量判断是否需要分配扇区,如增量为0,表示原扇区够用 
   */
  uint32_t add_blocks = file_will_use_blocks - file_has_used_blocks;
  memset(data_block, 0, 512);
  uint32_t data_blocks = file_will_use_blocks / BLOCK_PANE_NR + 1;

  //gprintk("used %d will use:%d will add:%d#", file_has_used_blocks,file_will_use_blocks, add_blocks);
  //gprintk("%%%%data blocks:%d#", data_blocks);



  int pane_idx = 0;

  int block_lba_idx = 0;
  int lba_in_data_idx = 0;



  // if (add_blocks == 0) {
  while (pane_idx < data_blocks) {
    //gprintk("pane%d start>", pane_idx);

    data_block_lba = file->fd_block->panes[pane_idx];
    //gprintk("::old pane%d data:%d#", pane_idx, data_block_lba);
    if (data_block_lba == 0) {
      block_lba = block_id2lba(block_bitmap_alloc());
      if (block_lba == -1) {
        gprintk("file_write: block_bitmap_alloc failed#");
        return -1;
      }
      file->fd_block->panes[pane_idx] = block_lba;
      //gprintk("++new pane%d data:%d#", pane_idx, block_lba);
      bitmap_sync(block_lba2id(block_lba));
      hd_write(0,block_id2lba(file->fd_block->id), (void *)file->fd_block, 1);


    }

    data_block_lba = file->fd_block->panes[pane_idx];
    //gprintk("@@old data block lba%d#", data_block_lba);
    memset(data_block, 0, 512);

    hd_read(0,data_block_lba, (void *)data_block, 1);

    while (lba_in_data_idx < BLOCK_DATA_ITEMS) {
      if (data_block[lba_in_data_idx] == 0) {
        block_lba = block_id2lba(block_bitmap_alloc());
        if (block_lba == -1) {
          gprintk("file_write: block_bitmap_alloc failed#");
          return -1;
        }
        data_block[lba_in_data_idx] = block_lba;
        //gprintk("--alloc a sector in block data%d data:%d#", lba_in_data_idx,data_block[lba_in_data_idx]);
        bitmap_sync(block_lba2id(block_lba));
        hd_write(0,data_block_lba, (void *)data_block, 1);

      }
      all_blocks[block_lba_idx] = data_block[lba_in_data_idx];
      block_lba_idx++;
      lba_in_data_idx++;
      // 到达最大就不继续收集了
      if (block_lba_idx >= file_will_use_blocks) {
        break;
      }
    }

    lba_in_data_idx = 0;
    // hd_read(0,all_blocks[0], (void*)all_blocks, 1);

    pane_idx++;
  }
 /* int i = 0;
  while (i < 4) {
    gprintk("store lba:%d#", all_blocks[i++]);
  }*/

  // } else {
  /* gprintk("need add#"); while (pane_idx < data_blocks) {
     indirect_block_table = file->fd_block->panes[pane_idx];
     hd_read(0,indirect_block_table, (void *)data_block, 1); //
     memcpy((all_blocks+128*pane_idx), data_block, BLOCK_SIZE); block_idx =
     file_has_used_blocks; // all_blocks[block_idx] = data_block[block_idx] =
     block_lba;

     bitmap_sync(block_lba2id(block_lba)); } hd_write(0,indirect_block_table,
     (void *)data_block, 1); // 同步一级间接块表到硬盘 pane_idx++;

     } */
  // }




  bool first_write_block = true; // 含有剩余空间的扇区标识
  /* 块地址已经收集到all_blocks中,下面开始写数据 */

  file->fd_pos = file->fd_block->size - 1; // 置fd_pos为文件大小-1,下面在写数据时随时更新
  
  int t = 0;
  
  while (bytes_written < count) { // 直到写完所有数据
    memset(io_buf, 0, BLOCK_SIZE);
    sec_idx = file->fd_block->size / BLOCK_SIZE;
    sec_lba = all_blocks[sec_idx];
    sec_off_bytes = file->fd_block->size % BLOCK_SIZE;
    sec_left_bytes = BLOCK_SIZE - sec_off_bytes;

    //gprintk("write begin>#lba:%d off:%dleft:%d#", sec_lba, sec_off_bytes,sec_left_bytes);


    /* 判断此次写入硬盘的数据大小 */
    chunk_size = size_left < sec_left_bytes ? size_left : sec_left_bytes;
    //gprintk("chunk size:%d#", chunk_size);
	
    if (first_write_block) {
      hd_read(0,sec_lba, io_buf, 1);
      gprintk("is first time write at lba%d#", sec_lba);
      first_write_block = false;
    }
    memcpy(io_buf + sec_off_bytes, src, chunk_size);
    hd_write(0,sec_lba, io_buf, 1);

    //hd_read(0,sec_lba, io_buf, 1);
    //gprintk("writed at lba 0x%x data:%x#", sec_lba, *io_buf); // 调试,完成后去掉

    src += chunk_size;          // 将指针推移到下个新数据
    file->fd_block->size += chunk_size; // 更新文件大小
    file->fd_pos += chunk_size;
    bytes_written += chunk_size;
    size_left -= chunk_size;
	t++;
  }
  
  memset(io_buf, 0, BLOCK_SIZE);
  block_sync(file->fd_block, io_buf);
  sys_mfree(all_blocks);
  sys_mfree(io_buf);
  sys_mfree(data_block);

	gprintk("written:%d t:%d#",bytes_written,t);
  return bytes_written;
}

int32_t sys_write(int32_t fd, const void *buf, uint32_t count) {
  if (fd < 0) {
    gprintk("sys_write:fd error");
    return -1;
  }
  struct file *wr_file = &file_table[fd];
  if (wr_file->fd_flags & O_WRONLY || wr_file->fd_flags & O_RDWR) {
    uint32_t bytes_written = file_write(wr_file, buf, count);
    return bytes_written;
  } else {
    gprintk("sys_write:not allowed!#");
    return -1;
  }
}

/* 从文件file中读取count个字节写入buf,
   返回读出的字节数,若到文件尾则返回-1 */
int32_t file_read(struct file * file, void *buf, uint32_t count) {
  uint8_t *buf_dst = (uint8_t *) buf;
  uint32_t size = count, size_left = size;
//gprintk("file_read:begin#");

  /* 若要读取的字节数超过了文件可读的剩余量,
     就用剩余量做为待读取的字节数 */
  if ((file->fd_pos + count) > file->fd_block->size) {
    size = file->fd_block->size - file->fd_pos;
	gprintk(">>file_read: size%d#", size);
    size_left = size;
    if (size == 0) {            // 若到文件尾则返回-1
      return -1;
    }
  }

  uint8_t *io_buf = (uint8_t *) sys_malloc(BLOCK_SIZE);
  if (io_buf == NULL) {
    gprintk("file_read: sys_malloc for io_buf failed#");
  }
  uint32_t *all_blocks = (uint32_t *) sys_malloc(BLOCK_SIZE * 128 * 112); // 用来记录文件所有的块地址
  if (all_blocks == NULL) {
    gprintk("file_read: sys_malloc for all_blocks failed#");
    return -1;
  }
  uint32_t *data_block = (uint32_t *) sys_malloc(BLOCK_SIZE);


  uint32_t block_read_start_idx = file->fd_pos / BLOCK_SIZE; // 数据所在块的起始地址
  uint32_t block_read_end_idx = (file->fd_pos + size) / BLOCK_SIZE; // 数据所在块的终止地址
  uint32_t read_blocks = block_read_end_idx - block_read_start_idx;
  uint32_t data_blocks = read_blocks / BLOCK_PANE_NR + 1;
  uint32_t block_idx;           // 获取待读的块地址 

  //所有的/BLOCK_DATA_ITEMS

  //gprintk("block_read_start_idx:%d block_read_end_idx:%d##",block_read_start_idx, block_read_end_idx);

 // gprintk("read_blocks:%d data_blocks:%d##", read_blocks, data_blocks);
  int pane_idx = 0;

  int block_lba_idx = 0;
  int lba_in_data_idx = 0;
  int data_block_lba = 0;

  //gprintk("read file >#");
  // if (add_blocks == 0) {
  while (pane_idx < data_blocks) {
    //gprintk("pane%d start>", pane_idx);
	//获取格子中的一个扇区
    data_block_lba = file->fd_block->panes[pane_idx];
    //gprintk("::old pane%d data:%d#", pane_idx, data_block_lba);
    if (data_block_lba != 0) {
      memset(data_block, 0, 512);
		//不为0就读取，得到一个数据块
      hd_read(0,data_block_lba, data_block, 1);

      while (lba_in_data_idx < BLOCK_DATA_ITEMS) {
        if (data_block[lba_in_data_idx] != 0) {
          all_blocks[block_lba_idx] = data_block[lba_in_data_idx];
          //gprintk("block data%d data:%d#", lba_in_data_idx,data_block[lba_in_data_idx]);
          block_lba_idx++;
        }

        lba_in_data_idx++;

      }

    }
    lba_in_data_idx = 0;
    // hd_read(0,all_blocks[0], (void*)all_blocks, 1);

    pane_idx++;
  }
  /*int i = 0;
  while (all_blocks[i] != 0) {
    gprintk("store lba:%d#", all_blocks[i++]);
  }*/

  /* 用到的块地址已经收集到all_blocks中,下面开始读数据 */
  uint32_t sec_idx, sec_lba, sec_off_bytes, sec_left_bytes, chunk_size;
  uint32_t bytes_read = 0;
  int t = 0;
  while (size_left > 0) {   // 直到读完为止
    sec_idx = file->fd_pos / BLOCK_SIZE;
    sec_lba = all_blocks[sec_idx];
    sec_off_bytes = file->fd_pos % BLOCK_SIZE;
    sec_left_bytes = BLOCK_SIZE - sec_off_bytes;

    //gprintk("read begin>#lba:%d off:%dleft:%d#", sec_lba, sec_off_bytes,sec_left_bytes);
    chunk_size = size_left < sec_left_bytes ? size_left : sec_left_bytes; // 待读入的数据大小
   // gprintk("chunk size:%d#", chunk_size);
    memset(io_buf, 0, BLOCK_SIZE);
    hd_read(0,sec_lba, io_buf, 1);
    memcpy(buf_dst, io_buf + sec_off_bytes, chunk_size);
    //gprintk("readed at lba 0x%x data:%x#", sec_lba, *io_buf); // 调试,完成后去掉

    buf_dst += chunk_size;
    file->fd_pos += chunk_size;
    bytes_read += chunk_size;
    size_left -= chunk_size;
	t++;
  }
  sys_mfree(all_blocks);
  sys_mfree(io_buf);
  
 gprintk("read size:%d t:%d#", bytes_read,t);
  
  return bytes_read;
}

int32_t sys_read(int32_t fd, void *buf, uint32_t count) {
 // gprintk("read:begin#");
  
  if (fd < 0) {
    gprintk("read: fd failed!#");
    return -1;
  }
  
  return file_read(&file_table[fd], buf, count);
}


/* 重置用于文件读写操作的偏移指针,成功时返回新的偏移量,出错时返回-1 
 */
int32_t sys_lseek(int32_t fd, int32_t offset, uint8_t whence) {
  if (fd < 0) {
    gprintk("lseek: fd error#");
    return -1;
  }

  struct file *pf = &file_table[fd];
  int32_t new_pos = 0;          // 新的偏移量必须位于文件大小之内
  int32_t file_size = (int32_t) pf->fd_block->size;
  switch (whence) {
    /* SEEK_SET
       新的读写位置是相对于文件开头再增加offset个位移量 */
  case SEEK_SET:
    new_pos = offset;
    break;

    /* SEEK_CUR
       新的读写位置是相对于当前的位置增加offset个位移量 */
  case SEEK_CUR:              // offse可正可负
    new_pos = (int32_t) pf->fd_pos + offset;
    break;

    /* SEEK_END
       新的读写位置是相对于文件尺寸再增加offset个位移量 */
  case SEEK_END:              // 此情况下,offset应该为负值
    new_pos = file_size + offset;
  }
  if (new_pos < 0 || new_pos > (file_size - 1)) {
    return -1;
  }
  pf->fd_pos = new_pos;
  return pf->fd_pos;
}

/* 在buf中填充文件结构相关信息,成功时返回0,失败返回-1 */
int32_t stat(const char *path, struct stat * buf) {
  /* 若直接查看根目录'/' */
  if (!strcmp(path, "/")) {
    buf->st_filetype = FILE_TYPE_DIRECTORY;
    buf->st_block_id = 0;
    buf->st_size = root_dir_block.size;
    return 0;
  }

  int32_t ret = -1;             // 默认返回值
  struct path_search_record searched_record;
  memset(&searched_record, 0, sizeof(struct path_search_record)); // 记得初始化或清0,否则栈中信息不知道是什么
  int block_id = search_file(path, &searched_record);


  if (block_id != -1) {
    struct block *obj_block = block_open(block_id); // 只为获得文件大小
    buf->st_size = obj_block->size;
    block_close(obj_block);
    buf->st_filetype = searched_record.file_type;
    buf->st_block_id = block_id;
    ret = 0;
  } else {
    gprintk("stat: %s not found#", path);
  }
  dir_close(searched_record.parent_dir);
  return ret;
}


/* 回收block的数据块和block本身 */
int32_t block_release(uint32_t block_id) {
  struct block *block_to_del = block_open(block_id);

  uint8_t *io_buf = (uint8_t *) sys_malloc(BLOCK_SIZE);
  if (io_buf == NULL) {
    gprintk("file_read: sys_malloc for io_buf failed#");
  }
  uint32_t *all_blocks = (uint32_t *) sys_malloc(BLOCK_SIZE * 128 * 112); // 用来记录文件所有的块地址
  if (all_blocks == NULL) {
    gprintk("file_read: sys_malloc for all_blocks failed#");
    return -1;
  }
  uint32_t *data_block = (uint32_t *) sys_malloc(BLOCK_SIZE);

  uint32_t block_read_start = 0; // 数据所在块的起始地址
  uint32_t block_read_end = block_to_del->size / BLOCK_SIZE + 1;
  // 计算一共多少个块
  uint32_t read_blocks = block_read_end - block_read_start;
  // 计算一共多少个格子，及数据块
  uint32_t data_blocks = read_blocks / BLOCK_DATA_ITEMS + 1;
  uint32_t block_idx;           // 获取待读的块地址 


 // gprintk("block_read_start_idx:%d block_read_end_idx:%d##", block_read_start, block_read_end);

  //gprintk("read_blocks:%d data_blocks:%d##", read_blocks, data_blocks);
  int pane_idx = 0;

  int block_lba_idx = 0;
  int lba_in_data_idx = 0;
  int data_block_lba = 0;

  gprintk("releaae block >#");
  // if (add_blocks == 0) {
  while (pane_idx < data_blocks) {
    //gprintk("pane%d start>#", pane_idx);

    data_block_lba = block_to_del->panes[pane_idx];
    //gprintk("::old pane%d data:%d#", pane_idx, data_block_lba);
    if (data_block_lba != 0) {
      memset(data_block, 0, 512);

      hd_read(0,data_block_lba, (void *)data_block, 1);
		all_blocks[block_lba_idx++] = data_block_lba;
      while (lba_in_data_idx < BLOCK_DATA_ITEMS) {
        if (data_block[lba_in_data_idx] != 0) {
          all_blocks[block_lba_idx] = data_block[lba_in_data_idx];
          //gprintk("block data idx:%d data:%d#", lba_in_data_idx,data_block[lba_in_data_idx]);
          block_lba_idx++;
        }

        lba_in_data_idx++;

      }

    }
    lba_in_data_idx = 0;
    // hd_read(0,all_blocks[0], (void*)all_blocks, 1);

    pane_idx++;
  }
  block_lba_idx = 0;
  int del_lba;
  while (all_blocks[block_lba_idx] != 0) {
    //gprintk("#*.*stored a lba:%d ->", all_blocks[block_lba_idx]);
    del_lba = all_blocks[block_lba_idx];
    bitmap_set(&block_bitmap, block_lba2id(del_lba), 0);
    bitmap_sync(block_lba2id(del_lba));
    memset(io_buf, 0, SECTOR_SIZE);
    hd_read(0,del_lba, io_buf, 1);
    //gprintk("sector:%d data:%c#", del_lba, io_buf[0]);
    memset(io_buf, 0, SECTOR_SIZE);
    hd_write(0,all_blocks[block_lba_idx], io_buf, 1);
    memset(io_buf, 0, SECTOR_SIZE);
    hd_read(0,all_blocks[block_lba_idx], io_buf, 1);
    // gprintk("data:%c #", io_buf[0]);
    memset(io_buf, 0, SECTOR_SIZE);
    block_lba_idx++;
  }
  //清除block本身的数据（目录，文件）
	//memset(io_buf, 0, SECTOR_SIZE);
	//hd_read(0,block_id2lba(block_id), io_buf, 1);
	memset(io_buf, 0, SECTOR_SIZE);
	hd_write(0,block_id2lba(block_id), io_buf, 1);
 
  sys_mfree(all_blocks);
  sys_mfree(data_block);

  sys_mfree(io_buf);
   /***********************************************/

  block_close(block_to_del);
}


/* 把分区part目录pdir中编号为block_id的目录项删除 */
bool delete_dir_entry(struct block *pdir, uint32_t block_id, void *io_buf) {
  struct block *dir_block = pdir;
  uint32_t block_idx = 0;
  uint32_t pane_idx = 0;

  uint32_t *data_buf = (uint32_t *) sys_malloc(BLOCK_SIZE);

  uint32_t *all_blocks = (uint32_t *) sys_malloc(4 * 128 * 112);

  int data_idx = 0;
  uint32_t data_blocks = dir_block->size / 4 + 1;
  gprintk("del file data_block:%d#", data_blocks);

  while (data_blocks) {
    memset(data_buf, 0, SECTOR_SIZE);
    if (dir_block->panes[pane_idx] != 0) {
      //gprintk("pane:%d data:%d#", pane_idx, dir_block->panes[pane_idx]);

      hd_read(0,dir_block->panes[pane_idx], (void *)data_buf, 1);
      while (data_idx < 128) {
        if (data_buf[data_idx] != 0) {
          //gprintk("  data idx:%d data:%d#", pane_idx, data_buf[data_idx]);
          if (data_buf[data_idx] == block_id) {

            data_buf[data_idx] = 0;
            hd_write(0,dir_block->panes[pane_idx], data_buf, 1);
            dir_block->size -= 4;
            //gprintk("  dir size%d#", dir_block->size);
            bitmap_set(&block_bitmap, block_id, 0);
            bitmap_sync(block_id);

            memset(io_buf, 0, SECTOR_SIZE);
            block_sync(dir_block, io_buf);
            sys_mfree(all_blocks);
            sys_mfree(data_buf);

            return true;
          }
        }
        data_idx++;
      }
      data_idx = 0;

    }
    pane_idx++;
    data_blocks--;
  }
  sys_mfree(all_blocks);
  sys_mfree(data_buf);

  return false;
}

/* 删除文件(非目录),成功返回0,失败返回-1 */
int32_t sys_unlink(const char *pathname) {


  /* 先检查待删除的文件是否存在 */
  struct path_search_record searched_record;
  memset(&searched_record, 0, sizeof(struct path_search_record));
  int block_id = search_file(pathname, &searched_record);

  // gprintk("s a#");

  if (block_id == -1) {
    gprintk("file %s not found!#", pathname);
    dir_close(searched_record.parent_dir);
    return -1;
  }
  if (searched_record.file_type == FILE_TYPE_DIRECTORY) {
    gprintk("can`t delete a direcotry with unlink(), use rmdir() to instead#");
    dir_close(searched_record.parent_dir);
    return -1;
  }
  // gprintk("name:%s
  // id:%d#",file_table[0].fd_block->name,file_table[0].fd_block->id);
  /* 检查是否在已打开文件列表(文件表)中 */
  uint32_t file_idx = 0;
  while (file_idx < MAX_FILE_OPEN) {
    if (file_table[file_idx].fd_block != NULL
        && (uint32_t) block_id == file_table[file_idx].fd_block->id) {
      break;
    }
    file_idx++;
  }
  // gprintk("name:%s
  // id:%d#",file_table[file_idx].fd_block->name,file_table[file_idx].fd_block->id);
  gprintk("del file block id:%d#", block_id);
  if (file_idx < MAX_FILE_OPEN) {
    dir_close(searched_record.parent_dir);
    gprintk("file %s is in use, not allow to delete!#", pathname);
    return -1;
  }

  /* 为delete_dir_entry申请缓冲区 */
  void *io_buf = (void *)sys_malloc(SECTOR_SIZE + SECTOR_SIZE);
  if (io_buf == NULL) {
    dir_close(searched_record.parent_dir);
    gprintk("sys_unlink: sys_malloc for io_buf failed#");
    return -1;
  }

  struct block *parent_dir = searched_record.parent_dir;
  delete_dir_entry(parent_dir, block_id, io_buf);
  block_release(block_id);
  sys_mfree(io_buf);
  dir_close(searched_record.parent_dir);
  return 0;                     // 成功删除文件 
}


int32_t sys_mkdir(const char *pathname) {
  //gprintk("create dir...#");
  uint8_t rollback_step = 0;    // 用于操作失败时回滚各资源状态
  void *io_buf = (void *)sys_malloc(SECTOR_SIZE * 2);
  if (io_buf == NULL) {
    gprintk("sys_mkdir: sys_malloc for io_buf failed#");
    return -1;
  }
  struct block *new_dir_block = (struct block *)sys_malloc(sizeof(struct block));
  if (new_dir_block == NULL) {
    gprintk("sys_mkdir: sys_malloc for block failed#");
    return -1;
  }

  struct path_search_record searched_record;
  memset(&searched_record, 0, sizeof(struct path_search_record));
  int block_id = -1;
  block_id = search_file(pathname, &searched_record);
  if (block_id != -1) {         // 如果找到了同名目录或文件,失败返回
    gprintk("sys_mkdir: file or directory %s exist!#", pathname);
    rollback_step = 1;
    goto rollback;
  } else {                      // 若未找到,也要判断是在最终目录没找到还是某个中间目录不存在
    //gprintk("dir block id:%d#", block_id);
    uint32_t pathname_depth = path_depth_cnt((char *)pathname);
    uint32_t path_searched_depth =
      path_depth_cnt(searched_record.searched_path);

    // gprintk("%s",searched_record.searched_path);
    /* 先判断是否把pathname的各层目录都访问到了,即是否在某个中间目录就失败了 
     */
    if (pathname_depth != path_searched_depth) { // 说明并没有访问到全部的路径,某个中间目录是不存在的
      gprintk("sys_mkdir: can`t access %s, subpath %s is`t exist#", pathname,
             searched_record.searched_path);
      rollback_step = 1;
      goto rollback;
    }
  }

  struct block *parent_dir = searched_record.parent_dir;
  /* 目录名称后可能会有字符'/',所以最好直接用searched_record.searched_path,无'/' 
   */
  char *dirname = (char *)(strrchr((char *)(searched_record.searched_path), '/') + 1);

  block_id = block_bitmap_alloc();
  //gprintk("dir block id:%d#", block_id);
  if (block_id == -1) {
    gprintk("sys_mkdir: allocate block failed#");
    rollback_step = 1;
    goto rollback;
  }

  memset(new_dir_block, 0, BLOCK_SIZE);
  //gprintk("::::::::::new dir id:%d#", block_id);
  block_init(block_id, new_dir_block); // 初始化i结点
  /* 
     uint32_t block_bitmap_idx = 0; //
     用来记录block对应于block_bitmap中的索引 int32_t block_lba =
     -1;



     block_bitmap_idx = block_bitmap_alloc(); block_lba =
     block_id2lba(block_bitmap_idx); if (block_bitmap_idx == -1) {
     gprintk("sys_mkdir: block_bitmap_alloc for create directory failed#");
     rollback_step = 2; goto rollback; } new_dir_block->panes[0] = block_lba;

     gprintk("a lba=%d in panes0#",new_dir_block->panes[0]);



     bitmap_sync( block_bitmap_idx);


     memset(io_buf, 0, SECTOR_SIZE * 2); // 清空io_buf io_buf[0] = block_id;
     hd_write(0,new_dir_block->panes[0], io_buf, 1);

     new_dir_block->size = 4; */
  /* 在父目录中添加自己的目录项 */
  // struct block new_dir_entry;
  // memset(&new_dir_entry, 0, sizeof(struct block));
  creat_dir_entry(dirname, parent_dir->id, block_id, FILE_TYPE_DIRECTORY,
                  new_dir_block);

  // gprintk("++%s#",new_dir_block->name);
  // gprintk("new name:%s id:%d type:%d#",dirname, new_dir_block->id,
  // new_dir_block->type);


  memset(io_buf, 0, SECTOR_SIZE * 2); // 清空io_buf
  if (!sync_dir_entry(parent_dir, new_dir_block, io_buf)) { // sync_dir_entry中将block_bitmap通过bitmap_sync同步到硬盘
    gprintk("sys_mkdir: sync_dir_entry to disk failed!#");
    rollback_step = 2;
    goto rollback;
  }

  /* 父目录的block同步到硬盘 */
  memset(io_buf, 0, SECTOR_SIZE * 2);
  block_sync(parent_dir, io_buf);

  /* 将新创建目录的block同步到硬盘 */
  memset(io_buf, 0, SECTOR_SIZE * 2);
  block_sync(new_dir_block, io_buf);

  /* 将block位图同步到硬盘 */
  bitmap_sync(block_id);

  // new_dir_block = block_open(block_id);
  // gprintk("%s %d",new_dir_block->name, new_dir_block->type);


  sys_mfree(io_buf);
  sys_mfree(new_dir_block);

  /* 关闭所创建目录的父目录 */
  dir_close(searched_record.parent_dir);
  return 0;

  /* 创建文件或目录需要创建相关的多个资源,若某步失败则会执行到下面的回滚步骤 
   */
rollback:                      // 因为某步骤操作失败而回滚
  switch (rollback_step) {
  case 2:
    bitmap_set(&block_bitmap, block_id, 0); // 如果新文件的block创建失败,之前位图中分配的block_id也要恢复 
                                            // 
    // 
  case 1:
    /* 关闭所创建目录的父目录 */
    dir_close(searched_record.parent_dir);
    break;
  }
  sys_mfree(io_buf);
  sys_mfree(new_dir_block);
  return -1;
}

/* 目录打开成功后返回目录指针,失败返回NULL */
struct block *sys_opendir(const char *name) {

  if (name[0] == '/' && name[1] == 0) {
    return &root_dir_block;
  }

  /* 先检查待打开的目录是否存在 */
  struct path_search_record searched_record;
  memset(&searched_record, 0, sizeof(struct path_search_record));
  int block_id = search_file(name, &searched_record);
  struct block *ret = NULL;
  if (block_id == -1) {         // 如果找不到目录,提示不存在的路径 
                                // 
    // 
    gprintk("In %s, sub path %s not exist#", name,
           searched_record.searched_path);
  } else {
    // gprintk("#%s %d#",name, searched_record.file_type);

    if (searched_record.file_type == FILE_TYPE_REGULAR) {

      gprintk("%s is regular file!#", name);
    } else if (searched_record.file_type == FILE_TYPE_DIRECTORY) {
      ret = dir_open(block_id);
    }
  }
  // gprintk("////////open a dir:%s size:%d",ret->name,ret->size);
  // dir_close(searched_record.parent_dir);
  return ret;
}

/* 成功关闭目录dir返回0,失败返回-1 */
int32_t sys_closedir(struct block * dir) {
  int32_t ret = -1;
  if (dir != NULL) {
    dir_close(dir);
    ret = 0;
  }
  return ret;
}

/* 读取目录,成功返回1个目录项,失败返回NULL */
struct block *dir_read(struct block *dir) {
  struct block *dir_e = (struct block *)sys_malloc(BLOCK_SIZE);
  struct block *dir_block = dir;

  uint32_t block_idx = 0;
  uint8_t *io_buf = (uint8_t *) sys_malloc(BLOCK_SIZE);


  if (io_buf == NULL) {
    gprintk("file_read: sys_malloc for io_buf failed#");
  }
  uint32_t *all_blocks_id = (uint32_t *) sys_malloc(4 * 128 * 112); // 用来记录文件所有的块地址
  if (all_blocks_id == NULL) {
    gprintk("file_read: sys_malloc for all_blocks failed#");
    return NULL;
  }
  uint32_t *data_block = (uint32_t *) sys_malloc(BLOCK_SIZE);

  uint32_t dir_entry_nr = dir_block->size / 4; // 数据所在块的终止地址

  // gprintk("dir size:%d#", dir_block->size);

  uint32_t data_blocks = (dir_block->size / 4) / 128 + 1 + 1;

  // gprintk("dir_entry_nr:%d will use data block:%d#",dir_entry_nr,
  // data_blocks);

  int pane_idx = 0;

  int block_id_idx = 0;
  int lba_in_data_idx = 0;
  int data_block_lba = 0;






  // gprintk("@read dir >#");
  // if(dir_block->pos < dir_block->size){
  while (pane_idx < data_blocks) {
    // gprintk("pane%d start>", pane_idx);

    data_block_lba = dir_block->panes[pane_idx];
    // gprintk("::old pane%d data:%d#", pane_idx, data_block_lba);
    if (data_block_lba != 0) {
      memset(data_block, 0, 512);

      hd_read(0,data_block_lba, (void *)data_block, 1);

      while (lba_in_data_idx < BLOCK_DATA_ITEMS) {
        if (data_block[lba_in_data_idx] != 0) {
          all_blocks_id[block_id_idx] = data_block[lba_in_data_idx];
          // gprintk("block data%d data:%d#",
          // lba_in_data_idx,data_block[lba_in_data_idx]);
          block_id_idx++;
        }

        lba_in_data_idx++;

      }

    }
    lba_in_data_idx = 0;
    // hd_read(0,all_blocks[0], (void*)all_blocks, 1);

    pane_idx++;
  }
  /* int i = 0; while (all_blocks_id[i] != 0) { gprintk("store id:%d#",
     all_blocks_id[i++]); } */
  block_idx = 0;
  block_id_idx = 0;

  //gprintk("!!!!block size:%d#",dir_block->size);
  while (all_blocks_id[block_id_idx + dir_block->pos/4]) {
	  
    if (dir_block->pos <= dir_block->size) {
      dir_e = dir_open(all_blocks_id[block_id_idx + dir_block->pos/4]);
      if (dir_e->type) {
		  //gprintk("!!!!block pos:%d#",dir_block->pos);
        dir_block->pos += 4;
		//gprintk("!!!!block pos:%d#",dir_block->pos);
        // gprintk("readed.");
        //block_sync(dir_block, io_buf);
        //gprintk("dir entry id:%d name:%s #", dir_e->id, dir_e->name);
        return dir_e;
      }
      dir_close(dir_e);
    }
    block_id_idx++;
  }
  /* 
     uint32_t cur_dir_entry_pos = 0; //
     当前目录项的偏移,此项用来判断是否是之前已经返回过的目录项
     uint32_t dir_entry_size = cur_part->sb->dir_entry_size; uint32_t dir_entrys_per_sec =
     SECTOR_SIZE / dir_entry_size; // 1扇区内可容纳的目录项个数

     while (block_idx < block_cnt) { if (dir->dir_pos >= dir_inode->i_size) { return NULL;
     } if (all_blocks[block_idx] == 0) { //
     如果此块地址为0,即空块,继续读出下一块 block_idx++; continue; }
     memset(dir_e, 0, SECTOR_SIZE); ide_read(cur_part->my_disk, all_blocks[block_idx],
     dir_e, 1); dir_entry_idx = 0;

     while (dir_entry_idx < dir_entrys_per_sec) { if ((dir_e + dir_entry_idx)->f_type) { // 
     如果f_type不等于0,即不等于FT_UNKNOWN

     if (cur_dir_entry_pos < dir->dir_pos) { cur_dir_entry_pos += dir_entry_size;
     dir_entry_idx++; continue; } ASSERT(cur_dir_entry_pos == dir->dir_pos); dir->dir_pos
     += dir_entry_size; // 更新为新位置,即下一个返回的目录项地址 return
     dir_e + dir_entry_idx; } dir_entry_idx++; } block_idx++; } */
  return NULL;

}


/* 读取目录dir的1个目录项,成功后返回其目录项地址,到目录尾时或出错时返回NULL 
 */
struct block *sys_readdir(struct block *dir) {
  return dir_read(dir);
}

/* 把目录dir的指针dir_pos置0 */
void sys_rewinddir(struct block *dir) {
  dir->pos = 0;
}

/* 判断目录是否为空 */
bool dir_is_empty(struct block *dir) {
  return (dir->size == 0);
}

/* 在父目录parent_dir中删除child_dir */
int32_t dir_remove(struct block * parent_dir, struct block * child_dir) {
  struct block *child_dir_block = child_dir;

  void *io_buf = (void *)sys_malloc(SECTOR_SIZE * 2);
  if (io_buf == NULL) {
    gprintk("dir_remove: sys_malloc for io_buf failed#");
    return -1;
  }

  /* 在父目录parent_dir中删除子目录child_dir对应的目录项 */
  delete_dir_entry(parent_dir, child_dir_block->id, io_buf);

  /* 回收inode中i_secotrs中所占用的扇区,并同步inode_bitmap和block_bitmap 
   */
  block_release(child_dir_block->id);
  sys_mfree(io_buf);
  return 0;
}
/* 删除空目录,成功时返回0,失败时返回-1 */
int32_t sys_rmdir(const char *pathname) {
  /* 先检查待删除的文件是否存在 */
  struct path_search_record searched_record;
  memset(&searched_record, 0, sizeof(struct path_search_record));
  int block_id = search_file(pathname, &searched_record);
  int retval = -1;              // 默认返回值
  if (block_id == -1) {
    gprintk("In %s, sub path %s not exist#", pathname,
           searched_record.searched_path);
  } else {
    if (searched_record.file_type == FILE_TYPE_REGULAR) {
      gprintk("%s is regular file!#", pathname);
    } else {
      struct block *dir = dir_open(block_id);
      if (!dir_is_empty(dir)) { // 非空目录不可删除
        gprintk
          ("dir %s is not empty, it is not allowed to delete a nonempty directory!#",
           pathname);
      } else {
        if (!dir_remove(searched_record.parent_dir, dir)) {
          retval = 0;
        }
      }
      dir_close(dir);
    }
  }
  dir_close(searched_record.parent_dir);
  return retval;
}
