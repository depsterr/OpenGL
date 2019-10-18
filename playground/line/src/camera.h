#pragma once

#include <cglm/cglm.h>
#include <math.h>

struct Camera{
	vec3 position;
    vec3 front;
    vec3 up;
    vec3 right;
    vec3 worldUp;
    float yaw;
    float pitch;
	mat4 view;
};

void initCamera(struct Camera *camera);

void cameraUpdateViewMatrix(struct Camera *camera);

void cameraUpdateVectors(struct Camera *camera);

void cameraMoveRelativeX(struct Camera *camera, float amount);

void cameraMoveRelativeY(struct Camera *camera, float amount);

void cameraMoveRelativeZ(struct Camera *camera, float amount);
