in vec4 color;
in vec2 tcoord;

uniform sampler2D texture0;

uniform float buffer 	= 0.8f;
uniform float gamma 	= 0.4f;
uniform int 	dft			= 1;
uniform int 	effect 	= 2;
uniform vec4 	glowColor = vec4(0.1, 0.1, 0.1, 1.0);
uniform vec2	shadowOffset = vec2(-0.004, 0.004);
out vec4 ocolor;

void main(void) {

	vec4 basecolor = color;

	//
	// Use normal texture fonts.
	//
	if(dft == 0) {
		basecolor = vec4(color.r, color.g, color.b, texture(texture0, tcoord).a);
		if(effect == 1) {
			basecolor += glowColor*texture(texture0, tcoord + shadowOffset);
		}
	} else
		//
		// Use Signed Distance Field textures fonts.
		//
		if(dft == 1) {

			// Get the distance form the texture.
			float dist = texture(texture0, tcoord).a;
			basecolor = color*smoothstep(buffer - gamma, buffer + gamma, dist);

			if(effect == 1) {
				float glowTexel 	=  texture(texture0, tcoord + shadowOffset).a;
				vec4 glowc				=  glowColor*smoothstep(0.3, 0.5, glowTexel);

				basecolor = glowc + basecolor;

			} else if(effect == 2) {
				float glowTexel 	=  texture(texture0, tcoord).a;
				vec4 glowc				=  glowColor*smoothstep(0.1, 0.2, glowTexel);

				basecolor = glowc + basecolor;

			} else {
				basecolor *= color*smoothstep(buffer - gamma, buffer + gamma, dist);
			}
		}

	ocolor = basecolor;

}
