uniform sampler2D diffuseTex;

in vec2 tcoord;
out vec4 color;

void main() {
//	vec4 tmp = texture(diffuseTex, tcoord);
	vec4 diffuse 	= texture(diffuseTex, tcoord);
	color = diffuse;
}
