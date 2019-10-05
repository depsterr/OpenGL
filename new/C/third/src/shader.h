#pragma once

#include "renderer.h"

struct Shader{ //could possibly be cleared free() of values after compiled
	unsigned int m_RendererID;	
	char **m_FilePaths;
	unsigned int paths;
	unsigned int *shaderTypes;
	struct Uniform *uniforms;
	unsigned int uniformCount;
};

struct Uniform{
	char *name;
	int * id;
};

const void initShader(struct Shader *shader);

const void deleteShader(struct Shader *shader);

const void bindShader(struct Shader shader);

const void unbindShader(struct Shader shader);

void shaderSetUniform4f(struct Shader *shader, const char name[], float v1, float v2, float v3, float v4); //just sets a vec4

int shaderGetUniformLocation(struct Shader *shader, const char name[]); //used to get the location of a uniform from it's name, should be able to somehow remember names values and then if they are not recognised look them up with the GL function

void addShaderPath(struct Shader *shader, char filepath[], unsigned int shaderType); //adds one of the shaders to filepath

void compileShader(struct Shader *shader); //parses and compiles the shaders added to the shader struct

static unsigned int compileShaderSource(unsigned int type, const char source[]);
