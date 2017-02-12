#version 420 core

layout( location = 0 ) in vec3 i_vertexNormal;
layout( location = 1 ) in vec3 i_fragmentPosition;
layout( location = 2 ) in vec3 i_lightPosition;
layout( location = 3 ) in vec2 i_UV;

layout( binding = 0 ) uniform sampler2D u_texture_diffuse;
layout( binding = 1 ) uniform sampler2D u_texture_specular;

layout( std140, binding = 0 ) uniform materialBuffer
{
	vec3 ambientConstant;
	vec3 diffuseConstant;
	vec3 specularConstant;
	float specularExponent;
};

out vec4 o_color;

void main()
{
	//Hardcoded Values for ambient light
	//vec3 ambientStrength = vec3(0.50980395,0.00000000,0.00000000);
	vec3 ambientColor = vec3(texture2D(u_texture_diffuse, i_UV));

	//Hardcoded Values for diffuse light
	//vec3 diffuseStrength = vec3(0.50980395,0.00000000,0.00000000);
	vec3 diffuseColor = ambientColor;

	//Hardcoded Values for specular light
	//vec3 specularStrength = vec3(0.80099994,0.80099994,0.80099994);
	vec3 specularColor = vec3(texture2D(u_texture_specular, i_UV));
	//int glossiness = 20;

	//Normalizing the normal
	vec3 vertexNormalNormalized = normalize(i_vertexNormal);

	//Ambient light
	vec3 ambient = ambientConstant * ambientColor;

	//Diffuse light
	//vec3 norm = normalize(ourNormal);
	vec3 lightDirection = normalize(i_lightPosition - i_fragmentPosition); 
	float cos_theta_diffuse = max(dot(vertexNormalNormalized, lightDirection), 0.0f);
	vec3 diffuse = cos_theta_diffuse * diffuseConstant *  diffuseColor;

	//Specular light
	vec3 viewDirection = normalize(-i_fragmentPosition);
	vec3 halfwayDirection = normalize(lightDirection + viewDirection);
	float cos_theta_specular = pow(max(dot(vertexNormalNormalized, halfwayDirection), 0.0f), specularExponent);
	vec3 specular = cos_theta_diffuse * cos_theta_specular * specularConstant * specularColor; 

	vec3 result = ambient + diffuse + specular;
	o_color = vec4(result, 1.0f);
}