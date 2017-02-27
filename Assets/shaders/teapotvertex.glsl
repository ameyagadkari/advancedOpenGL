#version 420 core

layout( location = 0 ) in vec3 i_vertexPosition_local;
layout( location = 1 ) in vec3 i_vertexNormal;

layout( location = 0 ) out vec3 o_vertexNormal;
layout( location = 1 ) out vec3 o_fragmentPosition;
layout( location = 2 ) out vec3 o_lightPosition;

layout( std140, binding = 1 ) uniform drawcallBuffer
{
	mat4 model;
	mat4 view;
	mat4 projection;
	mat4 viewInv;
};

uniform mat3 u_normal;
uniform vec3 u_lightPosition;

void main()
{
	
		vec4 vertexPosition_local = vec4( i_vertexPosition_local, 1.0 );
		vec4 vertexPosition_world = model * vertexPosition_local;
		vec4 vertexPosition_camera = view * vertexPosition_world;
		gl_Position = projection * vertexPosition_camera;
	
		o_fragmentPosition = vec3(vertexPosition_camera);
		o_vertexNormal = u_normal*i_vertexNormal;
		o_lightPosition =  vec3(view * vec4(u_lightPosition, 1.0));
}