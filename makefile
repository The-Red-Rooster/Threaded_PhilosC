all: assignment6

assignment6: random.o assignment6.o
			gcc assignment6.o random.o -o assignment6 -lm -lpthread

random.o: random.c random.h
			gcc -c random.c -o random.o

assignment6.o: assignment6.c
			gcc -c assignment6.c -o assignment6.o

run:
			./assignment6

clean:
			rm assignment6.o random.o assignment6
