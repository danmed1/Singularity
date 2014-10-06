#include <tm/tm.h>
#include <tm/tmg.h>
#include "ShadowMap.h"


namespace soan {


	ShadowMap::ShadowMap(xdl::XdevLOpenGL330* opengl) : 	PostProcessEffect(opengl), m_gausBlur(opengl),
		m_biasMatrix(	0.5f, 0.0f, 0.0f, 0.5f,
		              0.0f, 0.5f, 0.0f, 0.5f,
		              0.0f, 0.0f, 0.5f, 0.5f,
		              0.0f, 0.0f, 0.0f, 1.0f),
		m_minVariance(0.00002f),
		m_reduceLightBleedingAmount(0.2f) {

		// Orthographic projection is used for directional light.
		tmath::ortho(-20.0f, 20.0f, -20.0f, 20.0f,-80.0f, 40.0f, m_projectionMatrix);

		// Perspective is used for point lights.
		//tmath::frustum(-2.85f, 2.85f, -2.15f, 2.15f, 1.0f, 100.0f, m_projectionMatrix);

		// Let's pre-calculate the matrix.
		m_biasProjectionMatrix = m_biasMatrix*m_projectionMatrix;
	}

	ShadowMap::~ShadowMap() {
		m_opengl->destroy(m_vs);
		m_opengl->destroy(m_fs);
		m_opengl->destroy(m_frameBuffer);
		m_opengl->destroy(m_shaderProgram);
	}

	void ShadowMap::setLight(Light* light) {
		m_light = light;
	}

	void ShadowMap::setCamera(Camera* camera) {
		m_camera = camera;
	}

	void ShadowMap::setMinVariance(xdl::xdl_float minVariance) {
		m_minVariance = minVariance;
	}
	
	void ShadowMap::setReduceLightBleedingAmount(xdl::xdl_float reduceLightBleedingAmount) {
		m_reduceLightBleedingAmount = reduceLightBleedingAmount;
	}

	xdl::xdl_float ShadowMap::getMinVariance() {
		return m_minVariance;
	}

	xdl::xdl_float ShadowMap::getReduceLightBleedingAmount() {
		return m_reduceLightBleedingAmount;
	}

	int ShadowMap::init(unsigned int width, unsigned height) {

		// TODO The texture format can be xdl::XDEVL_RG16F

		PostProcessEffect::init(width, height);
		m_gausBlur.init(width, height, xdl::XDEVL_RG32F);

		m_opengl->createShaderProgram(&m_shaderProgram);

		if(m_opengl->createVertexShader(&m_vs) == -1) {
			std::cerr << "GBuffer::Could not compile vertex shader." << std::endl;
			return -1;
		}
		m_vs->compileFromFile("resources/shaders/shadowMap_vs.glsl");

		if(m_opengl->createFragmentShader(&m_fs) == -1) {
			std::cerr << "GBuffer::Could not compile fragment shader." << std::endl;
			return -1;
		}
		m_fs->compileFromFile("resources/shaders/shadowMap_fs.glsl");

		m_shaderProgram->attach(m_vs);
		m_shaderProgram->attach(m_fs);
		m_shaderProgram->link();

		m_projMatrix 		= 	m_shaderProgram->getUniformLocation("projMatrix");
		m_viewMatrix 		= 	m_shaderProgram->getUniformLocation("viewMatrix");
		m_modelMatrix 	= 	m_shaderProgram->getUniformLocation("modelMatrix");

		// Create Framebuffer with 4 render targets and one depth buffer.
		m_opengl->createFrameBuffer(&m_frameBuffer);
		if(m_frameBuffer->init(m_width, m_height) != xdl::ERR_OK) {
			std::cerr << "GBuffer::Could not create Framebuffer." << std::endl;
			return -1;
		}
		m_frameBuffer->addColorTarget(0, xdl::XDEVL_RG32F);
		m_frameBuffer->getTexture(0)->lock();
		m_frameBuffer->getTexture(0)->setTextureWrap(xdl::XDEVL_TEXTURE_WRAP_S, xdl::XDEVL_CLAMP_TO_BORDER);
		m_frameBuffer->getTexture(0)->setTextureWrap(xdl::XDEVL_TEXTURE_WRAP_T, xdl::XDEVL_CLAMP_TO_BORDER);
		m_frameBuffer->getTexture(0)->setTextureFilter(xdl::XDEVL_TEXTURE_MAG_FILTER, xdl::XDEVL_LINEAR);
		m_frameBuffer->getTexture(0)->setTextureFilter(xdl::XDEVL_TEXTURE_MIN_FILTER, xdl::XDEVL_LINEAR_MIPMAP_LINEAR);
		m_frameBuffer->getTexture(0)->setTextureMaxAnisotropy(16.0);
		m_frameBuffer->getTexture(0)->generateMipMap();
		m_frameBuffer->getTexture(0)->unlock();


		m_frameBuffer->addDepthTarget(xdl::XDEVL_DEPTH_COMPONENT24, xdl::XDEVL_FLOAT);



		return 0;
	}
	void ShadowMap::apply() {
	}

