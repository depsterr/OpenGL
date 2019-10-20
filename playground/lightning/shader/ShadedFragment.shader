#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_FragPos;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform sampler2D u_Texture;
uniform float ambientStrength;
uniform float specularStrength;

void main()
{
	vec4 texColor = texture(u_Texture, v_TexCoord);

	vec3 ambience = lightColor * ambientStrength;

	vec3 norm = normalize(v_Normal);
	vec3 lightdir = normalize(lightPos - v_FragPos);
	float diff = max(dot(norm, lightdir), 0.0);
	vec3 diffuse = lightColor * diff;

	vec3 viewDir = normalize(viewPos - v_FragPos);
	vec3 reflectDir = reflect(-lightdir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
	vec3 specular = specularStrength * spec * lightColor;

	vec4 result = vec4(ambience + diffuse + specular, 1.0f) * texColor;
	color = result;
};
