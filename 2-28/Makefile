.PHONY:all
all:mybin myprocess

myprocess:mytest.cc
	g++ -o $@ $^
mybin:test.c
	gcc -o $@ $^
.PHONY:clean
clean:
	rm -f myprocess mybin
