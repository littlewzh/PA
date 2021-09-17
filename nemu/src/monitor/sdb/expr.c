#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

enum {
  TK_NOTYPE = 256, TK_EQ,TK_NUM,

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
  {"[0-9]+",TK_NUM},    //十进制整数
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

static Token tokens[64] __attribute__((used)) = {};
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
int find_main_operator(int p,int q){
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
    else if(tokens[k].type=='+'||tokens[k].type=='-'){
        int l=1;
	if(l<pri){
	  pri=l;
	  ans=k;
	  }

	}
   else if(tokens[k].type=='*'||tokens[k].type=='/'){
         int l=2;
         if(l<pri){
           pri=l;
           ans=k;
           }
        }
   k--;
    }
    return ans;
}
//寻找主操作符
word_t eval(int p,int q){
  if(p>q){
    assert(0);
  }
  else if(p==q){
    uint32_t val;
    sscanf(tokens[p].str,"%d",&val);
    return val;
  }
  else if(check_parentheses(p,q)==true){
    return eval(p+1,q-1);
  }
  else {
    int op=find_main_operator(p,q);
    word_t val1=eval(p,op-1);
    word_t val2=eval(op+1,q);
    switch(tokens[op].type){
      case '+': return val1+val2;
      case '-': return val1-val2;
      case '*': return val1*val2;
      case '/': return val1/val2;
      //add more cases
    }
  }
  return 0;
}
word_t expr(char *e,bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  
  else {
    *success = true;
    printf("%d\n",eval(0,nr_token-1));
    return 0;
    }
}
