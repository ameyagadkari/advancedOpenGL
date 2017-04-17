#version 420 core

layout( location = 0 ) in vec3 i_vertexPosition_local;

layout( location = 0 ) out vec3 o_texCoords;

layout( std140, binding = 1 ) uniform drawcallBuffer
{
	mat4 model;
	mat4 view;
	mat4 projection;
	mat4 viewInv;
};

uniform vec4 u_clippingplane;

void main()
{
		vec4 vertexPosition_local = vec4( i_vertexPosition_local, 1.0 );
		vec4 vertexPosition_camera = view * vertexPosition_local;
		gl_ClipDistance[0] = dot(vertexPosition_local,u_clippingplane);
		gl_Position = projection * vertexPosition_camera;
		
		o_texCoords = i_vertexPosition_local;
}