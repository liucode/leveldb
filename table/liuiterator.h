#include <db/liu.h>
#ifndef STORAGE_LIU_H_
#define STORAGE_LIU_MERGER_H_
namespace leveldb {

class Comparator;

extern Iterator* NewLiuIterator(const Comparator* comparator, LiuCacheList list,int n);

}  // namespace leveldb
#endif 
