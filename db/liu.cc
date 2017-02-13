#include "malloc.h"
#include "leveldb/iterator.h"
#include "liu.h"
#include "leveldb/slice.h"
#include "db/dbformat.h"
#include "map"
namespace leveldb{

std::map<int,LiuCacheList> filemata;
std::map<std::string,LiuCache> largemap; 
Slice *lastkey;
Slice *inputlast;
LiuCacheList overlist1 = NULL;
LiuCacheList overlist2 = NULL;
LiuCacheList memlist = NULL;
LiuCacheList filelist = NULL;
LiuCacheList outlist = NULL;
Slice *smallblock = NULL;
int lastflag = 0;
int  liublocksize = 0;
bool firstflag = true;
bool flushflag = false;
int sizecache = 0;
int sizetemp = 0;
bool smallflag = false;


void AddLargemap(LiuCache p)
{
    if(overlist1->tail==NULL&&overlist2->tail == NULL)
    {
            return;
    }
    else if(overlist1->tail!=NULL&&overlist2->tail == NULL)
    {
      std::string so = std::string(overlist1->tail->large->data(),overlist1->tail->large->size()-8);
      largemap.insert(std::pair<std::string,LiuCache>(so,p));
    }
    else if(overlist1->tail==NULL&&overlist2->tail != NULL)
    {
        std::string so = std::string(overlist2->tail->large->data(),overlist2->tail->large->size()-8);
        largemap.insert(std::pair<std::string,LiuCache>(so,p));
    }
    else
    {
        if(overlist1->tail->large->compare(*overlist2->tail->large)>0)
        {
             std::string so = std::string(overlist1->tail->large->data(),overlist1->tail->large->size()-8);
            largemap.insert(std::pair<std::string,LiuCache>(so,p));
        }
        else
        {
          std::string so = std::string(overlist2->tail->large->data(),overlist2->tail->large->size()-8);
          largemap.insert(std::pair<std::string,LiuCache>(so,p));
        }
    }
}

void CheckList(LiuCacheList list)
{
    LiuCache p = list->head;
    LiuCache pnext=p->next;;
    while(pnext)
    {
        if(p->large->compare(*pnext->small)>0)
              printf("error check\n");
        p = pnext;
        pnext = p->next;
    }
}

void WriteList(LiuCacheList list,FILE *fp)
{
    LiuCache p = list->head;
    while(p)
    {
      std::string small = p->small->data();
      std::string large = p->large->data();
      fprintf(fp,"%c%c%c%c%c%c and %c%c%c%c%c%c\n",small[0],small[1],small[2],small[3],small[4],small[5],large[0],large[1],large[2],large[3],large[4],large[5]);
      p = p->next;
    }
    fclose(fp);
}
void PrintSize(LiuCacheList list)
{
      int nn=0;
      int num=0;
      LiuCache p = list->head;
      while(p)
      {
          if(p->biter==NULL)
              nn++;
          num++;
          p = p->next;
      }
        printf("listnum:%d %d\n",num,nn);
}
void PrintList(LiuCacheList list)
{
    LiuCache p = list->head;
    while(p)
    {
      std::string out = p->small->data();
      p = p->next;
    }  
}
void DeleteHead(LiuCacheList list)
{
  
  LiuCache p = list->head;
  while(p)
  {
    LiuCache q = p;
    p = p->next;
    free(q);
  }
  free(list);
}

LiuCache InitLiuCache(Iterator* iter)
{
  LiuCache newnode = (LiuCache)malloc(sizeof(struct liucache));
  newnode->biter = iter;
  iter->SeekToFirst();
  char *str = (char*)malloc(iter->key().size());
  Slice tempstr = iter->key();
  memcpy(str,tempstr.data(),iter->key().size());
  Slice* slices = new Slice(str,iter->key().size());
  newnode->small = slices;
  iter->SeekToLast();
  char *lstr = (char*)malloc(iter->key().size());  
  Slice ltempstr = iter->key();
  memcpy(lstr,ltempstr.data(),iter->key().size());
  Slice* slicl = new Slice(lstr,iter->key().size());
  newnode->large = slicl;
  newnode->next = NULL;
  return newnode;
}
void AddLiuCacheshen(LiuCacheList list,LiuCache newnode)
{
    if(newnode == NULL)
    {
        return;
    }
    if(list->head ==NULL)
    {
       list->head = newnode;
       LiuCache p = newnode;
       while(p->next)
      {
        p = p->next;
      }
      list->tail = p;
    }
    else
    {
      list->tail->next = newnode;
          while(newnode->next)
         {
           newnode = newnode->next;
         }
        list->tail = newnode;
    }
}
void AddLiuCacheList(LiuCacheList list,LiuCache newnode,int p)
{
  if(list->head ==NULL)
  {
    list->head = newnode;
    list->tail = newnode;
    newnode->next = NULL;
  }
  else
  {
    if(p==1)
    {if(newnode->small->compare(*(list->tail->large))<=0)
    {
        printf("error add\n");
        exit(0);
    }}
    list->tail->next = newnode;
    list->tail = newnode;
    newnode->next = NULL;
  }
}
void AddLiuIterList(LiuCacheList list,Iterator* iter)
{
  LiuCache newnode ;
  newnode = InitLiuCache(iter);
  if(list->head ==NULL)
  {
    list->head = newnode;
    list->tail = newnode;
    list->tail->next = NULL;
  }
  else
  {
    list->tail->next = newnode;
    list->tail = newnode;
    list->tail->next = NULL;
  }
}

}
