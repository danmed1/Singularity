layout(location = 0) in vec2 iposition;
layout(location = 4) in vec4 icolor;


uniform mat4 projMatrix = mat4(1.0f, 0.0f, 0.0f, 0.0f,
                               0.0f, 1.0f, 0.0f, 0.0f,
                               0.0f, 0.0f, 1.0f, 0.0f,
                               0.0f, 0.0f, 0.0f, 1.0f);


out vec4 color;

void main(void) {

	vec4 pos = projMatrix * vec4(iposition.xy, 0, 1);

	color =  icolor;

	gl_Position = pos;


}
