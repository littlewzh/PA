#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  panic("Not implemented");
}

char *strcpy(char *dst, const char *src) {
  panic("Not implemented");
}

char *strncpy(char *dst, const char *src, size_t n) {
  panic("Not implemented");
}

char *strcat(char *dst, const char *src) {
  char *tmp=dst;
  while(*dst){
    dst++;

  }
  while((*dst++==*src++)!='\0');
  return tmp;
  //panic("Not implemented");
}

int strcmp(const char *s1, const char *s2) {
  int ret = 0;  
    while(!(ret=*(unsigned char*)s1-*(unsigned char*)s2) && *s1)  
    {  
        s1++;  
        s2++;
    }  
    if (ret < 0)  
    {  
        return -1;  
    }  
    else if (ret > 0)  
    {  
        return 1;
    }  
    return 0;
  //panic("Not implemented");
}

int strncmp(const char *s1, const char *s2, size_t n) {
  panic("Not implemented");
}

void *memset(void *s, int c, size_t n) {
  char *ps=(char *)s;
  char pc=(char)c;
  while(n--){
    *ps=pc;
    ps++;

  }
  return s;
  //nic("Not implemented");
}

void *memmove(void *dst, const void *src, size_t n) {
  panic("Not implemented");
}

void *memcpy(void *out, const void *in, size_t n) {
  if(out==NULL||in==NULL) {return NULL;}
  char *pout=(char *)out;
  char *pin=(char *)in;
  if(pout>pin&&pout<pin+n){
    pout=pout+n-1;
    pin=pin+n-1;
    while(n--){
    *pout=*pin;
    pout--;
    pin--;
    }
  }
  else {
    while(n--){
      *pout=*pin;
      pout++;
      pin++;
    }
  }
  return out;
  //panic("Not implemented");
}

int memcmp(const void *s1, const void *s2, size_t n) {
  char *ps1=(char *)s1;
  char *ps2=(char *)s2;
  if(ps1==NULL||ps2==NULL||n<0){
    return false;
  }
  while(n--){
    if(*ps1==*ps2){
      continue;
    }
    else {break;}
  }
  return (*ps1-*ps2);
  //panic("Not implemented");
}

#endif
