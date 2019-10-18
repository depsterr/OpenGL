#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in mat4 instanceModel;

out vec2 v_TexCoord;

uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * instanceModel * position;
	v_TexCoord = texCoord;
};
