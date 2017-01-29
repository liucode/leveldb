make clean
make -j8
./install.sh
g++ -O0 -g3 -rdynamic ./mytest.cc -o mytest -lleveldb
rm -r testdb/
./mytest

