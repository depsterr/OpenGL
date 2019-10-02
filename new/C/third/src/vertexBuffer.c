#include "vertexBuffer.h"
#include "renderer.h"

void initVertexBuffer(struct VertexBuffer *vertexbuffer, const void* data, unsigned int size){
	glGenBuffers(1, &vertexbuffer->m_rendererID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer->m_rendererID);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void deleteVertexBuffer(struct VertexBuffer vertexbuffer){
	glDeleteBuffers(1, vertexbuffer.m_rendererID);
}

const void bindVertexBuffer(struct VertexBuffer vertexbuffer){
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer.m_rendererID);
}

const void unbindVertexBuffer(struct VertexBuffer vertexbuffer){
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
