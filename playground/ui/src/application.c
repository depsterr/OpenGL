#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string.h>
#include "renderer.h"
#include "vertexBuffer.h"
#include "texture.h"
#include <cglm/cglm.h>
#include "cglmExtention.h"
#include "camera.h"
#include "shapes.h"


#define W_WIDTH 960
#define W_HEIGHT 540
#define SCREEN_RATIO ((float)((float)W_WIDTH/(float)W_HEIGHT))
//#define fps enable to show fps

float lastX = W_WIDTH / 2, lastY = W_HEIGHT / 2;

int main(){

	//init GLFW and set up settings

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


	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glfwMakeContextCurrent(window); //set window to the current context

	glfwSwapInterval(0); //Vsync disabled

	//init glew

	if(glewInit() != GLEW_OK){ //init GLEW
		printf("could not init GLEW");
		return -1;
	}

	//set up OpenGL settings and print version

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEBUG_OUTPUT); // probably the greatest thing ever.
	glDebugMessageCallback(MessageCallback, 0);

	printf("%s\n", glGetString(GL_VERSION)); //print GL version

	while(!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && rendererLoop(&renderer, window)){
#ifdef fps
		printf("\nfps: %.2f", 1.0f / renderer.deltatime);
#endif
		//update uniforms
		
		//draw objects
		
	}

	glfwTerminate(); //kill GLFW
}
