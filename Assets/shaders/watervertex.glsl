#version 420 core

layout( location = 0 ) in vec3 i_vertexPosition_local;
layout( location = 1 ) in vec3 i_vertexNormal;
layout( location = 2 ) in vec2 i_UV;

layout( location = 0 ) out vec4 o_clipSpaceCoords;
layout( location = 1 ) out vec2 o_UV;
layout( location = 2 ) out vec3 o_toCameraVector;
layout( location = 3 ) out vec3 o_fromLightVector;
layout( location = 4 ) out vec3 o_fragmentPosition;
layout( location = 5 ) out vec3 o_lightPosition;

layout( std140, binding = 1 ) uniform drawcallBuffer
{
	mat4 model;
	mat4 view;
	mat4 projection;
	mat4 lightSpaceMatrix;
};

uniform vec3 u_cameraPosition;
uniform vec3 u_lightPosition;

float tiling = 6.0;

void main()
{	
	vec4 vertexPosition_local = vec4( i_vertexPosition_local, 1.0 );
	vec4 vertexPosition_world = model * vertexPosition_local;
	vec4 vertexPosition_camera = view * vertexPosition_world;
	gl_Position = projection * vertexPosition_camera;
	
	o_clipSpaceCoords = gl_Position;
	o_UV = i_UV * tiling;
	o_toCameraVector = u_cameraPosition - vertexPosition_world.xyz;
	o_fromLightVector =  vertexPosition_world.xyz - u_lightPosition;
	o_fragmentPosition = vec3(vertexPosition_camera);
	o_lightPosition =  vec3(view * vec4(u_lightPosition, 1.0));
}