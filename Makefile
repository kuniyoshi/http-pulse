.SUFFIXES=.c .out
CC=gcc -g -O0 -Wall
FRAMEWORKS=-framework OpenAL

all: beep.out sin.out sam.out split.out web.out atoi.out multi.out play.out

.c.out:
	$(CC) $< $(FRAMEWORKS) -o $(basename $<).out

clean:
	rm -R *.out *.out.*
