uniform sampler2D texture0;

uniform float 		blurSize = 1.0/400.0;

uniform int 			blurDirection = 0;

in vec2 tcoord;

layout(location = 0) out vec4 odiffuse;
layout(location = 1) out vec4 oposition;
layout(location = 2) out vec4 onormal;
layout(location = 3) out vec4 oblend;

void main() {
	
	vec4 sum = vec4(0.0f);
	for(int x = -2; x <= 2; x++) {
		for(int y = -2; y <= 2; y++) {
			sum += texture(texture0, vec2(tcoord.x + x * blurSize, tcoord.y + y*blurSize))/16.0f;
		}
	}
	
	odiffuse = sum;


}
