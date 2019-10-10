#pragma once

#include <GL/glew.h>
#include "vertexArray.h"
#include "indexBuffer.h"
#include "shader.h"

struct Renderer{
	
};

void rendererDraw(const struct VertexArray va, const struct IndexBuffer ib, const struct Shader shader);

const void rendererClear();

void GLAPIENTRY MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam );

void static GLClearError();
void static GLCheckError();
