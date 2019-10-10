#include "vertexArray.h"

void initVertexArray(struct VertexArray *vertexarray){
	glGenVertexArrays(1, &vertexarray->m_RendererID);
	glBindVertexArray(vertexarray->m_RendererID);
}

void deleteVertexArray(struct VertexArray *vertexarray){
	glDeleteVertexArrays(1, &vertexarray->m_RendererID);
}

const void bindVertexArray(struct VertexArray vertexarray){
	glBindVertexArray(vertexarray.m_RendererID);
}

const void unbindVertexArray(struct VertexArray vertexarray){
	glBindVertexArray(0);
}

void vertexArrayAddBuffer(const struct VertexArray *vertexarray,const struct VertexBuffer *vertexbuffer, struct VertexBufferLayout *vertexbufferlayout){
	bindVertexArray(*vertexarray);
	bindVertexBuffer(*vertexbuffer);
	unsigned int offset = 0;
	for(unsigned int n = 0; n < vertexbufferlayout->count; n++){
		const struct VertexBufferElement element = vertexbufferlayout->m_Elements[n];
		glEnableVertexAttribArray(n);
		glVertexAttribPointer(n, element.count, element.type, element.normalized, vertexbufferlayout->m_Stride, (const void*)offset);
		offset += element.count * getSizeOfType(element.type);
	}
}
