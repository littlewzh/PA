/*#include <am.h>
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
    putch(s[i]);
  
  }
    
  return len; 
  //panic("Not implemented");
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  char *str=out;
  int ret=0;
  char nums[100];
  int flag;
  uint32_t width;
  int len;
  while(*fmt){
    if(*fmt!='%'){
      *str=*fmt;
      str++;

    }
    else {
      fmt++;
      flag=0;
      while(*fmt=='0'){
         flag=1;
         fmt++;
      }
      width=0;
     if(*fmt>='0'&&*fmt<='9'){
       while(*fmt>='0'&&*fmt<='9'){
         width=width*10+*fmt-'0';
         fmt++;
       }
     }
      if(*fmt=='d'){
        //memset(nums,'0',100);
        int32_t val=va_arg(ap,int32_t);
        //int32_t n[20];
        int32_t k=0;
        while(val){
           k++;
           nums[k]=val%10+'0';
           val/=10;
        }
        if(k<width&&flag==1){
           len=width-k;
           while(len--){
             *str++='0';
           }
        }
        while(k){
          *str=nums[k];
          str++;
          k--;
        }
      }
      else if(*fmt=='x'||*fmt=='p'){
        uint32_t valx=(uintptr_t)va_arg(ap,uint32_t);
        int32_t k=0;
        while(valx){
          k++;
          nums[k]=valx%16+'0'+7;
          valx/=16;
        }
        if(k<width&&flag==1){
           len=width-k;
           while(len--){
             *str++='0';
           }
        }
        while(k){
          *str=nums[k];
          str++;
          k--;
        }
      }
      else if(*fmt=='u'){
        //memset(nums,'0',100);
        uint32_t valu=va_arg(ap,uint32_t);
        int32_t k=0;
        while(valu){
           k++;
           nums[k]=valu%10+'0';
           valu/=10;
        }
        if(k<width&&flag==1){
          len=width-k;
          while(len--){
            *str++='0';
          }
        }
        while(k){
          *str=nums[k];
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
    
  }
  *str='\0';
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

#endif*/
#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

const char lower_dict[] = "0123456789abcdef";
const char upper_dict[] = "0123456789ABCDEF";

// 借鉴了 https://github.com/atgreen/FreeRTOS/blob/master/Demo/CORTEX_STM32F103_Primer_GCC/printf-stdarg.c
static inline int _printc(char **out, char c) {
  if (out) {
    **out = c;
    (*out)++;
  } else {
    putch(c);
  }
  return 1;
}

static inline int _prints(char **out, const char *s) {
  int cnt = 0;
  while (*s != '\0') {
    cnt += _printc(out, *(s++));
  }
  return cnt;
}

static inline int _printi(char **out, int num, int base, int use_upper, int sign) {
  assert(base <= 16 && base >= 2);

  const int buf_sz = 51;
  char buf[buf_sz];
  buf[buf_sz - 1] = '\0';
  const char *dict = use_upper ? upper_dict : lower_dict;

  if (num == 0) {
    return _printc(out, dict[0]);
  }

  char *p = &buf[buf_sz - 1];
  if (!sign) {
    unsigned int n = *(unsigned int *)&num;
    while (n != 0) {
      p--;
      *p = dict[n % base];
      n /= base;
    }
  } else {
    int is_neg = 0;
    if (num < 0) {
      num = -num;
      is_neg = 1;
    }

    while (num != 0) {
      p--;
      *p = dict[num % base];
      num /= base;
    }

    if (is_neg) {
      p--;
      *p = '-';
    }
  }

  return _prints(out, p);
}

static inline int _print(char **out, const char *fmt, va_list ap) {
  const char *p = fmt;
  int cnt = 0;
  while (*p) {
    if (*p == '%') {
      p++;
      switch (*p) {
        case 'd':
          cnt += _printi(out, va_arg(ap, int), 10, 0, 1);
          break;
        case 'u':
          cnt += _printi(out, va_arg(ap, int), 10, 0, 0);
          break;
        case 'x':
          cnt += _printi(out, va_arg(ap, int), 16, 0, 0);
          break;
        case 'X':
          cnt += _printi(out, va_arg(ap, int), 16, 1, 0);
          break;
        case 's':
          cnt += _prints(out, va_arg(ap, char *));
          break;
        default:
          break;
      }
    } else {
      cnt += _printc(out, *p);
    }
    p++;
  }
  if (out)
    **out = '\0';
  return cnt;
}

int printf(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int cnt = _print(NULL, fmt, ap);
  va_end(ap);
  return cnt;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  return _print(&out, fmt, ap);
}

int sprintf(char *out, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int cnt = _print(&out, fmt, ap);
  va_end(ap);
  return cnt;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

#endif