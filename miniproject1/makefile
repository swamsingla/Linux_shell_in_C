# Compiler and flags
CC = gcc
CFLAGS = -std=c99

# Source files
SRCS = activities.c fgbg.c helper.c hop.c iman.c log.c main.c neonate.c ping.c pipe.c proclore.c reveal.c seek.c signal.c spec1.c spec9.c 

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
TARGET = shell

# Default rule
all: $(TARGET)

# Rule to build the target executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to build object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up the build files
clean:
	rm -f $(OBJS) $(TARGET)

# Rebuild everything
rebuild: clean all
