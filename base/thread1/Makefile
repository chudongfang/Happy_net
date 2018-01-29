
#编译参数
CXXFLAGS = -O0 -g -std=c++11 -Wall -I ../.. -pthread

#链接的库
LDFLAGS = -lpthread

#基层依赖
BASE_SRC =  

#编译文件
LIB_SRC = ./Thread.h

#目标程序
BINARIES = test1


all: $(BINARIES)

$(BINARIES):
	g++ $(CXXFLAGS) -o $@ $(LIB_SRC) $(BASE_SRC) $(filter %.cc,$^) $(LDFLAGS)

clean:
	rm -f $(BINARIES) core

test1: test1.cc
