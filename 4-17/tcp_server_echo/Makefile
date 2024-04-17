.PHONY:all
all:tcpserver tcpclient

tcpserver:main.cc
	g++ -o $@ $^ -std=c++11
tcpclient:TcpClinet.cc
	g++ -o $@ $^ -std=c++11

.PHONY:clean
clean:
	rm -f tcpserver tcpclient