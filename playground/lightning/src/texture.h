#pragma once

#include "renderer.h"

struct Texture{
	unsigned int m_RendererID;
	char *filepath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;
};

void initTexture(struct Texture *texture, char path[]);

void deleteTexture(struct Texture *texture);

const void bindTextureID(struct Texture texture, unsigned int id);

const void bindTexture(struct Texture texture);

const void unbindTexture(struct Texture texture);
