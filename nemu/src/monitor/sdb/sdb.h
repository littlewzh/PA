#ifndef __SDB_H__
#define __SDB_H__

#include <common.h>
typedef struct watchpoint WP;
void free_wp(WP *wp);
word_t expr(char *e,bool *success);
WP* new_wp(char *s);
int test();
#endif
