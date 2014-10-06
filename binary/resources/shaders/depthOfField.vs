layout(location = 0) in vec2 position;

out vec2 tcoord;

void main(){
	tcoord = vec2(position.xy) * 0.5 + 0.5;

	gl_Position = vec4(position.xy, 0.0, 1.0);

}
