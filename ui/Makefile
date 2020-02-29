CC=c99
TARGET=build/application.out
CFLAGS= -lglfw -lGLEW -lGL -lGLU -lm -lcglm -g
OFILES= $(shell find . -type f -name *.c | sed 's/\.c/\.o/g')

default: $(TARGET)

$(TARGET): $(OFILES)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $^

clean:
	rm -f *.o

run: default
	$(TARGET)
