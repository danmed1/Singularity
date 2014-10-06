precision mediump float;


#define DIFFUSE 1
#define NORMAL 2
#define SPECULAR 4
#define ROUGHNESS

struct Material {
	vec4 				diffuse;
	vec4 				ambient;
	vec4 				specular;
	float				roughness;
	sampler2D 	texture0;
	sampler2D 	texture1; 
	sampler2D 	texture2; 
	sampler2D 	texture3;
	samplerCube skyBoxTexture;	
	int					flags;
};

uniform Material SoanMaterial;
uniform float renderSkyBox = 0.0;
uniform float	debug = 0.0;

in FragmentData {
	vec4 position;
	vec4 color;
	vec4 normal;
	vec4 binormal;
	vec4 tangent;	
	vec2 texCoord;
	vec3 skyBoxTexCoord;	
	mat3 tbn;	
} fragment;

layout(location = 0) out vec4 position;
layout(location = 1) out vec4 normal;
layout(location = 2) out vec4 diffuse;

void main() {
	//
	// Write the interpolated diffuse color into the color target 0.
	//
	position 	= fragment.position; 
	
	//
	// Write the interpolated position in world coordinate system into color target 1.
	//
#ifdef NORMAL_MAP
//	normal 		= vec4(normalize(texture(SoanMaterial.texture1, fragment.texCoord.st)).xyz, 0.0);
	normal.xyz	= fragment.tbn * texture(SoanMaterial.texture1, fragment.texCoord).xyz;
#else
	normal 		= normalize(fragment.normal);
#endif

	//
	// What is this? It is a hack for now to transfer the roughness of the material to the lighting stage.
	//
	normal.w	= SoanMaterial.roughness;

		
	//
	// What is this diffuse.a = 0? Used for debug purpose. Causes in the lighting stage not to do lighting for the fragment.
	//
	if(renderSkyBox >= 1.0) {
			diffuse 	= 1.0*texture(SoanMaterial.skyBoxTexture, fragment.skyBoxTexCoord);
			diffuse.a	= 0.0;
	} else {
		diffuse 	= 2.0*texture(SoanMaterial.texture0, fragment.texCoord)*vec4(SoanMaterial.diffuse.xyz, 1.0f);
	}
	if(debug >= 1.0) {
		diffuse = fragment.color;
		diffuse.a = 0.0;
	}

	
}
