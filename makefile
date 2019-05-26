
test: test.o base64.o
	gcc -o test test.o base64.o

test.o: test.c base64.h
	gcc -c test.c

base64.o: base64.c base64.h
	gcc -c base64.c

clean:
	rm -rf *.o\
	rm -rf test
