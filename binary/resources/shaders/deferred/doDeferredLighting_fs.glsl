precision mediump float;

#define M_PI 3.1415926535897932384626433832795


in vec2 tcoord;
in vec2 shadowUV;


uniform sampler2D diffuseTex; 				// The color information
uniform sampler2D posTex;     				// World position
uniform sampler2D normalTex;  				// Normals
uniform sampler2D blendTex;   				// A bitmap with colors to blend with.


uniform mat4 viewMatrix;
uniform mat4 depthBiasMatrix;
uniform samplerCube reflectionTextureCube;


uniform vec3 cameraPos;
uniform vec2 shadowMapSize;
uniform float reflection = 0.2;


struct Light {
	vec3 	position;
	vec3 	direction;
	int		type;
	float intensity;
	vec3 	specular;
	vec3 	ambient;
};

uniform Light SoanLight;
uniform ShadowMap SoanShadowMap;

layout(location = 0) out vec4 color0;



vec3 blinn_phong(float ndotl, float ndoth, vec3 specularColor, vec3 diffuse, float fSpecularExponent) {

	// Compute the diffuse term
	float diffuseCoefficient = ndotl ;
	vec3 cdiffuse = diffuseCoefficient * diffuse;


	float specularCoefficient = 0.0;
	if(diffuseCoefficient > 0.0) {
		specularCoefficient = pow(ndoth, fSpecularExponent);
	}

	// Compute the specular colour
	vec3 cspecular = specularCoefficient * specularColor;

	return (cdiffuse + cspecular);
}


vec3 advanced_shading(float roughness, vec3 diffuse, float ndoth, float ndotv, float ndotl, float vdoth) {

	//
	// Microfacet Specular BRDF (Cook-Torrance)
	// No optimization at all. At the moment just to test.
	//

	vec3 	brdf_f0						= SoanLight.specular;
	float brdf_alpha 				= roughness*roughness;

	vec3 brdf_ambient				= SoanLight.ambient;

	//
	// Calculate Normal Distribution Term.
	//
	float brdf_specular_d 	= d_ggx(brdf_alpha, ndoth);

	//
	// Calculate geometric specular term.
	//
//	float brdf_specular_g = g_Implicit(brdf_alpha, ndotv, ndotl);
//	float brdf_specular_g = g_Neumann(brdf_alpha, ndotv, ndotl);
//	float brdf_specular_g = g_CookTorrance(brdf_alpha, ndotv, ndotl, ndoth, vdoth);
//	float brdf_specular_g = g_Kelemen(brdf_alpha, ndotv, ndotl, ndoth);
//	float brdf_specular_g		= g_Beckmann(brdf_alpha, ndotv, ndotl);
	float brdf_specular_g		= g_SchlickBeckmann(brdf_alpha, ndotv, ndotl);

	//
	// Calculate frenel term.
	//
//	vec3 brdf_specular_f		= f_None(brdf_f0, vdoth);
	vec3 brdf_specular_f		= f_Schlick(brdf_f0, vdoth);

	//
	// Put everyting together.
	//
	vec3 color			= diffuse.rgb/M_PI + ((brdf_specular_d * brdf_specular_g) * brdf_specular_f)/(4 * ndotl * ndotv);

	return color;
}



void main() {

	//
	// Get everhting from the G-Buffer.
	//
	vec4 diffuse 		= texture(diffuseTex,		tcoord);
	vec4 blend 			= texture(blendTex, 		tcoord);
	vec4 worldPos 	= texture(posTex, 			tcoord);
	vec4 normal 		= texture(normalTex, 		tcoord);

	//
	// Create all neccesary vectors.
	//
	vec3 n = normalize(normal.xyz);
	vec3 v = normalize(cameraPos - worldPos.xyz);						// Camera has to be represented in world space, so -camPos.
	vec3 l = normalize(vec4(-SoanLight.direction, 0.0)).xyz;	// This is for directional light. We have to inverse the direction to point away from the world pos.
	vec3 h = normalize(l + v);

	float ndoth = max(0.0f, dot(n,h));
	float ndotv = max(0.01f, dot(n,v));
	float ndotl = max(0.0f, dot(n,l));
	float vdoth = max(0.0f, dot(v,h));


	float roughness = normal.w;



	//
	// Shadow stuff.
	//
	float visibility = 1.0;
	visibility = calculateVisibility(SoanShadowMap, ndotl, depthBiasMatrix, worldPos);



	//
	// This is a hack which will be removed.
	//
	if(diffuse.a <= 0) {
		color0 = diffuse;
	} else {

		//
		// Reflection stuff.
		//
	//	vec3 idir = vec3(-v.x, v.y, -v.z);
	//	vec3 reflection = texture(reflectionTextureCube, reflect(idir, -n)).rgb;
//		diffuse.rgb = mix(diffuse.rgb, reflection, 0.0);

		//
		// Shading stuff.
		//
		color0.rgb = max(SoanLight.ambient, visibility * SoanLight.intensity * advanced_shading(roughness, diffuse.rgb, ndoth, ndotv, ndotl, vdoth) * ndotl);

		//color0.rgb = SoanLight.ambient + blinn_phong(ndotl, ndoth, SoanLight.specular, diffuse.rgb, roughness);
		//	color0.rgb = vec3(visibility);
	}
	
	//
	// Do gamma correction.
	//
	//color0.rgb = pow( color0.rgb, vec3(1.0/2.2) ) * 1.0f;
	color0.rgb *= 2.0f;


}
