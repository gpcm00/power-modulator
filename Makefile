CC ?= $(CROSS_COMPILE)gcc
CFLAGS ?= -g -Wall -Werror
OBJ := test.o

all: $(OBJ)
	$(CC) $(OBJ) -o test
	
%.o : %.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	$(RM) -f test $(OBJ)
