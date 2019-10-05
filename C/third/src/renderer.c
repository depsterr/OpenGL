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
