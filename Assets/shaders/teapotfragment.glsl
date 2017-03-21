#version 420 core

layout( location = 0 ) in vec3 i_vertexNormal;
layout( location = 1 ) in vec3 i_fragmentPosition;
layout( location = 2 ) in vec3 i_lightPosition;

layout( std140, binding = 0 ) uniform materialBuffer
{
	vec4 ambientConstant;
	vec4 diffuseConstant;
	vec4 specularConstant;
	float specularExponent;
	ivec4 textureUnitMask;
};

uniform bool u_depthShader;
//uniform vec3 viewPosition;

out vec4 o_color;

/*float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // Transform to [0,1] range
    //projCoords = projCoords * 0.5 + 0.5;
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(u_depthMap, projCoords); 
    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
	// Bias Calculation
	vec3 vertexNormalNormalized = normalize(i_vertexNormal);
    vec3 lightDirection = normalize(i_lightPosition - i_fragmentPosition);
    float bias = max(0.05f * (1.0f - dot(vertexNormalNormalized, lightDirection)), 0.005f);
    // Check whether current frag pos is in shadow
    float shadow = (currentDepth - bias) > closestDepth  ? 1.0f : 0.0f;

    return shadow;
}*/

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

		vec3 result = ambient + diffuse + specular;
		o_color = vec4(result, 1.0f);
	}
}