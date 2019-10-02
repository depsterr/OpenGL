#include <stdlib.h>
#include "vertexBufferLayout.h"

void initVertexBufferLayout(struct VertexBufferLayout *vertexbufferlayout){
	vertexbufferlayout->count = 0;
	vertexbufferlayout->m_Stride = 0;
	vertexbufferlayout->m_Elements = (struct VertexBufferElement*)malloc(0);
}

//make it so the layout has a pointer to a list that gets longer (malloc, realloc) of VertexBufferElements.

void pushVertexBufferLayout(struct VertexBufferLayout *vertexbufferlayout, unsigned int inputType, unsigned int count, unsigned char normal){
		vertexbufferlayout->m_Elements = (struct VertexBufferElement*)realloc(vertexbufferlayout->m_Elements, sizeof(struct VertexBufferElement));
		vertexbufferlayout->m_Elements[vertexbufferlayout->count].type = inputType;
		vertexbufferlayout->m_Elements[vertexbufferlayout->count].count = count;
		vertexbufferlayout->m_Elements[vertexbufferlayout->count].normalized = normal;
		vertexbufferlayout->count++;
		vertexbufferlayout->m_Stride += getSizeOfType(inputType);
}

unsigned int getSizeOfType(unsigned int type){ //used to add to the stride
	switch(type){
		case GL_UNSIGNED_INT:
			return sizeof(GLuint);
		case GL_FLOAT:
			return sizeof(GLfloat);
		case GL_UNSIGNED_BYTE:
			return sizeof(GLbyte);
	}
	return 0;
}
