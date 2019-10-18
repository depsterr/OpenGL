#include "camera.h"

void initCamera(struct Camera *camera){
	glm_vec3_copy(GLM_VEC3_ZERO, camera->position);
	glm_vec3_copy((vec3){0.0f, 0.0f, -1.0f}, camera->front);
	glm_vec3_copy(GLM_VEC3_ZERO, camera->up);
	glm_vec3_copy(GLM_VEC3_ZERO, camera->right);
	glm_vec3_copy((vec3){0.0f, 1.0f, 0.0f}, camera->worldUp);
    camera->yaw = -90.0f;
    camera->pitch = 0.0f;
	glm_mat4_copy(GLM_MAT4_IDENTITY, camera->view);
	cameraUpdateVectors(camera);
}

void cameraUpdateViewMatrix(struct Camera *camera){
	cameraUpdateVectors(camera);
	vec3 lookat;
	glm_vec3_add(camera->position, camera->front, lookat);
	glm_lookat(camera->position, lookat, camera->up, camera->view);
}

void cameraUpdateVectors(struct Camera *camera){
		vec3 front;
        front[0] = cos(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
        front[1] = sin(glm_rad(camera->pitch));
        front[2] = sin(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
		glm_vec3_normalize(front);
		glm_vec3_copy(front, camera->front);
		glm_vec3_cross(camera->front, camera->worldUp, camera->right);
		glm_vec3_normalize(camera->right);
		glm_vec3_cross(camera->right, camera->front, camera->up);
		glm_vec3_normalize(camera->up);
}

void cameraMoveRelativeX(struct Camera *camera, float amount){
	vec3 res;
	glm_vec3_scale(camera->right, amount, res);
	glm_vec3_add(camera->position, res, camera->position);
}

void cameraMoveRelativeY(struct Camera *camera, float amount){
	vec3 res;
	glm_vec3_scale(camera->up, amount, res);
	glm_vec3_add(camera->position, res, camera->position);
}

void cameraMoveRelativeZ(struct Camera *camera, float amount){
	vec3 res;
	glm_vec3_scale(camera->front, amount, res);
	glm_vec3_add(camera->position, res, camera->position);
}

