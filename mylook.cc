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
  int loop =190000;
  std::string data[1000];
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
for(i=0;i<100;i+=2){
    srand(i);
    std::string s = to_string(i);
    leveldb::Status status=db->Get(leveldb::ReadOptions(),s,&value);
    if(status.ok())
      {
        if(s == value)
        {
            cout<<s<<"ok"<<value<<endl;
        }
        else
            cout<<s<<"no"<<value<<endl;
        if(value.at(0)=='l')
        {
          count++;
        }
      }
   else
     cout<<status.ToString()<<i<<endl;
 }
  delete db;
  return 0;
}
