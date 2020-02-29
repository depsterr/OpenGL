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
#define SENSITIVITY 0.05f
/* #define fps enable to show fps */

struct Camera camera;

float lastX = W_WIDTH / 2, lastY = W_HEIGHT / 2;

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
	float xoffset = xpos - lastX;
	float yoffset = ypos - lastY;
	lastX = xpos;
	lastY = ypos;

	xoffset *= SENSITIVITY;
	yoffset *= SENSITIVITY;

	camera.pitch -= yoffset;
	camera.yaw += xoffset;

	if(camera.pitch < -89.9f){
		camera.pitch = -89.9f;
	}else if(camera.pitch > 89.9f){
		camera.pitch = 89.9f;
	}
}

int main(){

	/* init GLFW and set up settings */

	GLFWwindow* window; /* create window obj */

	if(!glfwInit()){ /* init GLFW */
		printf("could not init GLFW");
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "OpenGL", NULL, NULL); /* add context to window */
	if (!window){
		printf("could not create window");
		glfwTerminate();
		return -1;
	}


	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);

	glfwMakeContextCurrent(window); /* set window to the current context */

	glfwSwapInterval(0); /* Vsync disabled */

	/* init glew */

	if(glewInit() != GLEW_OK){ /* init GLEW */
		printf("could not init GLEW");
		return -1;
	}

	/* set up OpenGL settings and print version */

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEBUG_OUTPUT); /*  probably the greatest thing ever. */
	glDebugMessageCallback(MessageCallback, 0);

	printf("%s\n", glGetString(GL_VERSION)); /* print GL version */

	/* set up a cube with texture coordinates and normals */

	float positions[] = POS_TEX_NORM_CUBE; 
	struct VertexBuffer vb;                       
	initVertexBuffer(&vb, positions, POS_TEX_NORM_CUBE_BUFFER_SIZE);

	struct VertexBufferLayout vbl;
	initVertexBufferLayout(&vbl);

	pushVertexBufferLayout(&vbl, GL_FLOAT, 3, GL_FALSE);
	pushVertexBufferLayout(&vbl, GL_FLOAT, 2, GL_FALSE);
	pushVertexBufferLayout(&vbl, GL_FLOAT, 3, GL_FALSE);

	unsigned int indicies[] = POS_TEX_NORM_CUBE_INDEX;
	struct IndexBuffer ib;
	initIndexBuffer(&ib, indicies, POS_TEX_NORM_CUBE_INDEX_SIZE);

	/* create buffer of transforms for the cubes to do an instanced draw call */

	mat4 *transforms;
	transforms = (mat4*)malloc(100 * sizeof(mat4));

	for(int x = 0; x < 10; x++){
		for(int y = 0; y < 10; y++){
			glm_translate_to(GLM_MAT4_IDENTITY, (vec3){150 * x, 0, -150 * y}, transforms[(x * 10) + y]);
		}
	}

	struct VertexBuffer ivb;
	initVertexBuffer(&ivb, &transforms[0], 100 * sizeof(mat4));

	struct VertexBufferLayout ivbl;
	initVertexBufferLayout(&ivbl);

	pushInstancedVertexBufferMat4(&ivbl);

	/* create vertex array and add our buffers to it */

	struct VertexArray vao;
	initVertexArray(&vao);

	vertexArrayAddBuffer(&vao, &vb, &vbl);
	vertexArrayAddBuffer(&vao, &ivb, &ivbl);

	/* create white cube */

	float spositions[] = BASIC_CUBE;
	unsigned int sindicies[] = BASIC_CUBE_INDEX;

	struct IndexBuffer sib;
	initIndexBuffer(&sib, sindicies, BASIC_CUBE_INDEX_SIZE);
	struct VertexBuffer svb;                       
	initVertexBuffer(&svb, spositions, BASIC_CUBE_BUFFER_SIZE);

	struct VertexBufferLayout svbl;
	initVertexBufferLayout(&svbl);

	pushVertexBufferLayout(&svbl, GL_FLOAT, 3, GL_FALSE);

	/* create the white cubes vao */

	struct VertexArray svao;
	initVertexArray(&svao);

	vertexArrayAddBuffer(&svao, &svb, &svbl);

	/* set up camera */

	/* struct Camera camera; */
	initCamera(&camera);

	/* set up projection matrix */

	mat4 proj;
	glm_perspective(90.0f, SCREEN_RATIO, 0.01f, 100000.0f, proj);

	/* compile the shader for our white cube */
	
  	struct Shader whiteShader;
  	initShader(&whiteShader);
  	addShaderPath(&whiteShader, "shader/WhiteVertex.shader", GL_VERTEX_SHADER);
  	addShaderPath(&whiteShader, "shader/WhiteFragment.shader", GL_FRAGMENT_SHADER);
  	compileShader(&whiteShader);
  	freeShader(&whiteShader);
  	bindShader(whiteShader);

	/* create shader for drawing the other cubes */

	struct Shader instanceShader;
	initShader(&instanceShader);
	addShaderPath(&instanceShader, "shader/ShadedInstanceVertex.shader", GL_VERTEX_SHADER);
	addShaderPath(&instanceShader, "shader/ShadedFragment.shader", GL_FRAGMENT_SHADER);
	compileShader(&instanceShader);
	freeShader(&instanceShader);
	bindShader(instanceShader);
	
	/* set all the uniform settings  */

	shaderSetUniform1f(&instanceShader, "ambientStrength", 0.3f);
	shaderSetUniform1f(&instanceShader, "specularStrength", 1.0f);
	shaderSetUniform1f(&instanceShader, "diffuseStrength", 1.0f);
	shaderSetUniform1f(&instanceShader, "distanceStrength", 150.0f);
	shaderSetUniform3f(&instanceShader, "lightPos", 725.0f, 317.5f, -625.0f);
	shaderSetUniform3f(&instanceShader, "lightColor", 1.0f, 1.0f, 1.0f);
	shaderSetUniform3f(&instanceShader, "viewPos", 0.0f, 0.0f, 0.0f);

	shaderSetUniformMat4f(&instanceShader, "projection", proj);
	shaderSetUniformMat4f(&whiteShader, "projection", proj);

	/* create model matrix for the white cube */

	mat4 smodel;
	glm_translate_to(GLM_MAT4_IDENTITY, (vec3){707.5, 300, -642.5}, smodel);
	glm_scale_uni(smodel, 0.35f);
	shaderSetUniformMat4f(&whiteShader, "model", smodel);

	/* create our texture struct for the cubes */

	struct Texture texture;
	initTexture(&texture, "resources/textures/source-test.jpeg");
	bindTextureID(texture, 0);

	shaderSetUniform1i(&instanceShader, "u_Texture", 0);

	/* create renderer */

	struct Renderer renderer;
	initRenderer(&renderer);
	
	/* unbind everything and free up memory that is now on the GPU */
	
	unbindVertexArray(vao);
	unbindShader(instanceShader);
	unbindIndexBuffer(ib);
	free(transforms);

	while(!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && rendererLoop(&renderer, window)){
#ifdef fps
		printf("\nfps: %.2f", 1.0f / renderer.deltatime);
#endif
		/* wasd */
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

		/* reset */
		if(glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
			initCamera(&camera);
		}

		/* arrow camera */
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
		if(camera.pitch < -89.9f){
			camera.pitch = -89.9f;
		}else if(camera.pitch > 89.9f){
			camera.pitch = 89.9f;
		}
		cameraUpdateViewMatrix(&camera);

		/* update uniforms */
		shaderSetUniformMat4f(&instanceShader, "view", camera.view);
		shaderSetUniformMat4f(&whiteShader, "view", camera.view);
		shaderSetUniform3f(&instanceShader, "viewPos", camera.position[0], camera.position[1], camera.position[2]);
		/* draw objects */
		rendererInstancedDraw(vao, ib, instanceShader, 100);
		rendererDraw(svao, sib, whiteShader);
	}

	glfwTerminate(); /* kill GLFW */
}
