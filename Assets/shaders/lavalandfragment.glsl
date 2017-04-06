#version 420 core

layout( location = 2 ) in vec2 i_UV;

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
	vec3 sampledColor = texture(u_diffuse,i_UV).rgb;
	vec3 ambient = sampledColor*ambientConstant.rgb;
	vec3 diffuse = sampledColor*diffuseConstant.rgb;
	
	o_color = vec4((ambient + diffuse),1.0f);
}