#version 420 core

layout( location = 0 ) in vec3 i_vertexNormal;
layout( location = 1 ) in vec3 i_fragmentPosition;
layout( location = 2 ) in vec3 i_lightPosition;

uniform samplerCube u_skybox;

layout( std140, binding = 1 ) uniform drawcallBuffer
{
	mat4 model;
	mat4 view;
	mat4 projection;
	mat4 viewInv;
};

out vec4 o_color;

void main()
{
	//Normalizing the normal
	vec3 vertexNormalNormalized = normalize(i_vertexNormal);

	//Diffuse light
	vec3 lightDirection = normalize(i_lightPosition - i_fragmentPosition); 
	float cos_theta_diffuse = max(dot(vertexNormalNormalized, lightDirection), 0.0f);
	vec3 diffuse = cos_theta_diffuse * vec3(1.0f,1.0f,1.0f);

	//Specular light
	vec3 viewDirection = normalize(-i_fragmentPosition);
	vec3 halfwayDirection = normalize(lightDirection + viewDirection);
	float cos_theta_specular = pow(max(dot(vertexNormalNormalized, halfwayDirection), 0.0f), 20);
	vec3 specular = cos_theta_diffuse * cos_theta_specular * vec3(1.0f,1.0f,1.0f);

	vec3 reflectDir = (viewInv*vec4(2.0f*(dot(vertexNormalNormalized, viewDirection))*vertexNormalNormalized-viewDirection,1.0f)).xyz;
	vec3 result = diffuse + specular + (texture(u_skybox, reflectDir)).rgb;
	o_color = vec4(result, 1.0f);
}