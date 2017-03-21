#version 420 core

layout( location = 0 ) in vec3 i_vertexNormal;
layout( location = 1 ) in vec3 i_fragmentPosition;
layout( location = 2 ) in vec3 i_lightPosition;
layout( location = 3 ) in vec2 i_UV;
layout( location = 4 ) in vec4 i_fragmentPositionLightSpace;

//layout( binding = 0 ) uniform sampler2D u_diffuseMap;
//layout( binding = 1 ) uniform sampler2D u_specularMap;
layout( binding = 0 ) uniform sampler2D u_depthMap;

layout( std140, binding = 0 ) uniform materialBuffer
{
	vec4 ambientConstant;
	vec4 diffuseConstant;
	vec4 specularConstant;
	float specularExponent;
};

//uniform vec3 viewPosition;

out vec4 o_color;

uniform float u_near_plane;
uniform float u_far_plane;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0;
    return (2.0 * u_near_plane * u_far_plane) / (u_far_plane + u_near_plane - z * (u_far_plane - u_near_plane));
}

float ShadowCalculation(vec4 fragPosLightSpace,vec3 vertexNormalNormalized,vec3 lightDirection)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(u_depthMap, projCoords.xy).r; 
	// Linearzing Depth
	float closestDepthLinearized = LinearizeDepth(closestDepth) / u_far_plane;	
    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // Bias Calculation
    float bias = max(0.05f * (1.0f - dot(vertexNormalNormalized, lightDirection)), 0.005f);
    // Check whether current frag pos is in shadow
    float shadow = (currentDepth - bias) > closestDepthLinearized  ? 1.0 : 0.0;
	
    return shadow;
}

void main()
{             
	//float depthValue = texture(u_depthMap, i_UV).r;
	//o_color = vec4(vec3(LinearizeDepth(depthValue) / u_far_plane), 1.0); 
	
	//vec3 ambientColor = (texture(u_diffuseMap, i_UV)).rgb;
	//vec3 diffuseColor = ambientColor;
	//vec3 specularColor = (texture(u_specularMap, i_UV)).rgb;
	
	//Normalizing the normal
	vec3 vertexNormalNormalized = normalize(i_vertexNormal);

	//Ambient light
	vec3 ambient = ambientConstant.rgb; 

	//Diffuse light
	vec3 lightDirection = normalize(i_lightPosition - i_fragmentPosition); 
	float cos_theta_diffuse = max(dot(vertexNormalNormalized, lightDirection), 0.0f);
	vec3 diffuse = cos_theta_diffuse * diffuseConstant.rgb;

	//Specular light
	vec3 viewDirection = normalize(-i_fragmentPosition);
	vec3 halfwayDirection = normalize(lightDirection + viewDirection);
	float cos_theta_specular = pow(max(dot(vertexNormalNormalized, halfwayDirection), 0.0f), specularExponent);
	vec3 specular = cos_theta_diffuse * cos_theta_specular * specularConstant.rgb;
	
	// Calculate shadow
    float shadow = ShadowCalculation(i_fragmentPositionLightSpace,vertexNormalNormalized,lightDirection);       
    vec3 result = ambient + ((1.0 - shadow) * (diffuse + specular));    

	//vec3 result = ambient + diffuse + specular;
	o_color = vec4(result, 1.0f);
	
} 