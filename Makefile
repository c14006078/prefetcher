CFLAGS = -msse2 --std gnu99 -O0 -Wall

#Debug version.
ifeq ($(strip $(DEBUG)),1)
CFLAGS += -DDEBUG -g
endif

all: main.c
	$(CC) $(CFLAGS) -o main main.c

clean:
	$(RM) main
