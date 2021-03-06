#include <fs.h>

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);
extern size_t ramdisk_read(void *buf, size_t offset, size_t len);
extern size_t get_ramdisk_size();
extern size_t ramdisk_write(const void *buf, size_t offset, size_t len);
extern size_t events_read(void *buf, size_t offset, size_t len);
extern size_t dispinfo_read(void *buf, size_t offset, size_t len);
extern size_t fb_write(const void *buf, size_t offset, size_t len);
typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  size_t offset;
  ReadFn read;
  WriteFn write;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  [FD_STDIN]  = {"stdin", 0, 0, 0, invalid_read, invalid_write},
  [FD_STDOUT] = {"stdout", 0, 0, 0, invalid_read, serial_write},
  [FD_STDERR] = {"stderr", 0, 0, 0, invalid_read, serial_write},                //?????????shenmoguiyisi
#include "files.h"
  {"/dev/events", 0, 0, 0, events_read, invalid_write},
  {"/proc/dispinfo", 0, 0, 0,dispinfo_read, invalid_write},
  {"/dev/fb", 0, 0, 0, invalid_read, fb_write},
};
#define fs_number (sizeof(file_table)/sizeof(Finfo))
void init_fs() {
  file_table[fs_number-1].size = 300*400*4;//screen_size();
  file_table[fs_number-2].size = 128;
  // TODO: initialize the size of /dev/fb
}
int fs_open(const char *pathname, int flags, int mode){
  for(int i=3;i<fs_number;i++){
     if(strcmp(pathname,file_table[i].name)==0){
       //if(i!=fs_number-3) Log("open file = %s",file_table[i].name);
       file_table[i].offset = 0;
       return i;
     }
  }
  panic("do not find %s",pathname);
}
size_t fs_read(int fd, void *buf, size_t len){
  size_t size;
  size= (file_table[fd].offset+len<=file_table[fd].size) ? len : (file_table[fd].size-file_table[fd].offset);
  if(file_table[fd].read==NULL){
    size=ramdisk_read(buf,file_table[fd].disk_offset+file_table[fd].offset,size);
  }
  else {
    size=file_table[fd].read(buf,file_table[fd].disk_offset+file_table[fd].offset,size);
  }
  file_table[fd].offset+=size;
  return size;
}
size_t fs_write(int fd, const void *buf, size_t len){
  size_t size;
  size= (file_table[fd].offset+len<=file_table[fd].size) ? len : (file_table[fd].size-file_table[fd].offset);
  if(file_table[fd].write==NULL){
    ramdisk_write(buf,file_table[fd].disk_offset+file_table[fd].offset,size);
  }
  else {
    return file_table[fd].write(buf,file_table[fd].offset,len);/// len or size?????
  }
  file_table[fd].offset+=size;
  return size;
}
int fs_close(int fd){
  file_table[fd].offset = 0;
  return 0;             //always return close
}
size_t fs_lseek(int fd, size_t offset, int whence){
  switch(whence){
    case SEEK_SET:
      file_table[fd].offset=offset;
      break;
    case SEEK_CUR:
      file_table[fd].offset+=offset;
      break;
    case SEEK_END:
      file_table[fd].offset=file_table[fd].size+offset;
      break;
    default : assert(0);
  }
  return file_table[fd].offset;
}
