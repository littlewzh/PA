#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>
static char buf[65536] = {};
int pos=0;
uint32_t choose(int s){
  return rand()%s;
}
void gen_num(){
  uint32_t num=rand();
  while(num!=0){
  pos++;
  buf[pos]=num%10+'0';
  num/=10;
  }
}
void gen(char a){
  pos++;
  buf[pos]=a;
}
void gen_rand_op(){
    switch(choose(4)){
    case 0: pos++;buf[pos]='+';break;
    case 1: pos++;buf[pos]='-';break;
    case 2: pos++;buf[pos]='*';break;
    case 3: pos++;buf[pos]='/';break;
    default : break;
}
}
static void gen_rand_expr() {
  switch(choose(3)){
    case 0: gen_num();break;
    case 1: gen('(');gen_rand_expr();gen(')');break;
    default : gen_rand_expr();gen_rand_op();gen_rand_expr();break;
}
}
int main(){
  gen_rand_expr();
  pos++;
  buf[pos]='\0';
  printf("%s",buf);
  return 0;
}
