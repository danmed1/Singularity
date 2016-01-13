/*
	Copyright (c) 2015 Cengiz Terzibas

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.
	
	cengiz@terzibas.de
*/

#include "GBuffer.h"
#include "Camera.h"
#include "Types.h"

class DIFFUSE;
class setUniform;
class setUniform;
namespace soan {

	GBuffer::GBuffer(xdl::IPXdevLRAI opengl) :
		m_opengl(opengl),
		m_va(nullptr),
		m_fb(nullptr),
		m_camera(nullptr),
		m_skyBoxTexture(nullptr),
		m_reflectionTextureCube(nullptr),
		m_stage1vs(nullptr),
		m_stage1fs(nullptr),
		m_stage1sp(nullptr),
		m_stage2vs(nullptr),
		m_stage2fs(nullptr),
		m_stage2sp(nullptr),
		m_debugShaderProgram(nullptr),
		m_debugFragmentShader(nullptr),
		m_begin(false),
		m_beginStage1(false),
		m_blendStage(false),
		m_viewPortWidth(320),
		m_viewPortHeight(200),
		m_debugMode(0) {


	}

	GBuffer::~GBuffer() {
		m_lights.clear();
	}
	
	void GBuffer::setCamera(Camera* camera) {
		assert(camera && "GBuffer::setCamera: Invalid pointer.");
		m_camera = camera;
	}

	void GBuffer::setDebugMode(xdl::xdl_int mode) {
		m_debugMode = mode;
		m_stage1sp->setUniformi(debug, m_debugMode);
	}

	xdl::IPXdevLFrameBuffer GBuffer::getGBuffer() {
		return m_fb;
	}

	xdl::IPXdevLShaderProgram GBuffer::getFillGBufferShaderProgram() {
		return m_stage1sp;
	}
	xdl::IPXdevLShaderProgram GBuffer::getLightingShaderProgram() {
		return m_stage2sp;
	}

	xdl::xdl_uint GBuffer::getViewPortWidth() {
		return m_viewPortWidth;
	}

	xdl::xdl_uint GBuffer::getViewPortHeight() {
		return m_viewPortHeight;
	}

	void GBuffer::addLight(Light* light) {
		m_lights.push_back(light);
	}

	Light* GBuffer::getLight(xdl::xdl_uint idx) {
		return m_lights[idx];
	}

