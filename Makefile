CC = gcc
CFLAGS = -O3 -march=native -Wall -Wextra -fopenmp -funroll-loops
LDFLAGS = -lm -fopenmp

TARGET = flashsieve
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
