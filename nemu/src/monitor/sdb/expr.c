#include <isa.h>
#include <memory/paddr.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

enum {
  TK_NOTYPE = 256, TK_EQ,TK_HEX,TK_NUM,TK_NOTEQ,TK_AND,TK_REG,DEREF,NEG,

  /* TODO: Add more token types */

};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"\\+", '+'},         // plus
  {"==", TK_EQ},        // equal
  {"\\-",'-'},          //减
  {"\\*",'*'},          //乘
  {"\\/",'/'},          //除
  {"\\(",'('},          //左括号 
  {"\\)",')'},          //右括号  
  {"0x[0-9,a-f]+",TK_HEX}, //十六进制数
  {"[0-9]+",TK_NUM},    //十进制整数
  {"!=",TK_NOTEQ},      //NOT equal
  {"&&",TK_AND},        //与
  {"\\$[0-9a-z]{2,3}",TK_REG},  //寄存器(在这我没有考虑名为$0的寄存器，一是麻烦，而是它里面的内容恒为零)

};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[320] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {              //模式匹配函数regexec()
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;
        


        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
        switch (rules[i].token_type) {
	  case TK_NOTYPE:
	    break;
          default: 
            tokens[nr_token].type=rules[i].token_type;
            //memset(tokens[nr_token].str,'\0',sizeof(tokens[nr_token].str));      //这句话很关键，一定要注意strncpy函数的坑爹之处
	    strncpy(tokens[nr_token].str,substr_start,substr_len);
            nr_token++;
	    break;

        }

        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}
word_t trans(char *s){                            //进制转换函数
  uint32_t n=0;
  int pos;
  if(s[0]=='0'&&s[1]=='x') pos=2;
  else pos=0;
  while(s[pos]!='\0'){
  if(s[pos]>'9') n=16*n+(s[pos]-'a')+10;
  else n=16*n+(s[pos]-'0');
  pos++;
  }
  return n;
}
bool check_parentheses(int p,int q){                   //括号匹配函数 
  if(tokens[p].type!='('||tokens[q].type!=')') return false;
  else {
    int cout=0;
    for(int i=p+1;i<q;i++){
      if(tokens[i].type=='(') cout++;  
      else if(tokens[i].type==')'){
        cout--;
	if(cout<0) return false;       
        }
      else {continue;}
      }
     return true;
  }
}
int find_main_operator(int p,int q){          //寻找主操作符

  int ans=0;
  int k=q;//表示符号的位置
  int pri=10;//代表优先级
  while(k>=p){
    if(tokens[k].type==')'){
      int t=k;
      while(!check_parentheses(k,t)){
        k--;
        }
	
      }
    else if(tokens[k].type==TK_EQ||tokens[k].type==TK_NOTEQ){
         int l=3;
         if(l<pri){
           pri=l;
           ans=k;
           }
 
         }

    else if(tokens[k].type=='+'||tokens[k].type=='-'){
        int l=4;
	if(l<pri){
	  pri=l;
	  ans=k;
	  }

	}
   else if(tokens[k].type=='*'||tokens[k].type=='/'){
         int l=5;
         if(l<pri){
           pri=l;
           ans=k;
           }
        }
  else if(tokens[k].type==DEREF||tokens[k].type==NEG){
        int l=6;
        if(l<=pri){
          pri=l;
          ans=k;
          }
        }
   k--;
    }
    return ans;
}
word_t eval(int p,int q){
  if(p>q){
    assert(0);                             //这个地方（）里是0还是1还要在思考一下？
  }
  else if(p==q){                          //此处应进行更加详细的分类，区别十进制，十六进制，寄存器的值
    if(tokens[p].type==TK_NUM){
      word_t val;
      sscanf(tokens[p].str,"%d",&val);  
      return val;
    }
    else if(tokens[p].type==TK_HEX){
      if(tokens[p-1].type==DEREF){
        word_t address;
        sscanf(tokens[p].str,"%x",&address);
        return address;    
      }
      else {
        return trans(tokens[p].str);
      }
    }
    else if(tokens[p].type==TK_REG){          //读取寄存器的值
      char s[10];
      int pos=1;
      while(tokens[p].str[pos]!='\0'){
        s[pos-1]=tokens[p].str[pos];
        pos++; 
      }
      s[pos-1]='\0';
      bool success;
      return   isa_reg_str2val(s,&success);
    }
    else assert(0);//if(tokens[p].type==NEG) {return }
  }
  else if(check_parentheses(p,q)==true){
    return eval(p+1,q-1);
  }
  else {
    int op=find_main_operator(p,q);
    word_t val1=eval(p,op-1);;
    word_t val2=eval(op+1,q);
    switch(tokens[op].type){
      case '+': return val1+val2;
      case '-': return val1-val2;
      case '*': return val1*val2;
      case '/': return val1/val2;
      case DEREF : return paddr_read(val2,4);
      case NEG : return -val2;
      case TK_EQ: return val1==val2;
      case TK_NOTEQ: return val1!=val2;
      //add more cases
      default: assert(0);
    }
  }
  
}
word_t expr(char *e,bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  for(int i=0;i<nr_token;i++){
  if(tokens[i].type=='*'&&(i==0||(tokens[i-1].type!=TK_NUM&&tokens[i-1].type!=TK_HEX&&tokens[i-1].type!=TK_REG&&tokens[i-1].type!=')'))){
  tokens[i].type=DEREF;
}  
  if(tokens[i].type=='-'&&(i==0||(tokens[i-1].type!=TK_NUM&&tokens[i-1].type!=TK_HEX&&tokens[i-1].type!=TK_REG&&tokens[i-1].type!=')'))){
  tokens[i].type=NEG;//Log("%d",i);
}
}  



  *success = true;
  return eval(0,nr_token-1);
}
