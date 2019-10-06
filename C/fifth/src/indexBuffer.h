#pragma once

struct IndexBuffer{
	unsigned int m_rendererID;
	unsigned int m_Count;
};

void initIndexBuffer(struct IndexBuffer *indexbuffer, const unsigned int* data, unsigned int count);
void deleteIndexBuffer(struct IndexBuffer indexbuffer);

const void bindIndexBuffer(struct IndexBuffer indexbuffer);
const void unbindIndexBuffer(struct IndexBuffer indexbuffer);