	xdl::xdl_int GBuffer::init(xdl::xdl_uint width, xdl::xdl_uint height) {
		m_viewPortHeight 	= height;
		m_viewPortWidth 	= width;

		xdl::xdl_float screen_vertex [] = {
			-1.0, -1.0f,
			-1.0f, 1.0f,
			1.0f, 1.0f,

			1.0f, 1.0f,
			1.0f, -1.0f,
			-1.0f, -1.0f
		};

		auto vd = m_opengl->createVertexDeclaration();
		vd->add(2, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, VERTEX_POSITION);


		std::vector<xdl::xdl_uint8*> list;
		list.push_back((xdl::xdl_uint8*)screen_vertex);


		m_va = m_opengl->createVertexArray();
		m_va->init(list.size(), list.data(), 6, vd);


		m_fb = m_opengl->createFrameBuffer();

		// Create Framebuffer with 4 render targets and one depth buffer.
		if(m_fb->init(width, height) != xdl::ERR_OK) {
			std::cerr << "GBuffer::Could not create Framebuffer." << std::endl;
			return -1;
		}

		//
		// Add Position target. Position data will be in world coordinate system.
		//
		m_fb->addColorTarget(POSITION	, xdl::XDEVL_FB_COLOR_RGBA32F);
		m_fb->getTexture(POSITION)->lock();
		m_fb->getTexture(POSITION)->setTextureFilter(xdl::XDEVL_TEXTURE_MAG_FILTER, xdl::XDEVL_NEAREST);
		m_fb->getTexture(POSITION)->setTextureFilter(xdl::XDEVL_TEXTURE_MIN_FILTER, xdl::XDEVL_NEAREST);
//		m_fb->getTexture(POSITION)->setTextureWrap(xdl::XDEVL_TEXTURE_WRAP_S, xdl::XDEVL_CLAMP_TO_EDGE);
//		m_fb->getTexture(POSITION)->setTextureWrap(xdl::XDEVL_TEXTURE_WRAP_T, xdl::XDEVL_CLAMP_TO_EDGE);
		m_fb->getTexture(POSITION)->unlock();

		//
		// Create normal target. Normal data will be in world coordinate system.
		//
		m_fb->addColorTarget(NORMAL		, xdl::XDEVL_FB_COLOR_RGBA16F);
		m_fb->getTexture(NORMAL)->lock();
		m_fb->getTexture(NORMAL)->setTextureFilter(xdl::XDEVL_TEXTURE_MAG_FILTER, xdl::XDEVL_NEAREST);
		m_fb->getTexture(NORMAL)->setTextureFilter(xdl::XDEVL_TEXTURE_MIN_FILTER, xdl::XDEVL_NEAREST);
//		m_fb->getTexture(NORMAL)->setTextureWrap(xdl::XDEVL_TEXTURE_WRAP_S, xdl::XDEVL_CLAMP_TO_EDGE);
//		m_fb->getTexture(NORMAL)->setTextureWrap(xdl::XDEVL_TEXTURE_WRAP_T, xdl::XDEVL_CLAMP_TO_EDGE);
		m_fb->getTexture(NORMAL)->unlock();

		//
		// Create albedo target.
		//
		m_fb->addColorTarget(DIFFUSE	, xdl::XDEVL_FB_COLOR_RGBA);
		m_fb->getTexture(DIFFUSE)->lock();
		m_fb->getTexture(DIFFUSE)->setTextureFilter(xdl::XDEVL_TEXTURE_MAG_FILTER, xdl::XDEVL_LINEAR);
		m_fb->getTexture(DIFFUSE)->setTextureFilter(xdl::XDEVL_TEXTURE_MIN_FILTER, xdl::XDEVL_LINEAR);
		m_fb->getTexture(DIFFUSE)->setTextureWrap(xdl::XDEVL_TEXTURE_WRAP_S, xdl::XDEVL_CLAMP_TO_EDGE);
		m_fb->getTexture(DIFFUSE)->setTextureWrap(xdl::XDEVL_TEXTURE_WRAP_T, xdl::XDEVL_CLAMP_TO_EDGE);
		m_fb->getTexture(DIFFUSE)->unlock();


		m_fb->addColorTarget(LIGHTING	, xdl::XDEVL_FB_COLOR_RGBA);
		m_fb->getTexture(LIGHTING)->lock();
		m_fb->getTexture(LIGHTING)->setTextureFilter(xdl::XDEVL_TEXTURE_MAG_FILTER, xdl::XDEVL_LINEAR);
		m_fb->getTexture(LIGHTING)->setTextureFilter(xdl::XDEVL_TEXTURE_MIN_FILTER, xdl::XDEVL_LINEAR);
		m_fb->getTexture(LIGHTING)->setTextureWrap(xdl::XDEVL_TEXTURE_WRAP_S, xdl::XDEVL_CLAMP_TO_EDGE);
		m_fb->getTexture(LIGHTING)->setTextureWrap(xdl::XDEVL_TEXTURE_WRAP_T, xdl::XDEVL_CLAMP_TO_EDGE);
		m_fb->getTexture(LIGHTING)->unlock();


		m_fb->addDepthTarget(xdl::XDEVL_FB_DEPTH_COMPONENT24);

		m_stage1sp = m_opengl->createShaderProgram();
		m_stage1vs = m_opengl->createVertexShader();
		if(m_stage1vs->compileFromFile("resources/shaders/deferred/fillGBuffer_vs.glsl") != xdl::ERR_OK) {
			std::cerr << "GBuffer::Could not compile vertex shader." << std::endl;
			return -1;
		}

		m_stage1fs = m_opengl->createFragmentShader();
		if(m_stage1fs->compileFromFile("resources/shaders/deferred/fillGBuffer_fs.glsl") != xdl::ERR_OK) {
			std::cerr << "GBuffer::Could not compile vertex shader." << std::endl;
			return -1;
		}

		m_stage1sp->attach(m_stage1vs);
		m_stage1sp->attach(m_stage1fs);
		m_stage1sp->link();


		m_stage2sp = m_opengl->createShaderProgram();
		m_stage2vs = m_opengl->createVertexShader();
		m_stage2vs->compileFromFile("resources/shaders/deferred/doDeferredLighting_vs.glsl");


		m_stage2fs = m_opengl->createFragmentShader();

		m_stage2fs->addShaderCodeFromFile("resources/shaders/brdf.glsl");
		m_stage2fs->addShaderCodeFromFile("resources/shaders/shadow.glsl");
		m_stage2fs->addShaderCodeFromFile("resources/shaders/deferred/doDeferredLighting_fs.glsl");
		m_stage2fs->compile();



		m_stage2sp->attach(m_stage2vs);
		m_stage2sp->attach(m_stage2fs);
		if(m_stage2sp->link() == xdl::ERR_ERROR) {
			std::cerr << "GBuffer::Could not link shader code." << std::endl;
			return -1;
		}

		//
		// Create the shader program to debug.
		//
		m_debugShaderProgram = m_opengl->createShaderProgram();
		auto dvs = m_opengl->createVertexShader();
		dvs->compileFromFile("resources/shaders/debug_vs.glsl");


		m_debugFragmentShader = m_opengl->createFragmentShader();
		m_debugFragmentShader->compileFromFile("resources/shaders/debug_fs.glsl");



		m_debugShaderProgram->attach(dvs);
		m_debugShaderProgram->attach(m_debugFragmentShader);
		m_debugShaderProgram->link();



		projMatrixStage1 	= m_stage1sp->getUniformLocation("projMatrix");
		viewMatrixStage1 	= m_stage1sp->getUniformLocation("viewMatrix");
		modelMatrixStage1 = m_stage1sp->getUniformLocation("modelMatrix");


		viewMatrixStage2 	= m_stage2sp->getUniformLocation("viewMatrix");


		colorTexture 			= m_stage2sp->getUniformLocation("diffuseTex");
		posTexture 				= m_stage2sp->getUniformLocation("posTex");
		normTexture 			= m_stage2sp->getUniformLocation("normalTex");
		blendTexture 			= m_stage2sp->getUniformLocation("blendTex");


		cameraPosShader1	= m_stage1sp->getUniformLocation("cameraPos");
		cameraPos					= m_stage2sp->getUniformLocation("cameraPos");


		lightPos					= m_stage2sp->getUniformLocation("SoanLight.position");
		lightIntensity		= m_stage2sp->getUniformLocation("SoanLight.intensity");
		lightDirection		= m_stage2sp->getUniformLocation("SoanLight.direction");
		lightSpecular			= m_stage2sp->getUniformLocation("SoanLight.specular");
		lightAmbient			= m_stage2sp->getUniformLocation("SoanLight.ambient");



		depthBiasMatrix 										= m_stage2sp->getUniformLocation("depthBiasMatrix");
		shadowMapTexture										= m_stage2sp->getUniformLocation("SoanShadowMap.shadowMap");
		shadowMapSize												= m_stage2sp->getUniformLocation("SoanShadowMap.shadowMapSize");
		shadowMapMinVariance								= m_stage2sp->getUniformLocation("SoanShadowMap.minVariance");;
		shadowMapReduceLightBleedingAmount	= m_stage2sp->getUniformLocation("SoanShadowMap.reduceLightBleedingAmount");;

		reflectionTextureCube = m_stage2sp->getUniformLocation("reflectionTextureCube");

		debug			 						= m_stage1sp->getUniformLocation("debug");


		m_sh_diffuse					= m_stage1sp->getUniformLocation("SoanMaterial.diffuse");
		m_sh_ambient					= m_stage1sp->getUniformLocation("SoanMaterial.ambient");
		m_sh_specular					= m_stage1sp->getUniformLocation("SoanMaterial.specular");
		m_sh_roughness				= m_stage1sp->getUniformLocation("SoanMaterial.roughness");
		m_sh_diffuseMap				= m_stage1sp->getUniformLocation("SoanMaterial.diffuseMap");
		m_sh_normalMap				= m_stage1sp->getUniformLocation("SoanMaterial.normalMap");
		m_sh_displacementMap	= m_stage1sp->getUniformLocation("SoanMaterial.displacementMap");
		m_sh_specularMap			= m_stage1sp->getUniformLocation("SoanMaterial.specularMap");
		m_sh_roughnessMap			= m_stage1sp->getUniformLocation("SoanMaterial.roughnessMap");
		m_sh_skyBoxTexture		= m_stage1sp->getUniformLocation("SoanMaterial.skyBoxTexture");
		m_sh_flags						= m_stage1sp->getUniformLocation("SoanMaterial.flags");



		renderSkyBox 			= m_stage1sp->getUniformLocation("renderSkyBox");

		return 0;
	}


