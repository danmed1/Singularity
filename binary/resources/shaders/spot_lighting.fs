out vec4 color;

uniform vec4 diffuse = vec4(0.2f, 0.2f, 1.0f, 0.0f);
uniform vec4 ambient = vec4(0.1f, 0.1f, 0.2f, 0.0f);
uniform vec4 specular = vec4(1.0f, 1.0f, 1.0f, 0.0f);
uniform float shininess = 128.5f;

layout (std140) uniform Lights {
    vec4 light_position, light_direction;
    float light_cutoff;
};

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
        spec = specular * pow(intSpec, shininess);
    }
 
    color = max(intensity * diffuse + spec, ambient);
}