#include "leveldb/comparator.h"
#include "leveldb/iterator.h"
#include "table/iterator_wrapper.h"
#include "table/liuiterator.h"

namespace leveldb {

namespace {

class LiuIterator : public Iterator {
 public:
  LiuIterator(const Comparator* comparator, LiuCacheList list,int n)
       :comparator_(comparator),
        children_(new IteratorWrapper[n]),
        n_(n),
        current_(NULL),
        currenti_(0){
  LiuCache p = list->head;
  LiuCache pnext;
    for (int i = 0; i < n_; i++) {
      pnext = p->next;
      children_[i].Set(p->biter);
      p = pnext;
    }
  }

  virtual ~LiuIterator() {
    delete[] children_;
  }

  virtual bool Valid() const {
    return (current_ != NULL);
  }

  virtual void SeekToFirst() {
    children_[0].SeekToFirst();
    current_ = &children_[0];
    currenti_ = 0;
  } 

  virtual void SeekToLast() {
     children_[n_-1].SeekToLast();
     current_ = &children_[n_-1];
     currenti_ = n_-1;
  }

  virtual void Seek(const Slice& target) {
  for (int i = 0; i < n_; i++) {
      children_[i].Seek(target);
    }
  }

  virtual void Next() {
    assert(Valid());
      IteratorWrapper* child = &children_[currenti_];
      current_->Next();
      if(!current_->Valid())
      {
        if(currenti_+1>=n_)
        {
          current_ = NULL;
          return;
        }
        else
        {
          current_ = &children_[currenti_+1];
          currenti_++;
          current_->SeekToFirst();
          return;
        }
      }
      else
      {
        return;
      }
    }  
  virtual void Prev() {
      IteratorWrapper* child = &children_[currenti_];
      current_->Prev();
      if(!current_->Valid())
      {
        if(currenti_<=0)
        {
          current_ = NULL;
          return;
        }
        else
        {
          current_ = &children_[currenti_-1];
          currenti_--;
          current_->SeekToLast();
          return;
        }
      }
      else
      {
        return;
      }
    }
  virtual Slice key() const {
    assert(Valid());
    return current_->key();
  }

  virtual Slice value() const {
    assert(Valid());
    return current_->value();
  }

  virtual Status status() const {
    Status status;
    for (int i = 0; i < n_; i++) {
      status = children_[i].status();
      if (!status.ok()) {
        break;
      }
    }
    return status;
  }

  const Comparator* comparator_;
  IteratorWrapper* children_;
  int n_;
  IteratorWrapper* current_;
  int currenti_;
};

}
Iterator* NewLiuIterator(const Comparator* cmp, LiuCacheList list,int n) {
  assert(n >= 0);
  if (n == 0) {
    return NewEmptyIterator();
  } else if (n == 1) {
    return list->head->biter;
  } else {
    return new LiuIterator(cmp, list, n);
  }
}

}  // namespace leveldb

