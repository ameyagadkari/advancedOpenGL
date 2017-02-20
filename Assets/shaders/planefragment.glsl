#version 420 core

layout( location = 0 ) in vec2 i_UV;

layout( binding = 0 ) uniform sampler2D u_texture_diffuse;

layout( std140, binding = 0 ) uniform materialBuffer
{
	vec4 ambientConstant;
	vec4 diffuseConstant;
	vec4 specularConstant;
	float specularExponent;
};

out vec4 o_color;

void main()
{
	vec3 diffuseColor = (texture2D(u_texture_diffuse, i_UV)).rgb;
	vec3 result = (diffuseColor.r == 0.0f && diffuseColor.g == 0.0f && diffuseColor.b == 0.0f) ? diffuseConstant.rgb : diffuseColor;
	o_color = vec4(result, 1.0f);
}