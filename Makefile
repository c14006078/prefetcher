CC = gcc
CFLAGS = -msse2 --std gnu99 -O0 -Wall
EXECUTABLE = \
	main time_naive time_sse time_sse_prefetch \
    time_sse_align time_sse_prefetch

#Debug version.
ifeq ($(strip $(DEBUG)),1)
CFLAGS += -DDEBUG -g
endif

%.o:  %.c
	$(CC) -c $(CFLAGS) $< -o $@

all: default

default: impl.o time_test.c main.c
	$(CC) $(CFLAGS) -o main main.c impl.o
	$(CC) $(CFLAGS) impl.o time_test.c -DNAIVE -o time_naive
	$(CC) $(CFLAGS) impl.o time_test.c -DSSE -o time_sse
	$(CC) $(CFLAGS) impl.o time_test.c -DSSE_PREFETCH -o time_sse_prefetch
#	$(CC) $(CFLAGS) impl.o time_test.c -DSSE -DALIGN=16 -o time_sse_align
#	$(CC) $(CFLAGS) impl.o time_test.c -DSSE_PREFETCH -DALIGN=16 -o time_sse_prefetch_align


time: default
	time ./time_naive
	time ./time_sse
	time ./time_sse_prefetch
	time ./time_sse_align
	time ./time_sse_prefetch_align

cache-test: default
	perf stat --repeat 100 \
	 -e cache-misses,cache-references,instructions,cycles \
	  ./time_naive > /dev/null
	perf stat --repeat 100 \
	 -e cache-misses,cache-references,instructions,cycles \
	  ./time_sse > /dev/null
	perf stat --repeat 100 \
	 -e cache-misses,cache-references,instructions,cycles \
	  ./time_sse_prefetch > /dev/null
#	perf stat --repeat 100 \
	 -e cache-misses,cache-references,instructions,cycles \
	  ./time_sse_align > /dev/null
#	perf stat --repeat 100 \
	 -e cache-misses,cache-references,instructions,cycles \
	  ./time_sse_prefetch_align > /dev/null

L1-cache-test: default
	perf stat --repeat 100 \
	 -e L1-dcache-load-misses,L1-dcache-store-misses,L1-dcache-prefetch-misses,L1-icache-load-misses \
	  ./time_naive > /dev/null
	perf stat --repeat 100 \
	 -e L1-dcache-load-misses,L1-dcache-store-misses,L1-dcache-prefetch-misses,L1-icache-load-misses \
	  ./time_sse > /dev/null
	perf stat --repeat 100 \
	 -e L1-dcache-load-misses,L1-dcache-store-misses,L1-dcache-prefetch-misses,L1-icache-load-misses \
	  ./time_sse_prefetch > /dev/null
#	perf stat --repeat 100 \
	 -e L1-dcache-load-misses,L1-dcache-store-misses,L1-dcache-prefetch-misses,L1-icache-load-misses \
	  ./time_sse_align > /dev/null
#	perf stat --repeat 100 \
	 -e L1-dcache-load-misses,L1-dcache-store-misses,L1-dcache-prefetch-misses,L1-icache-load-misses \
	  ./time_sse_prefetch_align > /dev/null

clean:
	$(RM) $(EXECUTABLE) *.o
