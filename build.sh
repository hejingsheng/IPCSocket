echo "build start"

g++ main.cc libIPC.a -o main -std=c++11 -lpthread

echo "build end"
