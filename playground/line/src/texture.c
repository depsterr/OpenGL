#include "texture.h"
#include "vendor/stb_image/stb_image.h"
#include <string.h>
#include <stdlib.h>

void initTexture(struct Texture *texture, char path[]){
	texture->m_RendererID = 0;
	texture->filepath = (char*)malloc(strlen(path) + 1); //+1 for \0
	strcpy(texture->filepath, path);
	texture->m_LocalBuffer = 0;
	texture->m_Width = 0;
	texture->m_Height = 0;
	texture->m_BPP = 0;

	stbi_set_flip_vertically_on_load(1); //has to do with image encodings
	//stbi_set_flip_horizontally_on_load(1);
	texture->m_LocalBuffer = stbi_load(path, &texture->m_Width, &texture->m_Height, &texture->m_BPP, 4);

	glGenTextures(1, &texture->m_RendererID);
	glBindTexture(GL_TEXTURE_2D, texture->m_RendererID);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture->m_Width, texture->m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->m_LocalBuffer);
	glBindTexture(GL_TEXTURE_2D, 0);

	if(texture->m_LocalBuffer){
		stbi_image_free(texture->m_LocalBuffer);
	}
}

void deleteTexture(struct Texture *texture){
	glDeleteTextures(1, texture->m_RendererID);
}

const void bindTextureID(struct Texture texture, unsigned int id){
	glActiveTexture(GL_TEXTURE0 + id);
	glBindTexture(GL_TEXTURE_2D, texture.m_RendererID);
}

const void bindTexture(struct Texture texture){
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture.m_RendererID);
}

const void unbindTexture(struct Texture texture){
	glBindTexture(GL_TEXTURE_2D, 0);
}
