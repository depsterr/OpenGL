#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string.h>
#include "renderer.h"
#include "vertexBuffer.h"
#include "indexBuffer.h"
#include "vertexArray.h"


#define W_WIDTH 640
#define W_HEIGHT 480


static unsigned int CompileShader(unsigned int type, const char source[]){ //function to compile the shader, that way we do not have to write the same code twice in CreateShader()
	unsigned int id = glCreateShader(type); //create a shader of the given type, the funciton will return an ID that we use to adress the shader program
	glShaderSource(id, 1, &source, 0); //set the source of the shader program to ID, program one (out of 1), the source of the program code, amount of programs
	glCompileShader(id); //compile the shader

	int result; //variable to put the status of the compilation in
	glGetShaderiv(id, GL_COMPILE_STATUS, &result); //get info, shader to get info from, what info we want, a pointer to the string to put the data into
	if(result == GL_FALSE){ //if there is an error
		int length; //int to put length into
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char *message;
		message = (char *)malloc(length * sizeof(char)); //char[] to get message
		glGetShaderInfoLog(id, length, &length, message); //get the info from the log
		printf("Failed to compile %s shader\n", (type == GL_VERTEX_SHADER ? "vertex" : "fragment")); //print which shader it is that failed
		printf("%s\n", message); //print the shader error log
		free(message); //free up memory
		return 0; //unsigned, cannot return -1
	}

	return id; //return id as refrence to created shader
}

static unsigned int CreateShader(const char vertexShader[], const char fragmentShader[]){ //function that takes two strings and creates shaders form them

	unsigned int program = glCreateProgram(); //create a program, returns program ID
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader); //create two shaders
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs); //attach the shaders
	glAttachShader(program, fs);
	glLinkProgram(program); //link the binaries
	glValidateProgram(program); //validate it

	glDeleteShader(vs); //delete the shaders
	glDeleteShader(fs);
	
	return program; //return the id to the program
}

static unsigned int ParseShader(const char vertexFilepath[], const char fragmentFilepath[]){
	FILE *vfp;
	FILE *ffp;

	char *vertexBuffer;
	long vertexLen;
	char *fragmentBuffer;
	long fragmentLen;

	vfp = fopen(vertexFilepath, "rb");
	ffp = fopen(fragmentFilepath, "rb");

	if(vfp){
		fseek (vfp, 0, SEEK_END);
		vertexLen = ftell (vfp);
		fseek (vfp, 0, SEEK_SET);
		vertexBuffer = malloc (vertexLen);
		if (vertexBuffer){
			fread (vertexBuffer, 1, vertexLen, vfp);
		}
		fclose (vfp);
	}

	if(ffp){
		fseek (ffp, 0, SEEK_END);
		fragmentLen = ftell (ffp);
		fseek (ffp, 0, SEEK_SET);
		fragmentBuffer = malloc (fragmentLen);
		if (fragmentBuffer){
			fread (fragmentBuffer, 1, fragmentLen, ffp);
		}
		fclose (ffp);
	}
	printf("Vertexshader: \n\n%s\n", vertexBuffer);
	printf("Fragmentshader: \n\n%s\n", fragmentBuffer);
	return CreateShader(vertexBuffer, fragmentBuffer);
}

int main(){
	GLFWwindow* window; //create window obj

	if(!glfwInit()){ //init GLFW
		printf("could not init GLFW");
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "Hello World", NULL, NULL); //add context to window
	if (!window){
		printf("could not create window");
		glfwTerminate();
		return -1;
	}

	glfwSwapInterval(1); //Vsync enabled

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glfwMakeContextCurrent(window); //set window to the current context

	if(glewInit() != GLEW_OK){ //init GLEW
		printf("could not init GLEW");
		return -1;
	}

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEBUG_OUTPUT); // probably the greatest thing ever.
	glDebugMessageCallback(MessageCallback, 0);

	printf("%s\n", glGetString(GL_VERSION)); //print GL version

	float positions[] = { //the vertecies for the square we want to render
		-1.0f, -1.0f,
		 1.0f, -1.0f,
		 1.0f,  1.0f,
		-1.0f,  1.0f,
	};

	unsigned int indicies[] = { //define positions to use for rendering two triangles to make a square
		0, 1, 2,
		2, 3, 0
	};

	struct VertexArray vao;
	initVertexArray(&vao);

	struct VertexBuffer vb;
	initVertexBuffer(&vb, positions, sizeof(float) * 4 * 2);

	struct VertexBufferLayout vbl;
	initVertexBufferLayout(&vbl);

	pushVertexBufferLayout(&vbl, GL_FLOAT, 2, GL_FALSE);

	vertexArrayAddBuffer(&vao, &vb, &vbl);

	struct IndexBuffer ib;
	initIndexBuffer(&ib, indicies, 6);
	
	//create shader from source
	
	unsigned int shader = ParseShader("shader/BasicVertex.shader", "shader/BasicFragment.shader");
	glUseProgram(shader);

	int location = glGetUniformLocation(shader, "u_Color"); //get id for u_Color to be able to send uniform data
	glUniform4f(location, 0.8f, 0.0f, 0.5f, 1.0f);

	glBindVertexArray(0);
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	float r = 0.0f;
	float increment = 0.05f;

	while(!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS){ //loop for program

		glClear(GL_COLOR_BUFFER_BIT);


		glUseProgram(shader);
		glUniform4f(location, r, 0.2f, 0.3f, 1.0f);

		bindVertexArray(vao);
		bindIndexBuffer(ib);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //draw triangles with 6 total verticies

		if(r > 1.0f)
			increment = -0.05;
		if(r < 0.0f)
			increment =  0.05;
		r += increment;


		glfwSwapBuffers(window); //swap the buffer

		glfwPollEvents(); //poll for events
	}

	glfwTerminate(); //kill GLFW
}
