#version 420 core

layout( location = 0 ) in vec3 i_vertexNormal;
layout( location = 1 ) in vec3 i_fragmentPosition;
layout( location = 2 ) in vec3 i_lightPosition;
layout( location = 3 ) in vec2 i_UV;

layout( binding = 0 ) uniform sampler2D u_diffuse;

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
	vec3 ambientColor = texture(u_diffuse, i_UV).rgb;
	vec3 diffuseColor = ambientColor;
	
	//Ambient light
	vec3 ambient = ambientConstant.rgb * ambientColor;

	//Normalizing the normal
	vec3 vertexNormalNormalized = normalize(i_vertexNormal);

	//Diffuse light
	vec3 lightDirection = normalize(i_lightPosition - i_fragmentPosition); 
	float cosThetaDiffuse = max(dot(vertexNormalNormalized, lightDirection), 0.0f);
	vec3 diffuse = cosThetaDiffuse *  diffuseColor;

	o_color = vec4((ambient + diffuse), 1.0f);
} 