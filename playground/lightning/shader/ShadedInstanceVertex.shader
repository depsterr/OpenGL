#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in mat4 instanceModel;

out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_FragPos;

uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * instanceModel * position;
	v_TexCoord = texCoord;
	v_FragPos = vec3(instanceModel * position);
	v_Normal = normal;
};
