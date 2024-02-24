src=test.c
dst=mybin

$(dst):$(src)
	gcc -o $@ $^
.PHONY:clean
clean:
	rm -f $(dst)
