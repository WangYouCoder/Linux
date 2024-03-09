static-lib=libmymath.a

$(static-lib):Add.o Sub.o Mul.o
	ar -rc $@ $^

#mytest:*.o
	#gcc -o $@ $^
%.o:%.c
	gcc -c $<

.PHONY:output 
output:
	mkdir -p mymath_lib/include
	mkdir -p mymath_lib/lib 
	cp -f *.h mymath_lib/include
	cp -f .a mymath_lib/lib 
.PHONY:clean
clean:
	rm -rf *.o *.a mymath_lib
