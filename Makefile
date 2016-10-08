CFLAGS = -msse2 --std gnu99 -O0 -Wall -g

all: main.c
	$(CC) $(CFLAGS) -o main main.c

clean:
	$(RM) main
