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
	vec3 diffuseColor = (diffuseConstant.g == 0.0f && diffuseConstant.b == 0.0f) ? diffuseConstant.r * (texture2D(u_texture_diffuse, i_UV)).rgb : diffuseConstant.rgb * (texture2D(u_texture_diffuse, i_UV)).rgb;
	
	o_color = vec4(diffuseColor, 1.0f);
}