
#include "malloc.h"
#include "leveldb/iterator.h"
#include "liu.h"
#include "leveldb/slice.h"
#include "db/dbformat.h"

namespace leveldb{

LiuCache temphead =NULL;
LiuCache head;
int  liublocksize = 0;
bool firstflag = true;
bool flushflag = false;
int sizecache = 0;
int sizetemp = 0;
bool smallflag = false;
void InitLiuCache(LiuCache head)
{
  head = (LiuCache)malloc(sizeof(LiuCache));
  head->next = NULL;
}



void PrintHead(LiuCache temphead)
{
    LiuCache p = temphead;
    int count = 0;
    while(p->next)
    {
        count++;
        p = p->next;
        p->biter->SeekToFirst();
        printf("count:%d",count);
        std::string str = p->biter->key().ToString();
        printf("smallest:%c%c%c%c\n",str[0],str[1],str[2],str[3]);
        p->biter->SeekToLast();
        str = p->biter->key().ToString();
        printf("largest:%c%c%c%c\n",str[0],str[1],str[2],str[3]);
    }
}
void DeleteHead(LiuCache temphead)

{
  LiuCache p = temphead;
  p = p->next;
  while(p)
  {
    LiuCache q = p;
    p = p->next;
    free(q);
  }
  temphead->next = NULL;
}
void AddLiuCache(LiuCache head,Iterator* iter)
{
  LiuCache p = head;
  LiuCache pnext = p->next;
  LiuCache newnode = (LiuCache)malloc(sizeof(struct liucache));
  p->next = newnode;
  newnode->biter = iter;
  newnode->next = pnext;

}

void CheckValid()
{
      LiuCache p = head;
    while(p->next)
    {
      p = p->next;
      //printf("%s\n",p->biter->key().ToString().c_str());
      bool vb = p->biter->Valid();
    }
}

}
