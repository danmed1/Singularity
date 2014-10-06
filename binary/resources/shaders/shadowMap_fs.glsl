layout(location = 0) out vec2 ocolor;

void main() {
	
	float depth = gl_FragCoord.z;
	depth = depth;

	float moment2 = depth * depth;
	
	// This part helps to reduce shadow acne.
	float dx = dFdx(depth);
	float dy = dFdy(depth);
	moment2 += 0.25*(dx*dx + dy*dy) ;
	
	ocolor = vec2(depth, moment2);	
}
