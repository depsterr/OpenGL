#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string.h>
#include "renderer.h"
#include "vertexBuffer.h"
#include "texture.h"
#include <cglm/cglm.h>
#include "cglmExtention.h"
#include "camera.h"


#define W_WIDTH 960
#define W_HEIGHT 540
#define SCREEN_RATIO ((float)((float)W_WIDTH/(float)W_HEIGHT))
#define FOV 100.0f
#define FARVAL 900.0f
#define NEARVAL 10.0f
#define TURNSPEED 90.0f
#define MOVESPEED 100.0f

int main(){
	GLFWwindow* window; //create window obj

	if(!glfwInit()){ //init GLFW
		printf("could not init GLFW");
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "OpenGL", NULL, NULL); //add context to window
	if (!window){
		printf("could not create window");
		glfwTerminate();
		return -1;
	}

	glfwSwapInterval(1); //Vsync enabled

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glfwMakeContextCurrent(window); //set window to the current context

	if(glewInit() != GLEW_OK){ //init GLEW
		printf("could not init GLEW");
		return -1;
	}

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEBUG_OUTPUT); // probably the greatest thing ever.
	glDebugMessageCallback(MessageCallback, 0);

	printf("%s\n", glGetString(GL_VERSION)); //print GL version

	float positions[] = { //the vertecies for the square we want to render, as well as texture cordinates
        //back facing
		000.0f, 000.0f, 000.0f, 1.0f, 0.0f,  
		100.0f, 000.0f, 000.0f, 0.0f, 0.0f, 
		100.0f, 100.0f, 000.0f, 0.0f, 1.0f,
		000.0f, 100.0f, 000.0f, 1.0f, 1.0f,

		//front facing		
		100.0f, 000.0f, 100.0f, 1.0f, 0.0f, 
		000.0f, 000.0f, 100.0f, 0.0f, 0.0f, 
		000.0f, 100.0f, 100.0f, 0.0f, 1.0f, 
		100.0f, 100.0f, 100.0f, 1.0f, 1.0f, 

        //top
		000.0f, 100.0f, 000.0f, 0.0f, 0.0f, 
		100.0f, 100.0f, 000.0f, 1.0f, 0.0f, 
		100.0f, 100.0f, 100.0f, 1.0f, 1.0f, 
		000.0f, 100.0f, 100.0f, 0.0f, 1.0f, 

        //bottom
		100.0f, 000.0f, 100.0f, 0.0f, 0.0f, 
		000.0f, 000.0f, 100.0f, 1.0f, 0.0f, 
		000.0f, 000.0f, 000.0f, 1.0f, 1.0f, 
		100.0f, 000.0f, 000.0f, 0.0f, 1.0f, 

		//left
		000.0f, 000.0f, 100.0f, 1.0f, 0.0f, 
		000.0f, 000.0f, 000.0f, 0.0f, 0.0f, 
		000.0f, 100.0f, 000.0f, 0.0f, 1.0f, 
		000.0f, 100.0f, 100.0f, 1.0f, 1.0f, 

   	 	//right
		100.0f, 000.0f, 000.0f, 1.0f, 0.0f, 
		100.0f, 000.0f, 100.0f, 0.0f, 0.0f, 
		100.0f, 100.0f, 100.0f, 0.0f, 1.0f, 
		100.0f, 100.0f, 000.0f, 1.0f, 1.0f 
	};

	struct VertexBuffer vb;                       
	initVertexBuffer(&vb, positions, sizeof(float) * 5 * 4 * 6);

	struct VertexBufferLayout vbl;
	initVertexBufferLayout(&vbl);

	pushVertexBufferLayout(&vbl, GL_FLOAT, 3, GL_FALSE);
	pushVertexBufferLayout(&vbl, GL_FLOAT, 2, GL_FALSE);

	vec3 translationA = {000, 00, -200};
	vec3 translationB = {300, 00, -150};

	mat4 transforms[2];

	glm_translate_to((mat4)GLM_MAT4_IDENTITY_INIT, translationA, transforms[0]);
	glm_translate_to((mat4)GLM_MAT4_IDENTITY_INIT, translationB, transforms[1]);

	struct VertexBuffer ivb;
	initVertexBuffer(&ivb, &transforms[0], 2 * sizeof(mat4));

	struct VertexBufferLayout ivbl;
	initVertexBufferLayout(&ivbl);

	pushInstancedVertexBufferMat4(&ivbl);

	unsigned int indicies[] = { 
		0, 1, 2, //front
		2, 3, 0,

		4, 5, 6, //back
		6, 7, 4,

		8, 9, 10, //top
		10, 11, 8,

		12, 13, 14, //bottom
		14, 15, 12,

		16, 17, 18, //left
		18, 19, 16,

		20, 21, 22, //right
		22, 23, 20
	}; 

	struct IndexBuffer ib;
	initIndexBuffer(&ib, indicies, 6 * 6); //todo fix this

	struct VertexArray vao;
	initVertexArray(&vao);

	vertexArrayAddBuffer(&vao, &vb, &vbl);
	vertexArrayAddBuffer(&vao, &ivb, &ivbl);
	printf("\n%d\n", vao.attribCount);

	struct Camera camera;
	initCamera(&camera);

	mat4 proj;
    glm_perspective(90.0f, SCREEN_RATIO, 0.01f, 1000.0f, proj);

	mat4 model = GLM_MAT4_IDENTITY_INIT;

	//normal shader
	
