#include <unistd.h>
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <string>
#include <leveldb/db.h>
#include <time.h>
using namespace std;
#define MAXSIZE 1000
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
  int loop =2000000;
  std::string data[1000];
  leveldb::DB *db;
  leveldb::Options options;
  int i;
  options.create_if_missing=true;
  leveldb::Status status = leveldb::DB::Open(options,"./testdb",&db);
  assert(status.ok());
  std::string value;
  leveldb::WriteOptions write_options;
  int count=0;
  clock_t starts,ends,dels;
  dels = clock();
  FILE *fp = fopen("number","a+");
  for(i=0;i<loop;i+=2)
  {
    srand(i);
    char key[10+1];
    char name[1000+1];
    rand_str(name,MAXSIZE);
    std::string temp = name;
    rand_str(key,10);
    std::string s = key;
    leveldb::Status status=db->Put(write_options,key,temp);
    if (!status.ok()) cerr << status.ToString() << endl;
    if(i%1000==0)
    {
      ends = clock();  
      fprintf(fp,"%d\n",ends-dels);
    }
  }
  ends = clock();
  fclose(fp);
  db->CompactRange(NULL,NULL);
  delete db;
  printf("time: %d",ends-dels);
  return 0;
}
