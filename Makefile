CC = gcc
CFLAGS = -msse2 --std gnu99 -O0 -Wall
EXECUTABLE = \
	main time_naive time_sse time_sse_prefetch

#Debug version.
ifeq ($(strip $(DEBUG)),1)
CFLAGS += -DDEBUG -g
endif

%.o:  %.c
	$(CC) -c $(CFLAGS) $< -o $@

default: impl.o time_test.c main.c
	$(CC) $(CFLAGS) -o main main.c impl.o
	$(CC) $(CFLAGS) impl.o time_test.c -DNAIVE -o time_naive
	$(CC) $(CFLAGS) impl.o time_test.c -DSSE -o time_sse
	$(CC) $(CFLAGS) impl.o time_test.c -DSSE_PREFETCH -o time_sse_prefetch

time: default
	time ./time_naive
	time ./time_sse
	time ./time_sse_prefetch

clean:
	$(RM) $(EXECUTABLE)
