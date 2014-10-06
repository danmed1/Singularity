precision mediump float;

layout(location = 0) in vec3 iposition;
layout(location = 4) in vec4 icolor;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;

out vec4 vertexColor;

void main(){
	
	// Pass the vertex color.
	vertexColor						= icolor;

	// Just do the normal conversion into clip space.
	gl_Position 						= projMatrix * vec4(iposition, 1.0);
	
}