	xdl::xdl_int GBuffer::startFillGBuffer() {
		assert(!m_begin && " GBuffer::startFillGBuffer() already used");
		m_begin = true;

		// Let's clean up everything first. For that we have to write into all
		// render targets.
		m_fb->activate();

		// Let's clear the diffuse target.
		xdl::xdl_uint list_stage_init[] = {xdl::XDEVL_COLOR_TARGET0, xdl::XDEVL_COLOR_TARGET1, xdl::XDEVL_COLOR_TARGET2};
		m_fb->activateColorTargets(3, list_stage_init);

		m_fb->clearColorTargets(0.3f, 0.3f, 0.8f, 1.0f);

		m_fb->activateDepthTarget(xdl::xdl_true);
		m_fb->clearDepthTarget(1.0f);

		// Stage 1.
		// Render color, position, normal and depth information.
		// Blending is disabled.
		m_stage1sp->activate();


		m_beginStage1 = true;

		getFillGBufferShaderProgram()->setUniformMatrix4(projMatrixStage1, 1, getProjectionMatrix());
		getFillGBufferShaderProgram()->setUniformMatrix4(viewMatrixStage1, 1, getViewMatrix());

		getFillGBufferShaderProgram()->setUniform3v(cameraPosShader1, 1, m_camera->getForwardVector());


		return 0;
	}


