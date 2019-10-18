#pragma once

#include "renderer.h"
#include "vertexBuffer.h"
#include "vertexBufferLayout.h"

struct VertexArray{
	unsigned int m_RendererID;
	unsigned int attribCount;
};

void initVertexArray(struct VertexArray *vertexarray);

void deleteVertexArray(struct VertexArray *vertexarray);

const void bindVertexArray(struct VertexArray vertexarray);

const void unbindVertexArray(struct VertexArray vertexarray);

void vertexArrayAddBuffer(struct VertexArray *vertexarray,const struct VertexBuffer *vertexbuffer, struct VertexBufferLayout *vertexbufferlayout);

//instanced funcitons

void vertexArrayAddInstancedBuffer(const struct VertexArray *vertexarray,const struct VertexBuffer *vertexbuffer, struct VertexBufferLayout *vertexbufferlayout);
