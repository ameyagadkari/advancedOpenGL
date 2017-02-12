#version 420 core

layout( std140, binding = 0 ) uniform materialBuffer
{
	vec3 ambientConstant, diffuseConstant, specularConstant;
	float specularExponent;
};

out vec4 o_color;

void main()
{
	o_color = vec4(1.0f,1.0f,1.0f,1.0f);
}