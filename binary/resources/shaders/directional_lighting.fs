struct Material {
	vec4 diffuse;
	vec4 ambient;
	vec4 specular;
	float shininess;
};

uniform Material SoanMaterial;
uniform sampler2D firstTexture;

in FragmentData {
	vec3 pos;
	vec3 fragmentColor;
	vec3 fragmentNormal;
	vec2 fragmentTexCoord;
	vec4 eye;
	vec3 light_dir;
} inFragment;


layout(location = 0) out vec4 diffuseOutput;
layout(location = 1) out vec4 posOutput;
layout(location = 2) out vec4 normOutput;
layout(location = 3) out vec4 blendOutput;


void main() {

	vec4 spec = vec4(0.0);
	vec3 n = normalize(inFragment.fragmentNormal);
	vec3 e = normalize(vec3(inFragment.eye));

	float intensity = max(dot(n,-inFragment.light_dir), 0.0);
	if(intensity > 0.0) {
		// compute the half vector
		vec3 h = normalize(-inFragment.light_dir + e);
		// compute the specular term into spec
		float intSpec = max(dot(h,n), 0.0);
		spec = SoanMaterial.specular * pow(intSpec, SoanMaterial.shininess);
	}

	posOutput.xyz = inFragment.pos; 

	diffuseOutput = max(intensity *  SoanMaterial.diffuse + spec, SoanMaterial.ambient);


	normOutput = vec4(inFragment.fragmentNormal, 0);

	vec4 clr = SoanMaterial.diffuse;
	float alpha = clr.a;
	if(alpha < 0.1)
		discard;   // Optimization that will not change the depth buffer
	blendOutput.rgb = clr.rgb * clr.a; // Pre multiplied alpha
	blendOutput.a = clr.a;
}
