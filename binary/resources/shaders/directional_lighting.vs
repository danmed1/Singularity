layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 vertexColor;
layout(location = 7) in vec2 texCoord;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

out FragmentData {
	vec3 pos;
	vec3 fragmentColor;
	vec3 fragmentNormal;
	vec2 fragmentTexCoord;
	vec4 eye;
	vec3 light_dir;	
} outFragment;

uniform vec3 sunlight_direction = vec3(0.0f, -1.0f, 0.0f);

vec4 getTransformedPosition(mat4 pm, mat4 vm, vec3 p) {
	return pm*vm*vec4(p,1.0);
}

void main(){
	mat4 viewModel = viewMatrix*modelMatrix;
	
	// Create the normal matrix wich is N = transpose(inverse(ViewModel));
	mat4 normalMatrix = transpose(inverse(viewModel));
	
  // Now we use that to transform the vertex normal into viewModel space.
  outFragment.fragmentNormal = (normalMatrix*vec4(normal, 0.0)).xyz; 	

	outFragment.eye = -(viewModel * vec4(position,1.0));

	// Set the position of the vertex for the pipeline.
	gl_Position = getTransformedPosition(projMatrix,viewModel, position);

	outFragment.light_dir = sunlight_direction;
	
	outFragment.fragmentTexCoord 	= texCoord;
	
	outFragment.pos	= vec3( (modelMatrix * vec4(position, 1.0f)).xyz );
}
