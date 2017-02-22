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
  int loop =4000000;
  int datalen = 1000;
  std::string data[datalen];
  leveldb::DB *db;
  leveldb::Options options;
  int i;
  options.create_if_missing=true;
  

  leveldb::Status status = leveldb::DB::Open(options,"./testdb",&db);
  assert(status.ok());
  std::string value;
  leveldb::WriteOptions write_options;
  int count=0;
  clock_t starts,ends,dels,compacts,before,after;
  starts=clock();
  for(i=0;i<loop;i+=2)
  {
    srand(i);
    char name[4000+1];
    char key[10];
    rand_str(name,MAXSIZE);
    rand_str(key,KEYLEN);
    std::string temp = name;
    std::string s = key;
    leveldb::Status status=db->Put(write_options,s,temp);
    if (!status.ok()) cerr << status.ToString() << endl;
  }
  db->CompactRange(NULL,NULL);
  delete db;
  dels = clock();
  printf("time %d\n",dels-starts);
  return 0;
}
