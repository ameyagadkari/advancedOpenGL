#version 420 core

layout( location = 0 ) in vec4 i_clipSpaceCoords;
layout( location = 1 ) in vec2 i_UV;

layout( binding = 5 ) uniform sampler2D u_dudvTexture;
layout( binding = 10 ) uniform sampler2D u_reflectionTexture;
layout( binding = 11 ) uniform sampler2D u_refractionTexture;

uniform float moveFactor;

float waveStrength = 0.02;
vec4 greenishBlueTint = vec4(0.0, 0.3, 0.5, 1.0);

out vec4 o_color;

void main()
{             
	vec2 normalizedDeviceCoords = (i_clipSpaceCoords.xy/i_clipSpaceCoords.w)*0.5+0.5;
	
	vec2 reflectionTextureCoords = vec2(normalizedDeviceCoords.x,-normalizedDeviceCoords.y);
	vec2 refractionTextureCoords = vec2(normalizedDeviceCoords.x,normalizedDeviceCoords.y);
	
	vec2 distortion1 = ((texture(u_dudvTexture,vec2(i_UV.x + moveFactor, i_UV.y))).rg * 2.0 - 1.0) * waveStrength;
	vec2 distortion2 = ((texture(u_dudvTexture,vec2(-i_UV.x + moveFactor, i_UV.y + moveFactor))).rg * 2.0 - 1.0) * waveStrength;
	vec2 totalDistortion = distortion1 + distortion2;
	
	reflectionTextureCoords += totalDistortion;
	reflectionTextureCoords.x = clamp(reflectionTextureCoords.x, 0.001, 0.999);
	reflectionTextureCoords.y = clamp(reflectionTextureCoords.y, -0.999, -0.001);
	
	refractionTextureCoords += totalDistortion;
	refractionTextureCoords = clamp(refractionTextureCoords, 0.001, 0.999);
	
	
	vec4 reflectionColor = texture(u_reflectionTexture, reflectionTextureCoords);
	vec4 refractionColor = texture(u_refractionTexture, refractionTextureCoords);
	
	o_color = mix(reflectionColor,refractionColor,0.5);
	o_color = mix(o_color, greenishBlueTint, 0.2);
} 