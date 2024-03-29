#define DIFFUSE_CONST 	1
#define NORMAL_CONST 		2
#define SPECULAR_CONST 	4
#define ROUGHNESS_CONST 8

#define DIFFUSE_MAP 			16
#define NORMAL_MAP 				32
#define DISPLACEMENT_MAP 	64
#define SPECULAR_MAP 			128
#define ROUGHNESS_MAP 		256

struct Material {
	vec4 				diffuse;
	vec4 				ambient;
	vec4 				specular;
	float 			shininess;
	float				roughness;
	sampler2D 	diffuseMap;
	sampler2D 	normalMap;
	sampler2D		displacementMap;
	sampler2D 	specularMap;
	sampler2D 	roughnessMap;
	samplerCube skyBoxTexture;
	int					flags;
};

uniform Material SoanMaterial;
uniform int renderSkyBox = 0;
uniform int	debug = 0;

uniform mat4 viewMatrix;
uniform vec3 cameraPos;

in FragmentData {
	vec4 position;
	vec4 color;
	vec3 normal;
	vec3 tangent;
	vec2 texCoord;
	vec3 skyBoxTexCoord;
	vec3 v;
} fragment;

layout(location = 0) out vec4 positionTarget;
layout(location = 1) out vec4 normalTarget;
layout(location = 2) out vec4 diffuseTarget;



vec3 createNormals(Material material, mat3 tbn, vec2 texCoord) {
	vec3 nn =  (2.0*texture(material.normalMap, texCoord).xyz - 1);
	return normalize( tbn * nn);
}

vec2 calcParallaxTextureCoords(Material material, mat3 tbnMatrix, vec3 v, vec2 texCoords, float scale, float bias) {
	float height = (texture(material.displacementMap, texCoords.xy).r * scale + bias);
	return (texCoords.xy + (v * tbnMatrix).xy * height);
}


vec2 calcSteepParallaxTextureCoords(Material material, mat3 tbnMatrix, vec3 v, vec2 texCoords, float scale, float bias) {
	vec2 textureCoordinate = texCoords;

	float n = 20;

	float step = 1.0 / n;

	vec2 dtex = scale * (-v * tbnMatrix).xy / n;

	float height = 1.0f;
	float nb = texture(material.displacementMap, texCoords.xy).r;
	while(nb < height) {
		height -= step;

		textureCoordinate += dtex;

		nb = texture(material.displacementMap, texCoords.xy).r;
	}
	return textureCoordinate;
}

void main() {

	vec3 normal 		= normalize(fragment.normal);
	vec3 tangent 		= normalize(fragment.tangent);
	vec3 binormal 	= normalize(cross(tangent, normal));
	mat3 tbn 				= mat3(tangent, binormal, normal);


	//
	// Set default texture coordinates.
	//
	vec2 textureCoordinate = fragment.texCoord;

	//
	// Modify the default texture coordinates for parallax mapping.
	//
	if((SoanMaterial.flags & DISPLACEMENT_MAP) == DISPLACEMENT_MAP) {

		vec3 v = normalize(vec3(fragment.v.x, -fragment.v.y, -fragment.v.z));

		float scale = 0.1f;
		float bias = -scale*0.5;
		//textureCoordinate = calcParallaxTextureCoords(SoanMaterial, tbn, v, fragment.texCoord, scale, bias);
		textureCoordinate = calcSteepParallaxTextureCoords(SoanMaterial, tbn, v, fragment.texCoord, scale, bias);
	}


	//
	// Modify the default normal using a normal map.
	//
	if((SoanMaterial.flags & NORMAL_MAP) == NORMAL_MAP) {
		normalTarget.xyz 		= createNormals(SoanMaterial, tbn, textureCoordinate);
	} else {

		//
		// Set the default normal of this fragment.
		//
		normalTarget.xyz = normal;

	}

	//
	// What is this? It is a hack for now to transfer the roughness of the material to the lighting stage.
	//
	normalTarget.w	= SoanMaterial.roughness;

	//
	// Write the interpolated diffuse color into the G-Buffer.
	//
	positionTarget 	= fragment.position;


	//
	// What is this diffuse.a = 0? Used for debug purpose. Causes in the lighting stage not to do lighting for the fragment.
	//
	if(renderSkyBox == 1) {
		diffuseTarget 	= 1.0*texture(SoanMaterial.skyBoxTexture, fragment.skyBoxTexCoord);
		diffuseTarget.a	= 0.0;
	} else {

		//
		// Add the diffuse color term and mix with the diffuse map if neccessary.
		//
		if((SoanMaterial.flags & DIFFUSE_MAP) == DIFFUSE_MAP) {

			//
			// Do we have to mix?
			//
			if((SoanMaterial.flags & DIFFUSE_CONST) == DIFFUSE_CONST) {
				// Yes
				diffuseTarget 	= mix(texture(SoanMaterial.diffuseMap, textureCoordinate), SoanMaterial.diffuse, 0.5);
			} else {
				// No
				diffuseTarget 	= 2.0f*texture(SoanMaterial.diffuseMap, textureCoordinate);
			}

		} else {
			// Just constant diffuse color.
			diffuseTarget 	= SoanMaterial.diffuse;
		}
		//diffuseTarget.a = 1.0f;
	}

	if(debug == 1) {
		diffuseTarget = fragment.color;
		diffuseTarget.a = 0.0;
	}

	//diffuseTarget.rgb = binormal.rgb;//texture(SoanMaterial.normalMap,fragment.texCoord);
//	diffuseTarget.rgb = vec3(1.0f, 1.0f, 1.0f);
}
