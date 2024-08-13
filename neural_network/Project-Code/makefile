# Makefile for building the neural network program

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -O3 -fopenmp

# Linker flags
LDFLAGS = -fopenmp

# Source files for train target
TRAIN_SRCS = nn_struc.c nn_algo.c dataset.c neural_network.c

# Object files for train target
TRAIN_OBJS = $(TRAIN_SRCS:.c=.o)

# Source files for test target
TEST_SRCS = nn_struc.c nn_algo.c dataset.c test.c

# Object files for test target
TEST_OBJS = $(TEST_SRCS:.c=.o)

# Default target to build both train and test executables
all: neural_network 

# Rule to link object files to create the train executable
train: $(TRAIN_OBJS)
	$(CC) $(TRAIN_OBJS) -o train $(LDFLAGS) -lm

# Rule to link object files to create the test executable
test: $(TEST_OBJS)
	$(CC) $(TEST_OBJS) -o test $(LDFLAGS) -lm

# Rule to compile source files into object files for train
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(TRAIN_OBJS) $(TEST_OBJS) train test test.txt neural_network.txt

# Phony targets (they are not actual files)
.PHONY: all clean
