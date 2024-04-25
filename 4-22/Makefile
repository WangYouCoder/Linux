.PHONY:all
all:tcpserver tcpclient

tcpserver:TcpServerMain.cc
	g++ -o $@ $^ -std=c++11 -lpthread -ljsoncpp
tcpclient:TcpClientMain.cc
	g++ -o $@ $^ -std=c++11 -lpthread -ljsoncpp

.PHONT:clean
clean:
	rm -f tcpserver tcpclient
