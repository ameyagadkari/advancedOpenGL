#version 420 core

layout( location = 0 ) in vec4 i_clipSpaceCoords;
layout( location = 1 ) in vec2 i_UV;
layout( location = 2 ) in vec3 i_toCameraVector;
layout( location = 3 ) in vec3 i_vertexNormal;
layout( location = 4 ) in vec3 i_fromLightVector;

layout( binding = 2 ) uniform sampler2D u_normalTexture;
layout( binding = 5 ) uniform sampler2D u_dudvTexture;
layout( binding = 10 ) uniform sampler2D u_reflectionTexture;
layout( binding = 11 ) uniform sampler2D u_refractionTexture;

uniform float u_moveFactor;
uniform vec3 u_lightColor;

float waveStrength = 0.02;
float shineDamper = 20.0;
float reflectivity = 0.6;
vec4 greenishBlueTint = vec4(0.0, 0.3, 0.5, 1.0);

out vec4 o_color;

void main()
{             
	vec2 normalizedDeviceCoords = (i_clipSpaceCoords.xy/i_clipSpaceCoords.w)*0.5+0.5;
	
	vec2 reflectionTextureCoords = vec2(normalizedDeviceCoords.x,-normalizedDeviceCoords.y);
	vec2 refractionTextureCoords = vec2(normalizedDeviceCoords.x,normalizedDeviceCoords.y);
	
	/*vec2 distortion1 = (texture(u_dudvTexture,vec2(i_UV.x + u_moveFactor, i_UV.y)).rg * 2.0 - 1.0) * waveStrength;
	vec2 distortion2 = (texture(u_dudvTexture,vec2(-i_UV.x + u_moveFactor, i_UV.y + u_moveFactor)).rg * 2.0 - 1.0) * waveStrength;
	vec2 totalDistortion = distortion1 + distortion2;*/
	
	vec2 distortedUVs = texture(u_dudvTexture, vec2(i_UV.x + u_moveFactor,
	i_UV.y)).rg*waveStrength*5.0;
	distortedUVs = i_UV + vec2(distortedUVs.x, distortedUVs.y + u_moveFactor);
	vec2 totalDistortion = (texture(u_dudvTexture, distortedUVs).rg * 2.0 - 1.0) * waveStrength;
	
	reflectionTextureCoords += totalDistortion;
	reflectionTextureCoords.x = clamp(reflectionTextureCoords.x, 0.001, 0.999);
	reflectionTextureCoords.y = clamp(reflectionTextureCoords.y, -0.999, -0.001);
	
	refractionTextureCoords += totalDistortion;
	refractionTextureCoords = clamp(refractionTextureCoords, 0.001, 0.999);
	
	vec4 reflectionColor = texture(u_reflectionTexture, reflectionTextureCoords);
	vec4 refractionColor = texture(u_refractionTexture, refractionTextureCoords);
	
	vec3 viewVector = normalize(i_toCameraVector);
	vec3 normalizedNormal = normalize(i_vertexNormal);
	float refractiveFactor = dot(viewVector,normalizedNormal);
	refractiveFactor = pow(refractiveFactor, 0.5);
	
	vec4 normalColor = texture(u_normalTexture, distortedUVs);
	vec3 normal = vec3(normalColor.r * 2.0 - 1.0, normalColor.b, normalColor.g * 2.0 - 1.0);
	normal = normalize(normal);
	
	vec3 reflectedLight = reflect(normalize(i_fromLightVector), normal);
	float specular =  pow(max(dot(reflectedLight, viewVector), 0.0), shineDamper);
	
	/*vec3 halfwayDirection = normalize(i_fromLightVector + viewVector);
	float specular = pow(max(dot(normal, halfwayDirection), 0.0f), shineDamper);*/
	
	vec3 specularHighlights = u_lightColor * specular * reflectivity;
	
	o_color = mix(reflectionColor, refractionColor, refractiveFactor);
	o_color = mix(o_color, greenishBlueTint, 0.2) + vec4(specularHighlights, 0.0);
	//o_color = vec4(normalColor.rgb, 1.0);
} 