#include "vertexArray.h"

void initVertexArray(struct VertexArray *vertexarray){
	glGenVertexArrays(1, &vertexarray->m_RendererID);
	glBindVertexArray(vertexarray->m_RendererID);
	vertexarray->attribCount = 0;
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

void vertexArrayAddBuffer(struct VertexArray *vertexarray,const struct VertexBuffer *vertexbuffer, struct VertexBufferLayout *vertexbufferlayout){
	bindVertexArray(*vertexarray);
	bindVertexBuffer(*vertexbuffer);
	unsigned int offset = 0;
	unsigned int oldattribCount = vertexarray->attribCount;
	for(unsigned int n = vertexarray->attribCount; n < (vertexbufferlayout->count + oldattribCount); n++){
		const struct VertexBufferElement element = vertexbufferlayout->m_Elements[n - oldattribCount];
		glEnableVertexAttribArray(n);
		glVertexAttribPointer(n, element.count, element.type, element.normalized, vertexbufferlayout->m_Stride, (const void*)offset);
		if(element.instanced != 0) {glVertexAttribDivisor(n, 1);} //instanced buffers for instanced draw
		offset += element.count * getSizeOfType(element.type);
		vertexarray->attribCount++;
	//	printf("\nn: %d glVertexAttribPointer(%d, %d, %d, %d, %d, %d) instanced: %d attribCount: %d\n", n, n, element.count, element.type, element.normalized, vertexbufferlayout->m_Stride, offset, element.instanced, vertexarray->attribCount);
	}
}
