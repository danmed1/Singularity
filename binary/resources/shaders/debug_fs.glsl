precision mediump float;

uniform sampler2D diffuseTex; 

in vec2 tcoord;

layout(location = 0) out vec4 color;

void main() {
	vec4 diffuse 		= texture(diffuseTex,		tcoord);
	color = diffuse;		
}
