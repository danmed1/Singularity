out vec4 color;

struct Material {
    vec4 diffuse;
		vec4 ambient;
		vec4 specular;
		float shininess;
};

uniform Material SoanMaterial;

in FragmentData {
	vec3 fragmentColor;
	vec3 fragmentNormal;
	vec4 eye;		
	vec3 lightDir;
} inFragment;

void main() {

	vec4 spec = vec4(0.0);
	vec3 n = normalize(inFragment.fragmentNormal);
	vec3 e = normalize(vec3(inFragment.eye));
	vec3 l = normalize(inFragment.lightDir);
	
	float intensity = max(dot(n,l), 0.0);
    if (intensity > 0.0) {
        vec3 h = normalize(l + e);
        float intSpec = max(dot(h,n), 0.0);
        spec = SoanMaterial.specular * pow(intSpec, SoanMaterial.shininess);
    }
 
    color = max(intensity * SoanMaterial.diffuse + spec, SoanMaterial.ambient);
}