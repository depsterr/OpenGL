#include "renderer.h"
#include <stdio.h>

void GLAPIENTRY MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam ) {
    (void)id;(void)source;(void)length;(void)userParam;
    printf("OpenGL callback: %s type = %d, severity = %d, message = %s\n",
        ( type == GL_DEBUG_TYPE_ERROR ? "GL_DEBUG_TYPE_ERROR" : "" ),
        type, severity, message );
}

static void GLClearError(){
	while(glGetError() != GL_NO_ERROR);
}

static void GLCheckError(){
	GLenum error;
	while(error = glGetError()){
		printf("OpenGL error: %d\n", error);
	}
}

void rendererDraw(const struct VertexArray va, const struct IndexBuffer ib, const struct Shader shader){
	bindShader(shader);
	bindVertexArray(va);
	bindIndexBuffer(ib);
	glDrawElements(GL_TRIANGLES, ib.m_Count, GL_UNSIGNED_INT, 0); //draw triangles with 6 total verticies
}

const void rendererClear(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

unsigned char rendererLoop(struct Renderer *renderer, GLFWwindow* window){
	//end of loop
	glfwSwapBuffers(window); //swap the buffer
	glfwPollEvents(); //poll for events

	rendererClear();
	renderer->currenttime = glfwGetTime();
	renderer->deltatime = renderer->currenttime - renderer->lasttime;
	renderer->lasttime = renderer->currenttime;
	return 1;
}

void initRenderer(struct Renderer *renderer){
	renderer->currenttime = glfwGetTime();
	renderer->lasttime = renderer->currenttime;
	renderer->deltatime = 0.0f;
}
