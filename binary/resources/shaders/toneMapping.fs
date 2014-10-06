uniform sampler2D texture0;
uniform sampler2D texture1;

uniform float fExposureLevel = 32.0f;

in vec2 tcoord;

layout(location = 0) out vec4 color;

void main() {
	vec4 original = texture(texture0, tcoord); // Original texture
	vec4 blur 		= texture(texture1, tcoord); // Blur texture

	vec4 tmp 		= mix(original, blur, 0.4f);
	tcoord -= 0.5f; 
	
	// Square of distance from origin (center of screen)
	float vignette = 1 - dot(tcoord, tcoord);	
	
	tmp = tmp*vignette*vignette*vignette*vignette;
	tmp*= fExposureLevel;
	
	color = pow(tmp, 0.55f)
}
