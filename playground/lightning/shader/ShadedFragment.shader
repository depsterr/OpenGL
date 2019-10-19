#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;
uniform float ambientStrength;

void main()
{
	vec4 outColor = texture(u_Texture, v_TexCoord) * vec4(vec3(ambientStrength), 1.0f);
	color = outColor;
};
