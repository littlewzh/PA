#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  char s[200];
  va_list args;
  int len;
  va_start(args,fmt);
  len=vsprintf(s,fmt,args);
  va_end(args);
  for(int i=0;i<len;i++){
    putch(*s);
  
  }
    
  return len; 
  //panic("Not implemented");
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  char *str=out;
  int ret=0;
  while(*fmt){
    if(*fmt!='%'){
      *str=*fmt;
      str++;

    }
    else {
      fmt++;
      if(*fmt=='d'){
        int32_t val=va_arg(ap,int32_t);
        int32_t n[10];
        int32_t k=0;
        while(val){
           k++;
           n[k]=val%10;
           val/=10;
        }
        while(k){
          *str=n[k]+'0';
          str++;
          k--;
        }
        
      }
      else if(*fmt=='s'){
        char *s=va_arg(ap,char *);
        while(*s){
          *str++=*s++;
        }
      }
    }
    fmt++;
    *str='\0';
  }
  ret=str-out;
  return ret;
  //return vsnprintf(out,-1,fmt,ap);
  //panic("Not implemented");
}

int sprintf(char *out, const char *fmt, ...) {
    va_list args;
    int value;
    va_start(args,fmt);
    value=vsprintf(out,fmt, args);
    va_end(args);
    return value;
  //panic("Not implemented");
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {

  panic("Not implemented");
}

#endif