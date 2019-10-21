#include "renderer.h"

typedef struct UIshape{
	//buffer and other stuff needed to draw shape
	//maybe setitngs like rounded etc
	//possible some constraints
}UIshape;

typedef struct UItext{
	char* text;
	//stuff needed to render font
}UItext;

typedef struct UIelement{
	UIshape* shapes;
	UItext* texts;
}UIelement;

typedef struct UIwindow{
	GLFWwindow* window;
	UIelement* elements;
	unsigned int w_width;
	unsigned int w_height;
}UIwindow;

typedef struct UI{
	UIelement* elements;
	//possibly some other stuff that would help get the stuff on the screen
}UI;

void UIwindowUpdate(UIwindow UIwin, GLFWwindow* window); //UIwindow to update and window to bind back to afterwards

void UIelementRender(UIelement, GLFWwindow* window);
