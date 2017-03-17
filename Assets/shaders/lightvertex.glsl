#version 420 core

layout( location = 0 ) in vec3 i_vertexPosition_local;
layout( location = 1 ) in vec3 i_vertexNormal;

layout( std140, binding = 1 ) uniform drawcallBuffer
{
	mat4 model;
	mat4 view;
	mat4 projection;
};

void main()
{
	vec4 vertexPosition_local = vec4( i_vertexPosition_local, 1.0 );
	vec4 vertexPosition_world = model * vertexPosition_local;
	vec4 vertexPosition_camera = view * vertexPosition_world;
	gl_Position = projection * vertexPosition_camera;
}