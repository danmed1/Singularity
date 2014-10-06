uniform sampler2D texture0;

uniform vec2 	blurSize;
uniform int 	blurDirection = 0;

in vec2 textureCoordinate;

layout(location = 0) out vec4 odiffuse;

#define HORIZONTAL_BLUR 0
#define VERTICAL_BLUR 	1

#define NUM_SAMPLES 4
uniform float offset[NUM_SAMPLES] = float[]( 0.0, 1.0, 2.0, 3.0 );
uniform float weight[NUM_SAMPLES] = float[]( 20.0/64.0, 15.0/64.0, 6.0/64.0f, 1.0/64.0f);

//#define NUM_SAMPLES 3
//uniform float offset[NUM_SAMPLES] = float[]( 0.0, 1.3846153846, 3.2307692308 );
//uniform float weight[NUM_SAMPLES] = float[]( 0.2270270270, 0.3162162162, 0.0702702703 );


vec4 getBlurHorizontal(sampler2D sampler, vec2 textureCoord) {
	vec4 sum = texture(sampler, vec2(textureCoord.x, textureCoord.y)) * weight[0];
	for(int i = 1; i < NUM_SAMPLES; i++) {
		sum += texture(sampler, vec2(textureCoord.x - offset[i]*blurSize.x, textureCoord.y)) * weight[i];
		sum += texture(sampler, vec2(textureCoord.x + offset[i]*blurSize.x, textureCoord.y)) * weight[i];
	}
	return sum;
}

vec4 getBlurVertical(sampler2D sampler, vec2 textureCoord) {
	vec4 sum = texture(sampler, vec2(textureCoord.x, textureCoord.y)) * weight[0];
	for(int i = 1; i < NUM_SAMPLES; i++) {
		sum += texture(sampler, vec2(textureCoord.x, textureCoord.y - offset[i]*blurSize.y)) * weight[i];
		sum += texture(sampler, vec2(textureCoord.x, textureCoord.y + offset[i]*blurSize.y)) * weight[i];
	}
	return sum;
}


void main() {
	
	if(blurDirection == HORIZONTAL_BLUR) {
		odiffuse  	= getBlurHorizontal(texture0,	textureCoordinate);
	} else {
		odiffuse		= getBlurVertical(texture0,	textureCoordinate);
	}

}
