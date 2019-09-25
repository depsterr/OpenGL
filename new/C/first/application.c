#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

int main(){
	GLFWwindow* window; //create window obj

	if(!glfwInit()){ //init GLFW
		printf("could not init GLFW");
		return -1;
	}

	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL); //add context to window
	if (!window){
		printf("could not create window");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window); //set window to the current context

	if(glewInit() != GLEW_OK){ //init GLEW
		printf("could not init GLEW");
		return -1;
	}

	printf("%s\n", glGetString(GL_VERSION)); //print GL version

	float positions[6] = { //the vertecies for the triangle we want to render
		-0.5f, -0.5f,
		 0.0f,  0.5f,
		 0.5f, -0.5f
	};

	unsigned int buffer;
	glGenBuffers(1, &buffer); //create one buffer and load ID of it into buffer var
	glBindBuffer(GL_ARRAY_BUFFER, buffer); //set type of buffer to array
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, positions, GL_STATIC_DRAW); //bind data to buffer
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0); //explain buffer data: index 0, 2 cause 2d, float data, doesn't need to be normalized, size per vertex, first attribute, therefore 0 (no offset from start)
	glEnableVertexAttribArray(0); //enable vertex attrib array ID 0
	
	while(!glfwWindowShouldClose(window)){ //loop for program
		
		glDrawArrays(GL_TRIANGLES, 0, 3);	

		glfwSwapBuffers(window); //swap the buffer

		glfwPollEvents(); //poll for events
	}

	glfwTerminate(); //kill GLFW
}
