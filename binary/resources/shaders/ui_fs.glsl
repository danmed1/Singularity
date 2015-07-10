precision mediump float;

uniform sampler2D diffuseTex; 

in vec4 color;
in vec2 tcoord;

layout(location = 0) out vec4 ocolor;

void main() {
	vec4 diffuse = vec4(color.r, color.g, color.b, color.a);
	ocolor = diffuse;
}