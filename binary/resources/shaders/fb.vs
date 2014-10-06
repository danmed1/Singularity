layout(location = 0) in vec2 position;
layout(location = 9) in vec2 texCoord;

uniform mat4 projMatrix;

out vec2 tcoord;

void main(){
	tcoord = texCoord;

	gl_Position = projMatrix*vec4(position.x, position.y, 0.0, 1.0);

}
