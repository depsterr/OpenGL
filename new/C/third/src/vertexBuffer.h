#pragma once
#include "vertexBuffer.c"

void initVertexBuffer(struct VertexBuffer *vertexbuffer, const void* data, unsigned int size);
void deleteVertexBuffer(struct VertexBuffer vertexbuffer);

const void bindVertexBuffer(struct VertexBuffer vertexbuffer);
const void unbindVertexBuffer(struct VertexBuffer vertexbuffer);
