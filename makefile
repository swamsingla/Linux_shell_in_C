CC = gcc
CFLAGS = -Wall -Wextra

SRCS = main.c spec1.c reveal.c proclore.c seek.c hop.c log.c helper.c \
       pipe.c signal.c neonate.c activities.c fgbg.c ping.c spec9.c iman.c

OBJS = $(SRCS:.c=.o)
TARGET = shell

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) command_log.txt
