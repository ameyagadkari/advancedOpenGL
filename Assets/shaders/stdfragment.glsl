#version 420 core

layout( location = 0 ) in vec3 i_vertexNormal;
layout( location = 1 ) in vec3 i_fragmentPosition;
layout( location = 2 ) in vec3 i_lightPosition;
layout( location = 3 ) in vec2 i_UV;

layout( binding = 0 ) uniform sampler2D u_texture_diffuse;
layout( binding = 1 ) uniform sampler2D u_texture_specular;

out vec4 o_color;

void main()
{
	//Hardcoded Values for ambient light
	float ambientStrength = 0.1f;
	vec3 ambientColor = vec3(texture2D(u_texture_diffuse, i_UV));

	//Hardcoded Values for diffuse light
	float diffuseStrength = 1.0f;
	vec3 diffuseColor = ambientColor;

	//Hardcoded Values for specular light
	float specularStrength = 0.5f;
	vec3 specularColor = vec3(texture2D(u_texture_specular, i_UV));
	int glossiness = 32;

	//Normalizing the normal
	vec3 vertexNormalNormalized = normalize(i_vertexNormal);

	//Ambient light
	vec3 ambient = ambientStrength * ambientColor;

	//Diffuse light
	//vec3 norm = normalize(ourNormal);
	vec3 lightDirection = normalize(i_lightPosition - i_fragmentPosition); 
	float cos_theta_diffuse = max(dot(vertexNormalNormalized, lightDirection), 0.0f);
	vec3 diffuse = diffuseStrength * cos_theta_diffuse * diffuseColor;

	//Specular light
	vec3 viewDirection = normalize(-i_fragmentPosition);
	vec3 halfwayDirection = normalize(lightDirection + viewDirection);
	float cos_theta_specular = pow(max(dot(vertexNormalNormalized, halfwayDirection), 0.0f), glossiness);
	vec3 specular = cos_theta_diffuse * specularStrength * cos_theta_specular * specularColor; 

	vec3 result = ambient + diffuse + specular;
	o_color = vec4(result, 1.0f);
}