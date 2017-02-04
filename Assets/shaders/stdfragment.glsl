#version 420 core

layout( location = 0 ) in vec3 i_vertexNormal;

out vec4 o_color;

void main()
{
	o_color = vec4( i_vertexNormal, 1.0 );
}