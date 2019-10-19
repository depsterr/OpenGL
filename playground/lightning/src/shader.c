#include "shader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const void initShader(struct Shader *shader){
	shader->m_RendererID = glCreateProgram();	
	shader->m_FilePaths = (char**)malloc(0);
	shader->paths = 0;
	shader->shaderTypes = (unsigned int*)malloc(0);
	shader->uniforms = (struct Uniform*)malloc(0);
	shader->uniformCount = 0;
}

const void deleteShader(struct Shader *shader){
	for(int n = 0; n < shader->paths; n++){
		free(shader->m_FilePaths[n]);
	}
	free(shader->m_FilePaths);
	shader->paths = 0;
	shader->m_RendererID = 0;
	free(shader->shaderTypes);
	free(shader->uniforms);
	shader->uniformCount = 0;
	glDeleteProgram(shader->m_RendererID);
}

const void freeShader(struct Shader *shader){ //frees up variables that are not used post compilation, this is not done in the compile shader function since these might be usefull to keep around for debugging
	for(int n = 0; n < shader->paths; n++){
		free(shader->m_FilePaths[n]);
	}
	free(shader->m_FilePaths);
	free(shader->shaderTypes);
}


const void bindShader(struct Shader shader){
	glUseProgram(shader.m_RendererID);
}

const void unbindShader(struct Shader shader){
	glUseProgram(0);
}

void addShaderPath(struct Shader *shader, char filepath[], unsigned int shaderType){
	unsigned int pathsize = 0; 
	for(;filepath[pathsize] != '\0'; pathsize++);
	pathsize++; //account for \0 char
	shader->paths++;
	shader->m_FilePaths = (char**)realloc(shader->m_FilePaths, shader->paths * sizeof(char**));
	shader->m_FilePaths[shader->paths-1] = (char*)malloc(pathsize); //add one for \0 character
	shader->shaderTypes = (unsigned int*)realloc(shader->shaderTypes, shader->paths * sizeof(unsigned int));
	strcpy(shader->m_FilePaths[shader->paths-1], filepath);
	shader->shaderTypes[shader->paths-1] = shaderType;
}

void compileShader(struct Shader *shader){
	unsigned int *shaderIDs = (int*)malloc(shader->paths * sizeof(unsigned int));
	for(unsigned int n = 0; n < shader->paths; n++){ //compile each shaderfile
		FILE *fp;
		printf("\nfilepath %d: \"%s\"\n", n, shader->m_FilePaths[n]);
		fp = fopen(shader->m_FilePaths[n], "rb");

		char* buffer = NULL;
		size_t len;
		ssize_t bytes_read = getdelim( &buffer, &len, '\0', fp);
	
		printf("shader %d: \n\n-------------------------\n%s\n-------------------------\n\n", n, buffer);

		shaderIDs[n] = compileShaderSource(shader->shaderTypes[n], buffer);
		glAttachShader(shader->m_RendererID, shaderIDs[n]);
	}
	glLinkProgram(shader->m_RendererID); //link the binaries
	glValidateProgram(shader->m_RendererID); //validate it
	

	for(unsigned int n = 0; n < shader->paths; n++){ //delete all the shaders
		glDeleteShader(shaderIDs[n]);
	}
	free(shaderIDs);
	
}

static unsigned int compileShaderSource(unsigned int type, const char source[]){ //function to compile the shader, that way we do not have to write the same code twice in CreateShader()
	unsigned int id = glCreateShader(type); //create a shader of the given type, the funciton will return an ID that we use to adress the shader program
	glShaderSource(id, 1, &source, 0); //set the source of the shader program to ID, program one (out of 1), the source of the program code, amount of programs
	glCompileShader(id); //compile the shader

	int result; //variable to put the status of the compilation in
	glGetShaderiv(id, GL_COMPILE_STATUS, &result); //get info, shader to get info from, what info we want, a pointer to the string to put the data into
	if(result == GL_FALSE){ //if there is an error
		int length; //int to put length into
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char *message;
		message = (char *)malloc(length * sizeof(char)); //char[] to get message
		glGetShaderInfoLog(id, length, &length, message); //get the info from the log
		printf("Failed to compile %sshader\n", (type == GL_VERTEX_SHADER ? "vertex" : "fragment")); //print which shader it is that failed
		printf("%s\n", message); //print the shader error log
		free(message); //free up memory
		return 0; //unsigned, cannot return -1
	}else{
		printf("Successfully compiled %sshader\n", (type == GL_VERTEX_SHADER ? "vertex" : "fragment")); //print which shader it is that failed
	}

	return id; //return id as refrence to created shader
}

void shaderSetUniform4f(struct Shader *shader, const char name[], float v1, float v2, float v3, float v4){
	bindShader(*shader);
	glUniform4f(shaderGetUniformLocation(shader, name), v1, v2, v3, v4);
}

void shaderSetUniform1f(struct Shader *shader, const char name[], float v1){
	bindShader(*shader);
	glUniform1f(shaderGetUniformLocation(shader, name), v1);
}

void shaderSetUniform1i(struct Shader *shader, const char name[], int v1){
	bindShader(*shader);
	glUniform1i(shaderGetUniformLocation(shader, name), v1);
}

void shaderSetUniformMat4f(struct Shader *shader, const char name[], const mat4 matrix){
	bindShader(*shader);
	glUniformMatrix4fv(shaderGetUniformLocation(shader, name), 1, GL_FALSE, &matrix[0][0]);
}

int shaderGetUniformLocation(struct Shader *shader, const char name[]){
	for(unsigned int n = 0; n < shader->uniformCount; n++){
		if(!strcmp(name, shader->uniforms[n].name)){
			return shader->uniforms[n].id;
		}
	}
	shader->uniformCount++;
	shader->uniforms = (struct Uniform*)realloc(shader->uniforms, sizeof(struct Uniform) * shader->uniformCount);
	shader->uniforms[shader->uniformCount-1].id = glGetUniformLocation(shader->m_RendererID, name);
	shader->uniforms[shader->uniformCount-1].name = name;
	return shader->uniforms[shader->uniformCount-1].id;
}
