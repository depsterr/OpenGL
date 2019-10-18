CC=gcc
PROJECTDIR=build/application.out
BUILDDIR=.
CFLAGS= -lglfw -lGLEW -lGL -lGLU -lm -lcglm -g

default: src/application.c vertexBuffer.o indexBuffer.o vertexArray.o renderer.o vertexBufferLayout.o shader.o stb_image.o texture.o camera.o
	$(CC) -o $(PROJECTDIR) $^ $(CFLAGS)
	make clean

indexBuffer.o: src/indexBuffer.c
	$(CC) -c $^ $(CFLAGS)

renderer.o: src/renderer.c
	$(CC) -c $^ $(CFLAGS)

vertexArray.o: src/vertexArray.c
	$(CC) -c $^ $(CFLAGS)

vertexBufferLayout.o: src/vertexBufferLayout.c
	$(CC) -c $^ $(CFLAGS)

vertexBuffer.o: src/vertexBuffer.c
	$(CC) -c $^ $(CFLAGS)

shader.o: src/shader.c
	$(CC) -c $^ $(CFLAGS)

stb_image.o: src/vendor/stb_image/stb_image.c
	$(CC) -c $^ $(CFLAGS)

texture.o: src/texture.c
	$(CC) -c $^ $(CFLAGS)

camera.o: src/camera.c
	$(CC) -c $^ $(CFLAGS)

clean:
	rm -f *.o

run:
	make default
	./build/application.out

no-cleanup: src/application.c vertexBuffer.o indexBuffer.o vertexArray.o renderer.o vertexBufferLayout.o shader.o stb_image.o texture.o nuklear.o
	$(CC) -o $(PROJECTDIR) $^ $(CFLAGS)
