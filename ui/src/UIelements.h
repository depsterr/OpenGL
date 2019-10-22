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

typedef struct UIshape{
	unsigned int vertecies;
	unsigned int* vertexbuffer;
	int cornerradius;
	vec4* colors;
	vec3* colorpositions;
	//buffer and other stuff needed to draw shape
	//maybe setitngs like rounded etc
	//possible some constraints
}UIshape;

typedef struct UItext{
	char* text; //stuff needed to render font
}UItext;

union Data{
	int i;
	float f;
	vec2 v2;
	vec3 v3;
	vec4 v4;
	char str[12];
	unsigned char data[12];
};

typedef struct UIelement{
	UIshape* shapes;
	UItext* texts;
	union Data data;
}UIelement;

typedef struct UIwindow{
	GLFWwindow* window;
	GLFWwindow* returnwindow;
	UIelement* elements;
	unsigned int w_width;
	unsigned int w_height;
	vec3 clearcolor;
}UIwindow;

typedef struct UI{
	UIelement* elements;
	//possibly some other stuff that would help get the stuff on the screen
}UI;

unsigned char UIwindowUpdate(UIwindow uiwin); //UIwindow to update and window to bind back to afterwards

void initUIwindow(UIwindow* uiwin, GLFWwindow* returnwin, unsigned int width, unsigned int height, vec3 clear, char uiname[]); //set up all needed stuff

void deleteUIwindow(UIwindow* uiwin); //free up all stuff (also close first just to be sure)

void UIwindowAddElement(UIwindow* uiwin, UIelement);
