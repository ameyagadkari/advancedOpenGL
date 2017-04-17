#version 420 core

layout( location = 0 ) in vec3 i_vertexPosition_local;
layout( location = 1 ) in vec3 i_vertexNormal;
layout( location = 2 ) in vec2 i_UV;

layout( location = 0 ) out vec4 o_clipSpaceCoords;
layout( location = 1 ) out vec2 o_UV;

layout( std140, binding = 1 ) uniform drawcallBuffer
{
	mat4 model;
	mat4 view;
	mat4 projection;
	mat4 lightSpaceMatrix;
};

float tiling = 6.0;

void main()
{	
	vec4 vertexPosition_local = vec4( i_vertexPosition_local, 1.0 );
	vec4 vertexPosition_world = model * vertexPosition_local;
	vec4 vertexPosition_camera = view * vertexPosition_world;
	o_clipSpaceCoords = projection * vertexPosition_camera;	
	gl_Position = o_clipSpaceCoords;
	
	o_UV = vec2(i_vertexPosition_local.x*0.5+0.5,i_vertexPosition_local.z*0.5+0.5);
}