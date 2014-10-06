
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 vertexColor;
layout(location = 9) in vec3 texCoord;


uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 normMatrix;


uniform vec4 light_pos = vec4(10.0f, 0.0f, -2.0f, 0.0f);



out FragmentData {
	vec3 fragmentColor;
	vec3 fragmentNormal;
	vec4 eye;
	vec3 lightDir;
} outFragment;

void main(){

	mat4 viewModel = viewMatrix*modelMatrix;

	vec4 pos = viewModel * vec4(position,1.0);

	// Create the normal matrix wich is N = transpose(inverse(ViewModel));
	mat4 normalMatrix = transpose(inverse(viewModel));
	
  // Now we use that to transform the vertex normal into viewModel space.
	vec4 vRes = normalMatrix*vec4(normal, 0.0);
  outFragment.fragmentNormal = vRes.xyz; 	

	outFragment.lightDir = vec3(light_pos - pos);
	outFragment.eye = -(pos);

	// Set the position of the vertex for the pipeline.
	gl_Position = projMatrix*viewModel*vec4(position,1.0);

}