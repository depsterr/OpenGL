CC=gcc
PROJECTDIR=build/application.out
BUILDDIR=.
CFLAGS= -lglfw -lGLEW -lGL -lGLU

default: src/application.c vertexBuffer.o indexBuffer.o vertexArray.o renderer.o vertexBufferLayout.o
	$(CC) $(CFLAGS) -o $(PROJECTDIR) $^

indexBuffer.o: src/indexBuffer.c
	$(CC) $(CFLAGS) -c $^

renderer.o: src/renderer.c
	$(CC) $(CFLAGS) -c $^

vertexArray.o: src/vertexArray.c
	$(CC) $(CFLAGS) -c $^

vertexBufferLayout.o: src/vertexBufferLayout.c
	$(CC) $(CFLAGS) -c $^

vertexBuffer.o: src/vertexBuffer.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm -f *.o
