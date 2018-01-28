CXXFLAGS = -O0 -g  -Wall -I ../.. -pthread
LDFLAGS = -lpthread

#BASE_SRC = ../../muduo-master/muduo/base/Logging.cc ../../muduo-master/muduo/base/LogStream.cc ../../muduo-master/muduo/base/Thread.cc ../../muduo-master/muduo/base/Timestamp.cc

BASE_SRC = ../base/thread/Thread.cc 
LIB_SRC = ./Channel.cc ./EventLoop.cc ./Poller.cc

BINARIES = test1
all: $(BINARIES)


$(BINARIES):
	g++ $(CXXFLAGS) -o $@ $(LIB_SRC) $(BASE_SRC) $(filter %.cc,$^) $(LDFLAGS)

clean:
	rm -f $(BINARIES) core

test1: test1.cc
