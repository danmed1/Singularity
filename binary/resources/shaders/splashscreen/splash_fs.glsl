
#if PLATFORM_USES_ES2
precision highp float;
#else
// #version 120 at the beginning is added in FSlateOpenGLShader::CompileShader()
#extension GL_EXT_gpu_shader4 : enable
#endif

varying vec2 textureCoordinate;

uniform sampler2D SplashTexture;

void main() 
{
	// I had to negate the y direction because the picture was up-side down.
	vec2 tc = vec2(textureCoordinate.s, -textureCoordinate.t);

	gl_FragColor = texture2D(SplashTexture, tc);

}