	xdl::xdl_int GBuffer::stopFillGBuffer() {
		m_stage1sp->deactivate();

		return 0;
	}

	xdl::xdl_int GBuffer::startLightingStage() {


		// Disble the depth buffer.
		m_fb->activateDepthTarget(xdl::xdl_false);

		// Activate only render target 1. We have anyway just one render target for the post process
		// framebuffer.
		xdl::xdl_uint list_stage_init[] = {xdl::XDEVL_COLOR_TARGET3};
		m_fb->activateColorTargets(1, list_stage_init);


		//
		// Set shader parameters.
		//
		if(m_debugMode == 1) {
			m_debugShaderProgram->activate();
		} else {
			m_stage2sp->activate();
		}



		// Set texture values.
		m_stage2sp->setUniformi(posTexture, 	POSITION);
		m_stage2sp->setUniformi(normTexture, 	NORMAL);
		m_stage2sp->setUniformi(colorTexture,	DIFFUSE);
		m_stage2sp->setUniformi(reflectionTextureCube,4);

		// Activate texture units.
		getGBuffer()->getTexture(0)->activate(POSITION);
		getGBuffer()->getTexture(1)->activate(NORMAL);
		getGBuffer()->getTexture(2)->activate(DIFFUSE);

		if(m_reflectionTextureCube != nullptr) {
			m_reflectionTextureCube->activate(4);
		}

		m_stage2sp->setUniform3v(lightAmbient, 	1, 	m_lights[0]->getAmbient());
		m_stage2sp->setUniform(lightIntensity,  		m_lights[0]->getIntensity());
		m_stage2sp->setUniform3v(lightSpecular, 1, 	m_lights[0]->getSpecular());
		m_stage2sp->setUniform3v(lightPos, 		1,	m_lights[0]->getPosition());
		m_stage2sp->setUniform3v(lightDirection,1,	m_lights[0]->getForwardDirection());

		m_stage2sp->setUniform3v(cameraPos,1, m_camera->getPosition());


		//
		// Shadow map stuff.
		//

		m_stage2sp->setUniformMatrix4(depthBiasMatrix, 1, m_depthBiasVP);
		m_stage2sp->setUniformi(shadowMapTexture, 	4);

		m_stage2sp->setUniform(shadowMapMinVariance, m_shadowMap->getMinVariance());
		m_stage2sp->setUniform(shadowMapReduceLightBleedingAmount, m_shadowMap->getReduceLightBleedingAmount());
		m_stage2sp->setUniform(shadowMapSize, m_shadowMap->getOutputTexture(0)->getWidth(), m_shadowMap->getOutputTexture(0)->getHeight());
		m_shadowMap->getOutputTexture(0)->activate(4);


		// Set matrix values.
		m_stage2sp->setUniformMatrix4(viewMatrixStage2, 1, m_camera->getInverseTransformationMatrix());

		m_opengl->setActiveVertexArray(m_va);
		m_opengl->setActiveShaderProgram(getLightingShaderProgram());


		//	m_fb->activateColorTargetCubePosition(0, xdl::XDEVL_TEXTURE_CUBE_MAP_POSITIVE_Z);
		m_opengl->drawVertexArray(xdl::XDEVL_PRIMITIVE_TRIANGLES, 6);


//		m_fb->activateColorTargetCubePosition(0, xdl::XDEVL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
//		m_opengl->drawVertexArray(xdl::XDEVL_PRIMITIVE_TRIANGLES, 6);
//
//
//		m_fb->activateColorTargetCubePosition(0, xdl::XDEVL_TEXTURE_CUBE_MAP_POSITIVE_X);
//		m_opengl->drawVertexArray(xdl::XDEVL_PRIMITIVE_TRIANGLES, 6);
//
//
//		m_fb->activateColorTargetCubePosition(0, xdl::XDEVL_TEXTURE_CUBE_MAP_NEGATIVE_X);
//		m_opengl->drawVertexArray(xdl::XDEVL_PRIMITIVE_TRIANGLES, 6);
//
//
//		m_fb->activateColorTargetCubePosition(0, xdl::XDEVL_TEXTURE_CUBE_MAP_POSITIVE_Y);
//		m_opengl->drawVertexArray(xdl::XDEVL_PRIMITIVE_TRIANGLES, 6);
//
//
//		m_fb->activateColorTargetCubePosition(0, xdl::XDEVL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
//		m_opengl->drawVertexArray(xdl::XDEVL_PRIMITIVE_TRIANGLES, 6);

		m_stage2sp->deactivate();
		m_fb->deactivate();


		m_begin = false;

		return 0;
	}

