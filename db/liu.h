#define DEBUG true
#include "malloc.h"
#include "leveldb/iterator.h"
namespace leveldb {

#ifndef FUN_H
#define FUN_H
typedef struct liucache *LiuCache;

struct liucache
{
  Iterator * biter;
  LiuCache next;
  int flag;
  int number;
};
#endif
extern int liublocksize;
extern LiuCache head;
extern bool firstflag;

extern bool flushflag;
extern int sizecache;
extern LiuCache temphead;
extern int sizetemp;
extern bool smallflag;

void PrintHead(LiuCache temphead);
void DeleteHead(LiuCache head);
void InitLiuCache(LiuCache head);
void AddLiuCache(LiuCache head,Iterator* iter);
void CheckValid();
}
