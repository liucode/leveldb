#include <unistd.h>
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <string>
#include <leveldb/db.h>
#include <time.h>
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
  int loop =100000;
  std::string data[10];
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
  write_options.sync = true;
  int count=0;
  clock_t starts,ends,compacts,before,after;
  starts=clock();
  
  for(i=0;i<loop;i+=2)
  {
    srand(i);
    char name[4000+1];
    int len = (rand() % (4000-1))+ 1;
    rand_str(name,len);
    std::string temp = name;
    std::string s = to_string(len);
    s.append(temp);
    leveldb::Status status=db->Put(write_options,s,s);
    if(i<10)
    data[i] = s;
    if (!status.ok()) cerr << status.ToString() << endl;
  }

  for(i=2;i<5;i+=2)
  {
    srand(i);
    int len = (rand() % (4000-1))+ 1;
    char name[4000+1];
    rand_str(name,len);
    std::string temp = name;
    std::string s = to_string(len);
    s = data[i];
    leveldb::Status status=db->Delete(leveldb::WriteOptions(),s);
    if (!status.ok()) cerr << status.ToString() << endl;
  }
  
  printf("start Compact\n");
  compacts=clock();
  db->CompactRange(NULL,NULL);
  before = clock();
 
  for(i=3;i<loop;i+=1000)
  {
     srand(i);
     char name[4000+1];
     int len = (rand() % (4000-1))+ 1;
     rand_str(name,len);
     std::string temp = name;
     std::string s = to_string(len);
     s.append(temp);
     if(i<10)
     data[i] = s;
     leveldb::Status status=db->Put(write_options,s,s);
     if (!status.ok()) cerr << status.ToString() << endl;
   }
  
  after = clock();
db->CompactRange(NULL,NULL);
for(i=1;i<5;i++){
   leveldb::Status status=db->Get(leveldb::ReadOptions(),data[i],&value);
   ends = clock();
      cout<<data[i]<<endl;
    if(status.ok())
      {
        cout<<"ok get"<<endl;
        if(value.at(0)=='l')
        {
          count++;
        }
      }
   else
     cout<<status.ToString()<<endl;
 }
 db->CompactRange(NULL,NULL);
 cout<<compacts-starts<<":"<<before-compacts<<":"<<after-before<<":"<<ends-after<<":"<<count<<endl;
  delete db;
  return 0;
}