	xdl::IPXdevLTexture  GBuffer::getTexture(GBufferTexture texture) {
		switch(texture) {
			case DIFFUSE:
				return m_fb->getTexture();
			case POSITION:
				return m_fb->getTexture(1);
			case NORMAL:
				return m_fb->getTexture(2);
			case DEPTH:
				return m_fb->getDepthTexture();
			case LIGHTING:
				return m_fb->getTexture(3);
		}
	}

	void GBuffer::setDepthBias(tmath::mat4& depthBias) {
		m_depthBiasVP = depthBias;
	}


	void GBuffer::setShadowMap(ShadowMap* shadowMap) {
		m_shadowMap = shadowMap;
	}

	void  GBuffer::setProjectionMatrix(const tmath::mat4& projectionMatrix) {
		m_projectionMatrix = projectionMatrix;
	}

	tmath::mat4& GBuffer::getProjectionMatrix() {
		return m_projectionMatrix;
	}

	void  GBuffer::setViewMatrix(const tmath::mat4& viewMatrix) {
		m_viewMatrix = viewMatrix;
		getFillGBufferShaderProgram()->setUniformMatrix4(viewMatrixStage1, 1, m_viewMatrix);
	}

	tmath::mat4& GBuffer::getViewMatrix() {
		return m_viewMatrix;
	}

