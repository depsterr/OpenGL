#include "UIelements.h"

void startUIwindow(UIwindow* uiwin, GLFWwindow* return, unsigned int width, unsigned int height, vec3 clear){
	initUIwindow(uiwin, return, width, height, clear);
	openUIwindow(uiwin);
}

void initUIwindow(UIwindow* uiwin, GLFWwindow* return, unsigned int width, unsigned int height, vec3 clear){
	uiwin->returnwindow = return;
	uiwin->w_width = width;
	uiwin->w_height = height;
	uiwin->clearcolor = clear;

}
