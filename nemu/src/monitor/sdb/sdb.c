#include <isa.h>
#include <cpu/cpu.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "sdb.h"
#include <memory/vaddr.h>
static int is_batch_mode = false;
void init_regex();
void init_wp_pool();
//  dawdassss
/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}


static int cmd_q(char *args) {
  nemu_state.state=NEMU_QUIT;
  return -1;
}
static int cmd_si(char *args){
  char *arg=strtok(NULL," ");
  int number;
  if(arg==NULL){
     number=1;
  }
  else {
     number=atoi(arg);	 
    }
   cpu_exec(number);
   return 0;   
}
static int cmd_info(char *args){
  char *arg=strtok(NULL," ");
  if(strcmp(arg,"r")==0){
    isa_reg_display();
  }
  else if(strcmp(arg,"w")==0){
    wp_print();
  }
  else {printf("Unknown command");}
    return 0;
}
static int cmd_x(char *args){
  char *arg=strtok(NULL," ");
  int number=atoi(arg);
  arg=strtok(NULL," ");
  uint32_t address;
  bool success;
  address=expr(arg,&success);
  for(int i=0;i<number;i++){
    printf("0x%08x : ",address);
    printf("%02x ",vaddr_read(address,1));
    address++;
    printf("%02x ",vaddr_read(address,1));
    address++;
    printf("%02x ",vaddr_read(address,1));
    address++;
    printf("%02x ",vaddr_read(address,1));
    address++;
    printf("\n");
  }
  return 0;
}


static int cmd_p(char *args){
  char *arg=strtok(NULL,"\0");
  bool success;
  printf("%u\n", expr(arg,&success));
  return 0;
}
static int cmd_w(char *args){
  char *arg=strtok(NULL," ");
  new_wp(arg);
  return 0;
}
static int cmd_d(char *args){
  char *arg=strtok(NULL," ");
  int num=atoi(arg);
  WP *p=match(num);
  free_wp(p);
  return 0;
}

static int cmd_help(char *args);

static struct {
  const char *name;
  const char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  { "si","让程序单步执行N条指令后暂停执行,当N没有给出时, 缺省为1",cmd_si },
  { "info","r 打印寄存器状态,w 打印监视点信息",cmd_info },
  { "x","扫描内存",cmd_x },
  { "p","表达式求值",cmd_p },
  { "w","设置监视点",cmd_w },
  { "d","删除监视点",cmd_d },
 /* TODO: Add more commands */

};

#define NR_CMD ARRLEN(cmd_table)

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void sdb_set_batch_mode() {
  is_batch_mode = true;
}

void sdb_mainloop() {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef CONFIG_DEVICE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}

void init_sdb() {
  /* Compile the regular expressions. */
  init_regex();

  /* Initialize the watchpoint pool. */
  init_wp_pool();
}
