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
     switch(*fmt){
       case 'd':
       {
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
        
      
      case 'x':
      {
        int32_t valx=(uintptr_t)va_arg(ap,int32_t);
        int32_t k=0;
        while(valx){
          k++;
          if(valx%16<10)  nums[k]=valx%16+'0';
          else nums[k]=valx%16+'a'-10;
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
        
      case 'X':
      {
        int32_t valx=va_arg(ap,int32_t);
        int32_t k=0;
        while(valx){
          k++;
          if(valx%16<10)  nums[k]=valx%16+'0';
          else nums[k]=valx%16+'A'-10;
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
        
      case 'u':
      { uint32_t valu=va_arg(ap,uint32_t);
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
        //memset(nums,'0',100);
       
      case 's':
      {
        char *s=va_arg(ap,char *);
        while(*s){
          *str++=*s++;
        }
      }
        
    }
    fmt++;
    
   }
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

#endif