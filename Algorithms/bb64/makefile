##The name of the main executable
TARGET=b64

# The objects that make up the main executables
OBJS=d_array.o encoder.o b64.o


# Flags 
OPTIMZE=-O3
CFLAGS=$(OPTIMIZE) -Wall
LDFLAGS= -lm
CC=gcc




# The first target (all) is allways the default target
all: build test

# Our buld target depends on the real target
.PHONY: build
build: $(TARGET)

# Our target is built up from the objects
$(TARGET): $(OBJS)
	$(CC) -Wall -o $(TARGET) $(OBJS) $(LDFLAGS) 


#Our test is built from its object 
.PHONY: test
encoder_test:encoder_test.o d_array.o encoder.o 
	$(CC) -Wall -o encoder_test d_array.o encoder.o encoder_test.o $(LDFLAGS)

encoder_test.o: encoder_test.c
	gcc  -Wall   -c -o encoder_test.o encoder_test.c


# Cleanup all generated files
clean:
	rm -Rf  $(OBJS) $(TARGET) encoder_test.o encoder_test
