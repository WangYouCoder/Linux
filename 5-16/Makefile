.PHONY:all
all:tcpserver tcpclient

tcpserver:TcpServerMain.cc
	g++ -o $@ $^ -std=c++11 -lpthread
tcpclient:TcpClientMain.cc
	g++ -o $@ $^ -std=c++11  

.PHONY:clean
clean:
	rm -rf tcpserver tcpclient