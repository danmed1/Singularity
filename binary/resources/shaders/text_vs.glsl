layout(location = 0) in vec2 iposition;
layout(location = 4) in vec4 icolor;
layout(location = 9) in vec2 itexCoord;

uniform mat4 projViewModelMatrix = mat4(1.0f, 0.0f, 0.0f, 0.0f,
																				0.0f, 1.0f, 0.0f, 0.0f,
																				0.0f, 0.0f, 1.0f, 0.0f,
																				0.0f, 0.0f, 0.0f, 1.0f);

out vec2 tcoord; 
out vec4 color;
 
void main(void) {
	
	vec4 pos = projViewModelMatrix * vec4(iposition.xy, 0, 1);
	
  tcoord = itexCoord;
	
	color =  icolor;
	
  gl_Position = pos;
	
}
