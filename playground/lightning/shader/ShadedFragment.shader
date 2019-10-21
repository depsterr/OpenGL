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
uniform float diffuseStrength;
uniform float distanceStrength;

void main()
{
	//texture
	vec4 texColor = texture(u_Texture, v_TexCoord);

	//ambience
	vec3 ambience = lightColor * ambientStrength;

	//diffuse
	vec3 norm = normalize(v_Normal);
	vec3 lightdir = normalize(lightPos - v_FragPos);
	float diff = max(dot(norm, lightdir), 0.0);
	vec3 diffuse = lightColor * diff * diffuseStrength;

	//specular
	vec3 viewDir = normalize(viewPos - v_FragPos);
	vec3 reflectDir = reflect(-lightdir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
	vec3 specular = specularStrength * spec * lightColor;

	//distance
	float lightdist = distance(lightPos, v_FragPos) ;
	lightdist = 1 / lightdist;
	vec3 distance = distanceStrength * lightdist * lightColor;

	vec4 result = vec4(ambience + diffuse + specular, 1.0f) * vec4(distance, 1.0f) * texColor;
	color = result;
};