	void ShadowMap::activate() {
		assert(m_light && "ShadowMap::calculate: Input light not specified");
		assert(m_camera && "ShadowMap::calculate: Input Camera not specified");

		m_frameBuffer->activate();
		//	glEnable(GL_MULTISAMPLE);
		glViewport(0, 0, m_frameBuffer->getWidth(), m_frameBuffer->getHeight());

		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_FRONT);

		//
		// Enable and reset Depth Buffer.
		//
		xdl::xdl_uint targets [] = {xdl::XDEVL_COLOR_TARGET0};
		m_frameBuffer->activateColorTargets(1, targets);
		m_frameBuffer->clearColorTargets(1.0f, 1.0f, 0.0f, 0.0f);
		m_frameBuffer->activateDepthTarget(xdl::xdl_true);
		m_frameBuffer->clearDepthTarget(1.0);

		m_depthBiasVP = m_biasProjectionMatrix*m_light->getInverseTransformationMatrix();


		m_shaderProgram->activate();
		m_shaderProgram->setUniformMatrix4(m_projMatrix, 1, m_projectionMatrix);
		m_shaderProgram->setUniformMatrix4(m_viewMatrix, 1, m_light->getInverseTransformationMatrix());

	}

	void ShadowMap::deactivate() {
//		glCullFace(GL_BACK);
//glDisable(GL_MULTISAMPLE);


		m_shaderProgram->deactivate();
		m_frameBuffer->deactivate();

		m_gausBlur.setInputTexture(0, m_frameBuffer->getTexture(0));
		m_gausBlur.setBlurSize(1.0, 1.0);
		m_gausBlur.apply();

		m_frameBuffer->getTexture(0)->lock();
		m_frameBuffer->getTexture(0)->generateMipMap();
		m_frameBuffer->getTexture(0)->unlock();


		// This is important. Set the output texture.
		//setOutputTexture(0,	m_frameBuffer->getTexture(0));
		setOutputTexture(0,	m_gausBlur.getOutputTexture(0));

	}


	xdl::XdevLShaderProgram* ShadowMap::getShaderProgram() {
		return m_shaderProgram;
	}

	tmath::mat4&	ShadowMap::getDepthBias() {
		return m_depthBiasVP;
	}

	void ShadowMap::setProjectionMatrix(const tmath::mat4& matrix) {
		m_projectionMatrix = matrix;
		m_shaderProgram->setUniformMatrix4(m_projMatrix, 1, m_projectionMatrix);
	}

	void  ShadowMap::setViewMatrix(const tmath::mat4& matrix) {
		m_shaderProgram->setUniformMatrix4(m_viewMatrix, 1, matrix);
	}

	void ShadowMap::setModelMatrix(const tmath::mat4& matrix) {
		m_shaderProgram->setUniformMatrix4(m_modelMatrix, 1, matrix);
	}

}
