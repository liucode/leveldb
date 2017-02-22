#define BLOCKS 520
#define DEBUG true
#include "malloc.h"
#include "leveldb/iterator.h"
#include <map>
namespace leveldb {

#ifndef FUN_H
#define FUN_H
typedef struct liucache *LiuCache;

struct liucache
{
  Iterator * biter;
  LiuCache next;
  Slice *small;
  Slice *large;
};

typedef struct liucachelist *LiuCacheList;

struct liucachelist
{
  LiuCache head;
  LiuCache tail;
};


#endif
extern Slice *inputlast;
extern std::string lastkey;
extern Slice *smallblock;
extern LiuCacheList filelist;
extern std::map<int,LiuCacheList> filemata;
extern double liublocksize;
extern int liublocknum;
extern int lastflag;
extern LiuCacheList memlist;
extern bool firstflag;
extern std::map<std::string,LiuCache> largemap; 
extern bool flushflag;
extern int sizecache;
extern LiuCacheList overlist1;
extern LiuCacheList overlist2;
extern LiuCacheList outlist;
extern int sizetemp;
extern int nexttime;
extern bool smallflag;
extern int noblocknum;
void AddLargemap(LiuCache p);
int PrintSize(LiuCacheList list);
void PrintList(LiuCacheList list);
void AddLiuCacheshen(LiuCacheList list,LiuCache newnode);
void DeleteHead(LiuCache head);
void AddLiuCacheList(LiuCacheList list,LiuCache newnode,int p);
LiuCache InitLiuCache(LiuCache newnode,Iterator* iter);
void AddLiuIterList(LiuCacheList list,Iterator* iter);
void CheckList(LiuCacheList list);
void WriteList(LiuCacheList list,FILE *fp);
}
