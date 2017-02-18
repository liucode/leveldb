#include <unistd.h>
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <string>
#include <leveldb/db.h>
#include <time.h>
#define MAXSIZE  128
#define DATALEN 1000
#define KEYLEN 10
using namespace std;

char *rand_str(char *str,const int len)
{
    int i;
    for(i=0;i<len;++i)
        str[i]='A'+rand()%26;
    str[++i]='\0';
    return str;
}
 
int main(int argc,char** argv)
{
  int loop =16000000;
  int datalen = 1000;
  std::string data[datalen];
  leveldb::DB *db;
  leveldb::Options options;
  //if(argc<=1)
   // printf("参数太少！\n");
  int i;
  options.create_if_missing=true;
  //options.max_open_files = 1<<20;
  //options.compression = false;
  //options.write_buffer_size = 1<<13;
  //options.max_file_size = 1<<13;
  

  leveldb::Status status = leveldb::DB::Open(options,"./testdb",&db);
  assert(status.ok());
  std::string value;
  leveldb::WriteOptions write_options;
  //write_options.sync = true;
  int count=0;
  clock_t starts,ends,dels,compacts,before,after;
  starts=clock();
  for(i=0;i<loop;i+=2)
  {
    srand(i);
    char name[4000+1];
    char key[10];
    int len = (rand() % (4000-1))+ 1;
    rand_str(name,MAXSIZE);
    rand_str(key,KEYLEN);
    std::string temp = name;
    std::string s = key;
    if(i<datalen)
    data[i] = key;
    leveldb::Status status=db->Put(write_options,key,temp);
    if (!status.ok()) cerr << status.ToString() << endl;
  }
  dels = clock();

  db->CompactRange(NULL,NULL);
/*
  for(i=2;i<datalen;i+=100)
  {
    leveldb::Status status=db->Delete(write_options,data[i]);
    if (!status.ok()) cerr << status.ToString() << endl;
  }
  db->CompactRange(NULL,NULL);

  printf("start Compact\n");
  compacts=clock();
*/  
/*for(i=3;i<loop;i+=100000)
  {
     srand(i);
     char name[4000+1];
     int len = (rand() % (4000-1))+ 1;
     rand_str(name,MAXSIZE);
     std::string temp = name;
     std::string s = to_string(i);
     s.append(temp);
     leveldb::Status status=db->Put(write_options,s,s);
     if (!status.ok()) cerr << status.ToString() << endl;
   }
*/
  after = clock();
db->CompactRange(NULL,NULL);
ends = clock();
for(i=0;i<datalen;i+=2){
    leveldb::Status status=db->Get(leveldb::ReadOptions(),data[i],&value);
    if(status.ok())
      {
        if(data[i] == value)
        {
        }
        else
       {
      //   cout<<s<<"no"<<value<<endl;
      }  
      if(value.at(0)=='l')
        {
          count++;
        }
      }
   else
     {
      cout<<data[i]<<status.ToString()<<endl;
    }
  }
 db->CompactRange(NULL,NULL);
 cout<<dels-starts<<":"<<after-dels<<":"<<ends-after<<":"<<count<<endl;
  delete db;
  return 0;
}
