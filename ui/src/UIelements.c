#include "UIelements.h"

void initUIwindow(UIwindow* uiwin, GLFWwindow* returnwin, unsigned int width, unsigned int height, vec3 clear, char uiname[]){
	uiwin->returnwindow = returnwin;
	uiwin->w_width = width;
	uiwin->w_height = height;
	uiwin->clearcolor[0] = clear[0];
	uiwin->clearcolor[1] = clear[1];
	uiwin->clearcolor[2] = clear[2];
	uiwin->elements = (UIelement*)malloc(0);

	if(!glfwInit()){ //init GLFW
		printf("could not init GLFW");
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	uiwin->window = glfwCreateWindow(uiwin->w_width, uiwin->w_height, uiname, NULL, NULL);
	if(!uiwin->window){
		printf("couldn't create window");
		return;
	}

	glfwSetInputMode(uiwin->window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(uiwin->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glfwMakeContextCurrent(uiwin->window);
	glfwSwapInterval(1);

	if(glewInit() != GLEW_OK){ //init GLEW
		printf("could not init GLEW");
		return;
	}

	glClearColor(clear[0], clear[1], clear[2], 1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEBUG_OUTPUT); // probably the greatest thing ever.
	glDebugMessageCallback(MessageCallback, 0);

	glfwMakeContextCurrent(returnwin);
}

void deleteUIwindow(UIwindow* uiwin){
	glfwDestroyWindow(uiwin->window);
	free(uiwin->elements);
	uiwin->w_width = 0;
	uiwin->w_height = 0;
	uiwin->clearcolor[0] = 0;
	uiwin->clearcolor[1] = 0;
	uiwin->clearcolor[2] = 0;
}

unsigned char UIwindowUpdate(UIwindow uiwin){
	glfwMakeContextCurrent(uiwin.window);
	glClearColor(uiwin.clearcolor[0], uiwin.clearcolor[1], uiwin.clearcolor[2], 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	UIwindowDraw(uiwin);
	glfwSwapBuffers(uiwin.window); //swap the buffer
	glfwPollEvents(); //poll for events
	glfwMakeContextCurrent(uiwin.returnwindow);
	return 1;
}

void UIwindowDraw(UIwindow uiwin){
	for(unsigned int element; element < uiwin.elementCount; element++){
		for(unsigned int text; text < uiwin.elements[element].textCount; text++){
			
		}
		for(unsigned int shape; shape < uiwin.elements[element].shapeCount; shape++){
			
		}
	}
}

void UIelementDraw(UIelement uiele);

void UIshapeDraw(UIelement uiele);

void UItextDraw(UItext uitxt);