//	struct Shader shader;
//	initShader(&shader);
//	addShaderPath(&shader, "shader/BasicVertex.shader", GL_VERTEX_SHADER);
//	addShaderPath(&shader, "shader/BasicFragment.shader", GL_FRAGMENT_SHADER);
//	compileShader(&shader);
//	freeShader(&shader);
//	bindShader(shader);

	//instance shader
	struct Shader instanceShader;
	initShader(&instanceShader);
	addShaderPath(&instanceShader, "shader/InstanceVertex.shader", GL_VERTEX_SHADER);
	addShaderPath(&instanceShader, "shader/BasicFragment.shader", GL_FRAGMENT_SHADER);
	compileShader(&instanceShader);
	freeShader(&instanceShader);
	bindShader(instanceShader);

	shaderSetUniformMat4f(&instanceShader, "projection", proj);

	struct Texture texture;
	initTexture(&texture, "resources/textures/source-test.jpeg");
	bindTextureID(texture, 0);

	shaderSetUniform1i(&instanceShader, "u_Texture", 0);

	struct Renderer renderer;
	initRenderer(&renderer);
	
	unbindVertexArray(vao);
	unbindShader(instanceShader);
	unbindIndexBuffer(ib);

	while(!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && rendererLoop(&renderer, window)){
		//wasd
		if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
			cameraMoveRelativeZ(&camera, MOVESPEED * renderer.deltatime);
		}
		if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
			cameraMoveRelativeZ(&camera, -MOVESPEED * renderer.deltatime);
		}
		if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
			cameraMoveRelativeX(&camera, MOVESPEED * renderer.deltatime);
		} 
		if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
			cameraMoveRelativeX(&camera, -MOVESPEED * renderer.deltatime);
		} 

		//reset
		if(glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
			initCamera(&camera);
		}

		//arrow camera
		if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
			camera.pitch += TURNSPEED * renderer.deltatime;
		}
		if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
			camera.pitch -= TURNSPEED * renderer.deltatime;
		}
		if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
			camera.yaw += TURNSPEED * renderer.deltatime;
		} 
		if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
			camera.yaw -= TURNSPEED * renderer.deltatime;
		} 
		cameraUpdateViewMatrix(&camera);

	//	shaderSetUniformMat4f(&shader, "model", model);
	//	shaderSetUniformMat4f(&shader, "view", camera.view);
		shaderSetUniformMat4f(&instanceShader, "view", camera.view);
	//	rendererDraw(vao, ib, shader);
		rendererInstancedDraw(vao, ib, instanceShader, 2);

	}

	glfwTerminate(); //kill GLFW
}
