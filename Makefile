all:
	gcc *.c
debug:
	gcc -DBERT_DEBUG *.c
clean:
	rm -f *.o a.out
