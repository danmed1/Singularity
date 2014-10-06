layout(location = 0) in vec3 iposition;
layout(location = 1) in vec3 inormal;
layout(location = 2) in vec3 itangent;
layout(location = 3) in vec3 ibinormal;
layout(location = 4) in vec4 icolor;
layout(location = 9) in vec2 itexCoord;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform vec3 cameraPos;

out FragmentData {
	vec4 position;
	vec4 color;
	vec3 normal;
	vec3 tangent;
	vec2 texCoord;
	vec3 skyBoxTexCoord;
	vec3 v;
} outFragment;

void main(){
	vec4 position 							= vec4(iposition, 1.0f);
	vec4 normal									= vec4(inormal, 0.0f);
	vec4 tangent								= vec4(itangent, 0.0f);
	vec4 binormal								= vec4(ibinormal, 0.0f);
	
	mat4 viewModel 							= viewMatrix*modelMatrix;
	mat4 normalMatrix 					= inverse(transpose(modelMatrix));
	
	outFragment.skyBoxTexCoord	=	normalize(vec3(iposition.x, -iposition.y, iposition.z));
	
	// Pass the postion in world coordinate system.
	outFragment.position				= modelMatrix  * position;
	
	outFragment.v 							= (cameraPos - outFragment.position.xyz);
		
	// Pass the normal in world coordinate system.
	outFragment.normal    			= (normalMatrix * normal).xyz;

	// Pass the tangent in world coordinate system.
	outFragment.tangent 				= (normalMatrix * tangent).xyz;
	
	// Pass the vertex color.
	outFragment.color						= icolor;
	
	// Pass the texture coordinate.
	outFragment.texCoord 				= itexCoord;

	// Just do the normal conversion into clip space.
	gl_Position 								= projMatrix * viewModel * position;
	
}
