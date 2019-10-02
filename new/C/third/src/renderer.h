#pragma once

#include <GL/glew.h>
#include "renderer.c"

void GLAPIENTRY MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam );

void static GLClearError();
void static GLCheckError();
