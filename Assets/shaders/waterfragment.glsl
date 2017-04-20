#version 420 core

layout( location = 0 ) in vec4 i_clipSpaceCoords;
layout( location = 1 ) in vec2 i_UV;
layout( location = 2 ) in vec3 i_toCameraVector;
layout( location = 3 ) in vec3 i_fromLightVector;
layout( location = 4 ) in vec3 i_fragmentPosition;
layout( location = 5 ) in vec3 i_lightPosition;

layout( binding = 2 ) uniform sampler2D u_normalTexture;
layout( binding = 5 ) uniform sampler2D u_dudvTexture;
layout( binding = 9 ) uniform sampler2D u_refractionDepthTexture;
layout( binding = 10 ) uniform sampler2D u_reflectionTexture;
layout( binding = 11 ) uniform sampler2D u_refractionTexture;

layout( std140, binding = 0 ) uniform materialBuffer
{
	vec4 ambientConstant;
	vec4 diffuseConstant;
	vec4 specularConstant;
	float specularExponent;
	ivec4 textureUnitMask;
};

uniform float u_moveFactor;
uniform float u_nearPlane;
uniform float u_farPlane;

uniform float u_waveStrength;
uniform float u_reflectivity;

uniform vec3 u_lightColor;

vec4 murkyWaterColour = vec4(0.75, 0.58, 0.45, 1.0);

out vec4 o_color;

float LinearizeDepth(float depth)
{
	return (2.0 * u_nearPlane * u_farPlane) / (u_farPlane + u_nearPlane - (depth * 2.0 - 1.0) * (u_farPlane - u_nearPlane));
}

void main()
{             
	vec2 normalizedDeviceCoords = (i_clipSpaceCoords.xy/i_clipSpaceCoords.w)*0.5+0.5;	
	vec2 reflectionTextureCoords = vec2(normalizedDeviceCoords.x,-normalizedDeviceCoords.y);
	vec2 refractionTextureCoords = vec2(normalizedDeviceCoords.x,normalizedDeviceCoords.y);
	
	float waterDepth = LinearizeDepth(texture(u_refractionDepthTexture, refractionTextureCoords).r) - LinearizeDepth(gl_FragCoord.z);
	
	vec2 distortedUVs = texture(u_dudvTexture, vec2(i_UV.x + u_moveFactor, i_UV.y)).rg * u_waveStrength * 5.0;
	distortedUVs = i_UV + vec2(distortedUVs.x, distortedUVs.y + u_moveFactor);
	vec2 totalDistortion = (texture(u_dudvTexture, distortedUVs).rg * 2.0 - 1.0) * u_waveStrength * clamp(waterDepth/20.0, 0.0, 1.0);;
	
	reflectionTextureCoords += totalDistortion;
	reflectionTextureCoords.x = clamp(reflectionTextureCoords.x, 0.001, 0.999);
	reflectionTextureCoords.y = clamp(reflectionTextureCoords.y, -0.999, -0.001);
	
	refractionTextureCoords += totalDistortion;
	refractionTextureCoords = clamp(refractionTextureCoords, 0.001, 0.999);
	
	vec4 reflectionColor = texture(u_reflectionTexture, reflectionTextureCoords);
	vec4 refractionColor = texture(u_refractionTexture, refractionTextureCoords);
	refractionColor = mix(refractionColor, murkyWaterColour, clamp(waterDepth/60.0, 0.0, 1.0));
	
	vec4 normalColor = texture(u_normalTexture, distortedUVs);
	vec3 normal = vec3(normalColor.r * 2.0 - 1.0, normalColor.b * 3.0, normalColor.g * 2.0 - 1.0);
	normal = normalize(normal);
	
	vec3 viewVector = normalize(i_toCameraVector);
	float refractiveFactor = dot(viewVector, normal);
	refractiveFactor = clamp(pow(refractiveFactor, 0.5), 0.0, 1.0);
	
	vec3 lightDirection = normalize(i_lightPosition - i_fragmentPosition); 
	float diffuse = 1.0 - max(dot(normal, lightDirection), 0.0);
	vec3 diffuseHighlights = diffuse * diffuseConstant.rgb;
	
	vec3 viewDirection = normalize(-i_fragmentPosition);	
	vec3 halfwayDirection = normalize(lightDirection + viewDirection);
	float specular = pow(max(dot(normal, halfwayDirection), 0.0f), specularExponent);	
	vec3 specularHighlights = specularConstant.rgb * u_lightColor * specular * u_reflectivity * clamp(waterDepth/5.0, 0.0, 1.0);
	
	o_color = mix(reflectionColor, refractionColor, refractiveFactor);
	o_color = mix(o_color, vec4(diffuseHighlights, 1.0), 0.2) + vec4(specularHighlights, 0.0);
	o_color.a = clamp(waterDepth/5.0, 0.0, 1.0);
} 