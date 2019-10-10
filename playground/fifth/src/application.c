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
#include "vendor/nuklear/nuklear.h"


#define W_WIDTH 960
#define W_HEIGHT 540
#define SCREEN_RATIO ((float)((float)W_WIDTH/(float)W_HEIGHT))
#define FOV 100.0f
#define FARVAL 900.0f
#define NEARVAL 10.0f

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

//  Nuklear UI that can be used for debugging, etc	
//  enum {EASY, HARD};
//  static int op = EASY;
//  static float value = 0.6f;
//  static int i =  20;
//  struct nk_context ctx;
//  int nk_init_default(struct nk_context *ctx, const struct nk_user_font *font);
//  if (nk_begin(&ctx, "Show", nk_rect(50, 50, 220, 220),
//      NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_CLOSABLE)) {
//      // fixed widget pixel width
//      nk_layout_row_static(&ctx, 30, 80, 1);
//      if (nk_button_label(&ctx, "button")) {
//          // event handling
//      }
//      // fixed widget window ratio width
//      nk_layout_row_dynamic(&ctx, 30, 2);
//      if (nk_option_label(&ctx, "easy", op == EASY)) op = EASY;
//      if (nk_option_label(&ctx, "hard", op == HARD)) op = HARD;
//      // custom widget pixel width
//      nk_layout_row_begin(&ctx, NK_STATIC, 30, 2);
//      {
//          nk_layout_row_push(&ctx, 50);
//          nk_label(&ctx, "Volume:", NK_TEXT_LEFT);
//          nk_layout_row_push(&ctx, 110);
//          nk_slider_float(&ctx, 0, &value, 1.0f, 0.1f);
//      }
//      nk_layout_row_end(&ctx);
//  }
//  nk_end(&ctx);
	

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEBUG_OUTPUT); // probably the greatest thing ever.
	glDebugMessageCallback(MessageCallback, 0);

	printf("%s\n", glGetString(GL_VERSION)); //print GL version

	float positions[] = { //the vertecies for the square we want to render, as well as texture cordinates
		000.0f, 000.0f, 0.0f, 0.0f, 0.0f,
		100.0f, 000.0f, 0.0f, 1.0f, 0.0f,
		100.0f, 100.0f, 0.0f, 1.0f, 1.0f,
		000.0f, 100.0f, 0.0f, 0.0f, 1.0f
	};

	unsigned int indicies[] = { //define positions to use for rendering two triangles to make a square
		0, 1, 2,
		2, 3, 0
	};

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	struct VertexArray vao;
	initVertexArray(&vao);

	struct VertexBuffer vb;
	initVertexBuffer(&vb, positions, sizeof(float) * 4 * 5);

	struct VertexBufferLayout vbl;
	initVertexBufferLayout(&vbl);

	pushVertexBufferLayout(&vbl, GL_FLOAT, 3, GL_FALSE);
	pushVertexBufferLayout(&vbl, GL_FLOAT, 2, GL_FALSE);

	vertexArrayAddBuffer(&vao, &vb, &vbl);

	//deleteVertexBufferLayout(&vbl);
	
	struct IndexBuffer ib;
	initIndexBuffer(&ib, indicies, 6);
	
	//create shader from source
	
	vec3 translationA  = {00, 00, -200};
	vec3 translationB  = {30, 00, -150};

	vec3 camera = {0, 0, 0};
	vec3 negCam = {0, 0, 0};
	float xrot = 0.0f;
	float yrot = 0.0f;

	mat4 proj;
    glm_perspective(90.0f, SCREEN_RATIO, 20.00f, 1000.0f, proj);
	mat4 view;
	mat4 model;

	mat4 mvp;
	glm_mat4_mulN((mat4 *[]){&proj, &view, &model}, 3, mvp);

	struct Shader shader;
	initShader(&shader);
	addShaderPath(&shader, "shader/BasicVertex.shader", GL_VERTEX_SHADER);
	addShaderPath(&shader, "shader/BasicFragment.shader", GL_FRAGMENT_SHADER);
	compileShader(&shader);
	bindShader(shader);

	struct Texture texture;
	initTexture(&texture, "resources/textures/source-test.jpeg");
	bindTextureID(texture, 0);
	shaderSetUniform1i(&shader, "u_Texture", 0);

	shaderSetUniformMat4f(&shader, "u_MVP", mvp);

	unbindVertexArray(vao);
	unbindShader(shader);
	unbindIndexBuffer(ib);

	float r = 0.0f;
	float increment = 0.05f;

	while(!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS){ //loop for program
		rendererClear();

		//wasd
		if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
			glm_vec3_sub(camera, (vec3){0.0f, 0.0f, 1.0f}, camera);
		}
		if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
			glm_vec3_sub(camera, (vec3){0.0f, 0.0f, -1.0f}, camera);
		}
		if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
			glm_vec3_sub(camera, (vec3){-1.0f, 0.0f, 0.0f}, camera);
		} 
		if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
			glm_vec3_sub(camera, (vec3){1.0f, 0.0f, 0.0f}, camera);
		} 
		glm_vec3_negate_to(camera, negCam);
		glm_translate_to((mat4)GLM_MAT4_IDENTITY_INIT, negCam, view);


		//arrow camera
		if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
			yrot -= 0.5f;
		}
		if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
			yrot += 0.5f;
		}
		if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
			xrot += 0.5f;
		} 
		if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
			xrot -= 0.5f;
		} 
		glm_rotate_at(view, camera, glm_rad(yrot), (vec3){1.0f, 0.0f, 0.0f});
		glm_rotate_at(view, camera, glm_rad(xrot), (vec3){0.0f, 1.0f, 0.0f});


		glm_translate_to((mat4)GLM_MAT4_IDENTITY_INIT, translationA, model);

		glm_mat4_mulN((mat4 *[]){&proj, &view, &model}, 3, mvp);
		shaderSetUniformMat4f(&shader, "u_MVP", mvp);
		rendererDraw(vao, ib, shader);

		glm_translate_to((mat4)GLM_MAT4_IDENTITY_INIT, translationB, model);
		glm_mat4_mulN((mat4 *[]){&proj, &view, &model}, 3, mvp);
		shaderSetUniformMat4f(&shader, "u_MVP", mvp);
		rendererDraw(vao, ib, shader);
		
		glfwSwapBuffers(window); //swap the buffer
		glfwPollEvents(); //poll for events
	}

	deleteShader(&shader);
	glfwTerminate(); //kill GLFW
}
