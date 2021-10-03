#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536] = {};
int pos;
static char code_buf[65536 + 128] = {}; // a little larger than `buf`
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

uint32_t choose(int s){
  return rand()%s;
}
void gen_num(){
  uint32_t num=choose(9)+1;
  /*int n[10];
  int k=0;
   while(num!=0){
  
  n[k]=num%10;
  k++;
  num/=10;
  }
  for(int i=k-1;i>=0;i--){
    pos++;
    buf[pos]=n[i]+'0';
  }*/
  pos++;
    buf[pos]=num+'0';
}
void gen(char a){
   pos++;
   buf[pos]=a;
}
void gen_rand_op(){
    char a;
    switch(choose(4)){
    case 0: a='+';break;
    case 1: a='-';break;
    case 2: a='*';break;
    default: a='/';break;
    }
    pos++;
    buf[pos]=a;
}
static void gen_rand_expr() {
  if(pos>60000) {gen_num();}
  else{
  switch(choose(3)){
    case 0: gen_rand_expr();gen_rand_op();gen_rand_expr();break;
    case 1: gen('(');gen_rand_expr();gen(')');break;
    default :gen_num();break;
}
}
}

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    pos=-1;
    memset(buf,'\0',sizeof(buf));                     //清空buf的内容
    gen_rand_expr();

    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -o /tmp/.expr");           //执行shell中的命令
    if (ret != 0) continue;

    fp = popen("/tmp/.expr", "r");                                //执行shell命令，并读取此命令的返回值
    assert(fp != NULL);

    int result;
    fscanf(fp, "%d", &result);
    pclose(fp);                                                     //关闭popen函数产生的管道

    printf("%u %s\n", result, buf);
  }
  return 0;
}
