#include "indexBuffer.h"
#include "renderer.h"

struct IndexBuffer{
	unsigned int m_rendererID;
	unsigned int m_Count;
};

void initIndexBuffer(struct IndexBuffer *indexbuffer, const unsigned int* data, unsigned int count){
	indexbuffer->m_Count = count;
	glGenBuffers(1, &indexbuffer->m_rendererID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer->m_rendererID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

void deleteIndexBuffer(struct IndexBuffer indexbuffer){
	glDeleteBuffers(1, indexbuffer.m_rendererID);
}

const void bindIndexBuffer(struct IndexBuffer indexbuffer){
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer.m_rendererID);
}

const void unbindIndexBuffer(struct IndexBuffer indexbuffer){
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
