#pragma once

#include "renderer.h"

struct VertexBufferElement{
	unsigned int type;
	unsigned int count;
	unsigned char normalized; //bool :)
	unsigned char instanced; //bool (:
};

struct VertexBufferLayout{
	struct VertexBufferElement *m_Elements; //malloc to 0 in inti to enable realloc
	unsigned int m_Stride;
	unsigned int count;
};

void initVertexBufferLayout(struct VertexBufferLayout *vertexbufferlayout);

//make it so the layout has a pointer to a list that gets longer (malloc, realloc) of VertexBufferElements.

void deleteVertexBufferLayout(struct VertexBufferLayout *vertexbufferlayout);

void pushVertexBufferLayout(struct VertexBufferLayout *vertexbufferlayout, unsigned int inputType, unsigned int count, unsigned char normal);

//like the one above but makes the vertex buffer shift one value per time
void pushInstancedVertexBufferLayout(struct VertexBufferLayout *vertexbufferlayout, unsigned int inputType, unsigned int count, unsigned char normal);

void pushInstancedVertexBufferMat4(struct VertexBufferLayout *vertexbufferlayout);

unsigned int getSizeOfType(unsigned int type); //used to add to the stride
