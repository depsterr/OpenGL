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
#include "shapes.h"


#define W_WIDTH 960
#define W_HEIGHT 540
#define SCREEN_RATIO ((float)((float)W_WIDTH/(float)W_HEIGHT))
#define FOV 90.0f
#define FARVAL 900.0f
#define NEARVAL 10.0f
#define TURNSPEED 90.0f
#define MOVESPEED 500.0f
//#define fps enable to show fps

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


	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glfwMakeContextCurrent(window); //set window to the current context

	glfwSwapInterval(0); //Vsync disabled

	if(glewInit() != GLEW_OK){ //init GLEW
		printf("could not init GLEW");
		return -1;
	}

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEBUG_OUTPUT); // probably the greatest thing ever.
	glDebugMessageCallback(MessageCallback, 0);

	printf("%s\n", glGetString(GL_VERSION)); //print GL version

	float positions[] = { //the vertecies for the square we want to render, as well as texture cordinates and normals
        //back facing
		000.0f, 000.0f, 000.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		100.0f, 000.0f, 000.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		100.0f, 100.0f, 000.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
		000.0f, 100.0f, 000.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,

		//front facing		
		100.0f, 000.0f, 100.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 
		000.0f, 000.0f, 100.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		000.0f, 100.0f, 100.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		100.0f, 100.0f, 100.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        //top
		000.0f, 100.0f, 000.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		100.0f, 100.0f, 000.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		100.0f, 100.0f, 100.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		000.0f, 100.0f, 100.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,

        //bottom
		100.0f, 000.0f, 100.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 
		000.0f, 000.0f, 100.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,  
		000.0f, 000.0f, 000.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
		100.0f, 000.0f, 000.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,

		//left
		000.0f, 000.0f, 100.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		000.0f, 000.0f, 000.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		000.0f, 100.0f, 000.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
		000.0f, 100.0f, 100.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,

   	 	//right
		100.0f, 000.0f, 000.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 
		100.0f, 000.0f, 100.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		100.0f, 100.0f, 100.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		100.0f, 100.0f, 000.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f
	};

	struct VertexBuffer vb;                       
	initVertexBuffer(&vb, positions, sizeof(float) * 8 * 4 * 6);

	struct VertexBufferLayout vbl;
	initVertexBufferLayout(&vbl);

	pushVertexBufferLayout(&vbl, GL_FLOAT, 3, GL_FALSE);
	pushVertexBufferLayout(&vbl, GL_FLOAT, 2, GL_FALSE);
	pushVertexBufferLayout(&vbl, GL_FLOAT, 3, GL_FALSE);

	mat4 *transforms;
	transforms = (mat4*)malloc(2 * sizeof(mat4));

	glm_translate_to(GLM_MAT4_IDENTITY, (vec3){150, 0, -200}, transforms[0]);
	glm_translate_to(GLM_MAT4_IDENTITY, (vec3){-175, 0, -100}, transforms[1]);


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
	initIndexBuffer(&ib, indicies, 6 * 6);

	struct VertexArray vao;
	initVertexArray(&vao);

	vertexArrayAddBuffer(&vao, &vb, &vbl);
	vertexArrayAddBuffer(&vao, &ivb, &ivbl);

	//white cube

	float spositions[] = BASIC_CUBE;
	unsigned int sindicies[] = BASIC_CUBE_INDEX;

	struct IndexBuffer sib;
	initIndexBuffer(&sib, sindicies, 6 * 6);
	struct VertexBuffer svb;                       
	initVertexBuffer(&svb, spositions, sizeof(float) * 3 * 8);

	struct VertexBufferLayout svbl;
	initVertexBufferLayout(&svbl);

	pushVertexBufferLayout(&svbl, GL_FLOAT, 3, GL_FALSE);

	struct VertexArray svao;
	initVertexArray(&svao);

	vertexArrayAddBuffer(&svao, &svb, &svbl);

	//end white cube

	struct Camera camera;
	initCamera(&camera);

	mat4 proj;
    glm_perspective(90.0f, SCREEN_RATIO, 0.01f, 100000.0f, proj);

	//normal shader
	
  	struct Shader whiteShader;
  	initShader(&whiteShader);
  	addShaderPath(&whiteShader, "shader/WhiteVertex.shader", GL_VERTEX_SHADER);
  	addShaderPath(&whiteShader, "shader/WhiteFragment.shader", GL_FRAGMENT_SHADER);
  	compileShader(&whiteShader);
  	freeShader(&whiteShader);
  	bindShader(whiteShader);

	//instance shader
	struct Shader instanceShader;
	initShader(&instanceShader);
	addShaderPath(&instanceShader, "shader/ShadedInstanceVertex.shader", GL_VERTEX_SHADER);
	addShaderPath(&instanceShader, "shader/ShadedFragment.shader", GL_FRAGMENT_SHADER);
	compileShader(&instanceShader);
	freeShader(&instanceShader);
	bindShader(instanceShader);

	shaderSetUniform1f(&instanceShader, "ambientStrength", 0.3f);
	shaderSetUniform1f(&instanceShader, "specularStrength", 0.3f);
	shaderSetUniform3f(&instanceShader, "lightPos", 50, 150, -125);
	shaderSetUniform3f(&instanceShader, "lightColor", 1.0f, 1.0f, 1.0f);
	shaderSetUniform3f(&instanceShader, "viewPos", 0.0f, 0.0f, 0.0f);

	shaderSetUniformMat4f(&instanceShader, "projection", proj);
	shaderSetUniformMat4f(&whiteShader, "projection", proj);

	mat4 smodel;
	glm_translate_to(GLM_MAT4_IDENTITY, (vec3){0, 100, -175}, smodel);
	glm_scale_uni(smodel, 0.35f);
	shaderSetUniformMat4f(&whiteShader, "model", smodel);

	struct Texture texture;
	initTexture(&texture, "resources/textures/source-test.jpeg");
	bindTextureID(texture, 0);

	shaderSetUniform1i(&instanceShader, "u_Texture", 0);

	struct Renderer renderer;
	initRenderer(&renderer);
	
	unbindVertexArray(vao);
	unbindShader(instanceShader);
	unbindIndexBuffer(ib);
	free(transforms);

	while(!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && rendererLoop(&renderer, window)){
#ifdef fps
		printf("\nfps: %.2f", 1.0f / renderer.deltatime);
#endif
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

		shaderSetUniformMat4f(&instanceShader, "view", camera.view);
		shaderSetUniformMat4f(&whiteShader, "view", camera.view);
		shaderSetUniform3f(&instanceShader, "viewPos", camera.position[0], camera.position[1], camera.position[2]);
		rendererInstancedDraw(vao, ib, instanceShader, 2);
		rendererDraw(svao, sib, whiteShader);
	}

	glfwTerminate(); //kill GLFW
}
