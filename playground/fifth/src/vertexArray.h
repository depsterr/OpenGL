#pragma once

#include "renderer.h"
#include "vertexBuffer.h"
#include "vertexBufferLayout.h"

struct VertexArray{
	unsigned int m_RendererID;
};

struct InstancedVertexArray{
	unsigned int m_RendererID;
	unsigned int instances;
};

void initVertexArray(struct VertexArray *vertexarray);

void deleteVertexArray(struct VertexArray *vertexarray);

const void bindVertexArray(struct VertexArray vertexarray);

const void unbindVertexArray(struct VertexArray vertexarray);

void vertexArrayAddBuffer(const struct VertexArray *vertexarray,const struct VertexBuffer *vertexbuffer, struct VertexBufferLayout *vertexbufferlayout);

//instanced funcitons

void initInstancedVertexArray(struct InstancedVertexArray *vertexarray);

void deleteInstancedVertexArray(struct InstancedVertexArray *vertexarray);

const void bindInstancedVertexArray(struct InstancedVertexArray vertexarray);

const void unbindInstancedVertexArray(struct InstancedVertexArray vertexarray);

void instancedVertexArrayAddBuffer(const struct InstancedVertexArray *vertexarray,const struct VertexBuffer *vertexbuffer, struct VertexBufferLayout *vertexbufferlayout);
