layout(location = 0) in vec2 InPosition;

out vec2 textureCoordinate;

void main() {

	textureCoordinate = InPosition * 0.5 + 0.5;

	gl_Position = vec4(InPosition, 0.0, 1.0);

}
