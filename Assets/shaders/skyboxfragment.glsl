#version 420

layout( location = 0 ) in vec3 i_texCoords;

uniform samplerCube u_skybox;

out vec4 o_color;

void main()
{
	vec3 cubeColor = (texture(u_skybox, i_texCoords)).rgb;
	o_color = vec4(cubeColor, 1.0f);
}