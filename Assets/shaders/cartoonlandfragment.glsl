#version 420 core

layout( location = 0 ) in vec3 i_vertexNormal;
layout( location = 1 ) in vec3 i_fragmentPosition;
layout( location = 2 ) in vec3 i_lightPosition;

layout( std140, binding = 0 ) uniform materialBuffer
{
	vec4 ambientConstant;
	vec4 diffuseConstant;
	vec4 specularConstant;
	float specularExponent;
	ivec4 textureUnitMask;
};

out vec4 o_color;

void main()
{          
	vec3 ambientColor = diffuseConstant.rgb * 0.1;
	vec3 diffuseColor = diffuseConstant.rgb;
	vec3 specularColor = specularConstant.rgb;

	//Normalizing the normal
	vec3 vertexNormalNormalized = normalize(i_vertexNormal);

	//Diffuse light
	vec3 lightDirection = normalize(i_lightPosition - i_fragmentPosition); 
	float cosThetaDiffuse = max(dot(vertexNormalNormalized, lightDirection), 0.0f);
	vec3 diffuse = cosThetaDiffuse *  diffuseColor;

	//Specular light
	vec3 viewDirection = normalize(-i_fragmentPosition);
	vec3 halfwayDirection = normalize(lightDirection + viewDirection);
	float cosThetaSpecular = pow(max(dot(vertexNormalNormalized, halfwayDirection), 0.0f), specularExponent);
	vec3 specular = cosThetaDiffuse * cosThetaSpecular * specularColor;

	o_color = vec4((ambientColor + diffuse + specular), 1.0f);
} 