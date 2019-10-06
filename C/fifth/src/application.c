#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string.h>
#include "renderer.h"
#include "vertexBuffer.h"
#include "texture.h"


#define W_WIDTH 256
#define W_HEIGHT 256

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

	window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "OpenGL", NULL, NULL); //add context to window
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

	float positions[] = { //the vertecies for the square we want to render, as well as texture cordinates
		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 1.0f
	};

	unsigned int indicies[] = { //define positions to use for rendering two triangles to make a square
		0, 1, 2,
		2, 3, 0
	};

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	struct VertexArray vao;
	initVertexArray(&vao);

	struct VertexBuffer vb;
	initVertexBuffer(&vb, positions, sizeof(float) * 4 * 4);

	struct VertexBufferLayout vbl;
	initVertexBufferLayout(&vbl);

	pushVertexBufferLayout(&vbl, GL_FLOAT, 2, GL_FALSE);
	pushVertexBufferLayout(&vbl, GL_FLOAT, 2, GL_FALSE);

	vertexArrayAddBuffer(&vao, &vb, &vbl);

	//deleteVertexBufferLayout(&vbl);

	struct IndexBuffer ib;
	initIndexBuffer(&ib, indicies, 6);
	
	//create shader from source
	
	struct Shader shader;
	initShader(&shader);
	addShaderPath(&shader, "shader/BasicVertex.shader", GL_VERTEX_SHADER);
	addShaderPath(&shader, "shader/BasicFragment.shader", GL_FRAGMENT_SHADER);
	compileShader(&shader);
	bindShader(shader);

	struct Texture texture;
	initTexture(&texture, "resources/textures/source-test.jpeg");
	bindTextureID(texture, 0);

	shaderSetUniform1i(&shader, "u_Texture", 0);

	unbindVertexArray(vao);
	unbindShader(shader);
	unbindIndexBuffer(ib);

	float r = 0.0f;
	float increment = 0.05f;

	while(!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS){ //loop for program

		rendererClear();

		bindShader(shader);
		shaderSetUniform4f(&shader, "u_Color", r, 0.2f, 0.3f, 1.0f);

		rendererDraw(vao, ib, shader);
		
		if(r > 1.0f)
			increment = -0.05;
		if(r < 0.0f)
			increment =  0.05;
		r += increment;


		glfwSwapBuffers(window); //swap the buffer

		glfwPollEvents(); //poll for events
	}

	deleteShader(&shader);
	glfwTerminate(); //kill GLFW
}
