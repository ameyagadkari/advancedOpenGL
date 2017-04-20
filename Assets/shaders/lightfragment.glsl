#version 420 core

uniform vec3 u_lightColor;

out vec4 o_color;

void main()
{
	o_color = vec4(u_lightColor, 1.0);
}