#version 420 core

layout( location = 0 ) in vec3 i_vertexNormal;
layout( location = 1 ) in vec3 i_fragmentPosition;
//layout( location = 2 ) in vec2 i_UV;

layout( binding = 1 ) uniform sampler2DRect u_texture;

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
	vec3 vertexNormalNormalized = normalize(i_vertexNormal);
	vec3 viewDirection = normalize(-i_fragmentPosition);
	vec3 reflectDir = (viewInv*vec4(2.0f*(dot(vertexNormalNormalized, viewDirection))*vertexNormalNormalized-viewDirection,1.0f)).xyz;
	vec3 reflectedEnv = (texture(u_skybox, reflectDir)).rgb;
	vec4 diffuseColor = (texture(u_texture,gl_FragCoord.xy));
	o_color = (diffuseColor.a == 0.0f) ? vec4(reflectedEnv.rgb,1.0f) : vec4(diffuseColor.rgb,1.0f);
}