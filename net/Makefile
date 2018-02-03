
#编译参数
CXXFLAGS = -O0 -g -std=c++11 -Wall -I ../base/ -pthread

#链接的库
LDFLAGS = -lpthread

#依赖文件 包含头文件对应的.cc文件
BASE_SRC = ../base/thread1/CurrentThread.cc 

#编译文件
LIB_SRC = ./Channel.cc ./EventLoop.cc ./Poller.cc ./Acceptor.cc ./Socket.cc ./Socket_Base.cc ./InetAddress.cc \
		  ./TcpConnection.cc ./TcpServer.cc

#目标程序
BINARIES = test_TcpServer


all: $(BINARIES)

$(BINARIES):
	g++ $(CXXFLAGS) -o $@ $(LIB_SRC) $(BASE_SRC) $(filter %.cc,$^) $(LDFLAGS)

clean:
	rm -f $(BINARIES) core

test_TcpServer: test_TcpServer.cc