	void  GBuffer::setModelMatrix(const tmath::mat4& modelMatrix) {
		m_modelMatrix = modelMatrix;

		getFillGBufferShaderProgram()->setUniformMatrix4(modelMatrixStage1, 1, m_modelMatrix);
	}

	tmath::mat4& GBuffer::getModelMatrix() {
		return m_modelMatrix;
	}

	void GBuffer::setMaterial(const Material& material) {

		m_material = material;

		getFillGBufferShaderProgram()->setUniform4v(m_sh_diffuse, 	1, 	getMaterial().getDiffuse());
		getFillGBufferShaderProgram()->setUniform4v(m_sh_ambient, 	1, 	getMaterial().getAmbient());
		getFillGBufferShaderProgram()->setUniform4v(m_sh_specular,	1, 	getMaterial().getSpecular());
		getFillGBufferShaderProgram()->setUniform(m_sh_roughness, 			getMaterial().getRoughness());
		getFillGBufferShaderProgram()->setUniformi(m_sh_flags, 				getMaterial().getStates());


		if(getMaterial().getSkyBoxTexture() != nullptr) {
			getFillGBufferShaderProgram()->setUniformi(m_sh_skyBoxTexture, 4);
			getMaterial().getSkyBoxTexture()->activate(4);
		}

		if(getMaterial().getTexture(Material::DIFFUSE_MAP) != nullptr) {
			getFillGBufferShaderProgram()->setUniformi(m_sh_diffuseMap, 3);
			getMaterial().getTexture(Material::DIFFUSE_MAP)->activate(3);
		}


		if(getMaterial().getTexture(Material::DISPLACEMENT_MAP) != nullptr) {
			getFillGBufferShaderProgram()->setUniformi(m_sh_displacementMap, 2);
			getMaterial().getTexture(Material::DISPLACEMENT_MAP)->activate(2);
		}

		if(getMaterial().getTexture(Material::NORMAL_MAP) != nullptr) {
			getFillGBufferShaderProgram()->setUniformi(m_sh_normalMap, 1);
			getMaterial().getTexture(Material::NORMAL_MAP)->activate(1);
		}


		if(getMaterial().getTexture(Material::SPECULAR_MAP) != nullptr) {
			getFillGBufferShaderProgram()->setUniformi(m_sh_specularMap, 0);
			getMaterial().getTexture(Material::SPECULAR_MAP)->activate(0);
		}

//		if(getMaterial().getTexture(Material::ROUGHNESS) != nullptr) {
//			getFillGBufferShaderProgram()->setUniformi(m_sh_roughnessMap, 0);
//			getMaterial().getTexture(Material::ROUGHNESS)->activate(0);
//		}


	}

	Material& GBuffer::getMaterial() {
		return m_material;
	}

	xdl::xdl_uint GBuffer::getModelViewMatrixID() {
		return modelMatrixStage1;
	}

	void  GBuffer::setReflectionTextureCube(xdl::IPXdevLTextureCube textureCube, xdl::xdl_int state) {
		m_skyBoxTexture = textureCube;
		m_reflectionTextureCube = textureCube;
		getFillGBufferShaderProgram()->setUniformi(renderSkyBox,	state);
	}

}
