#version 420 core

layout( location = 0 ) in vec3 i_vertexNormal;
layout( location = 1 ) in vec3 i_fragmentPosition;
layout( location = 2 ) in vec3 i_lightPosition;
layout( location = 3 ) in vec4 i_fragmentPositionLightSpace;

layout( binding = 0 ) uniform sampler2DShadow u_depthMap;

layout( std140, binding = 0 ) uniform materialBuffer
{
	vec4 ambientConstant;
	vec4 diffuseConstant;
	vec4 specularConstant;
	float specularExponent;
	ivec4 textureUnitMask;
};

out vec4 o_color;

uniform bool u_depthShader;

float ShadowCalculation(vec4 fragPosLightSpace,vec3 vertexNormalNormalized,vec3 lightDirection)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
   
   // Transform to [0,1] range
    projCoords = projCoords * 0.5f + 0.5f;
	
	if(projCoords.z > 1.0f)
		return 0.0f;
	
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(u_depthMap, projCoords);//texture(u_depthMap, projCoords.xy).r;
	
	// Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    
	// Bias Calculation
    float bias = max(0.05f * (1.0f - dot(vertexNormalNormalized, lightDirection)), 0.005f);
    
	// Check whether current frag pos is in shadow
    float shadow = (currentDepth - bias) > closestDepth  ? 1.0f : 0.0f;
	
    return shadow;
}

void main()
{
	if(u_depthShader)
	{
		//gl_FragDepth = gl_FragCoord.z;
	}
	else
	{
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
}