
#编译参数
CXXFLAGS = -O0 -g  -Wall -I ../.. -pthread

#链接的库
LDFLAGS = -lpthread

#基层依赖
BASE_SRC = ../base/thread/Thread.cc 

#编译文件
LIB_SRC = ./Channel.cc ./EventLoop.cc ./Poller.cc

#目标程序
BINARIES = test1


all: $(BINARIES)

$(BINARIES):
	g++ $(CXXFLAGS) -o $@ $(LIB_SRC) $(BASE_SRC) $(filter %.cc,$^) $(LDFLAGS)

clean:
	rm -f $(BINARIES) core

test1: test1.cc
