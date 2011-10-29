CC = gcc -g -O0 -Wall
FRAMEWORKS = -framework OpenAL
all: sam

sam:
	$(CC) $(FRAMEWORKS) sam.c

