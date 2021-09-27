#include "sdb.h"

#define NR_WP 32

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  char str[64];                      //表达式
  uint32_t val;                       //表达式的结果
  /* TODO: Add more members if necessary */

} WP;

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

WP* new_wp(char *s){
   if(free_==NULL) assert(0);
   bool success;
   if(head==NULL){
     head=free_;
     free_=free_->next;
     head->next=NULL;
     strcpy(head->str,s);
     head->val=expr(s,&success);
     return head;
   }
   else {
     WP* p=head;
     while(p->next){
       p=p->next;
     }
     p->next=free_;
     free_=free_->next;
     p=p->next;
     p->next=NULL;
     strcpy(p->str,s);
     p->val=expr(s,&success);
     return p;
   }
}
void free_wp(WP *wp){
  WP *current;
  WP *p;
  if(wp==head) head=head->next;
  else {
    current=head;
    while((current->next!=wp)&&current!=NULL){
      current=current->next;
    }
    if(current==NULL) {printf("没有此监视点"); return;}
    else {
      p=wp->next;
      current->next=p;
    }
  }
  while(current)
  wp->val=0;
  memset(wp->str,'\0',sizeof(wp->str));
  wp->next=free_;
  free_=wp;
  free_->next=p;
  return;
}
int test(){
  WP *t=head;
  bool success;
  while(t!=NULL){
  uint32_t result;
  result=expr(t->str,&success);
  if(result==t->val) {continue;}
  else {return 0;}
  }
  return 1;
}
WP* match(int n){
  if(head==NULL) {printf("无");return 0;}
  else {
    WP *p=head;
    while(p->NO!=n&&p!=NULL){
      p=p->next;
    }
    if(p==NULL) {printf("无此监视点");return 0;}
    else {return p;}
  }
}
/* TODO: Implement the functionality of watchpoint */

