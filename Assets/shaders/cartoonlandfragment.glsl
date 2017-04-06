#version 420 core

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
	o_color = vec4(diffuseConstant.rgb, 1.0f);
} 