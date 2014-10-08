#include "GausBlur.h"
#include <tm/tm.h>
#include <tm/tmg.h>

namespace soan {


	GausBlur::GausBlur(xdl::XdevLOpenGL330* opengl) : PostProcessEffect(opengl) {

	}

	GausBlur::~GausBlur() {
		m_opengl->destroy(m_vs);
		m_opengl->destroy(m_fs);
		m_opengl->destroy(m_frameBuffer);
		m_opengl->destroy(m_frameBuffer2);
		m_opengl->destroy(m_shaderProgram);
	}
	void GausBlur::setBlurSize(xdl::xdl_float xSize, xdl::xdl_float ySize) {
		m_blurSize[0] = (1.0/m_width)*xSize;
		m_blurSize[1] = (1.0f/m_height)*ySize;
	}

	int GausBlur::init(xdl::xdl_uint width, xdl::xdl_uint height, xdl::XdevLFrameBufferColorFormat textureFormat) {

		PostProcessEffect::init(width, height);


		m_opengl->createShaderProgram(&m_shaderProgram);

		if(m_opengl->createVertexShader(&m_vs) == -1) {
			std::cerr << "GBuffer::Could not compile vertex shader." << std::endl;
			return -1;
		}
		m_vs->compileFromFile("resources/shaders/blurGaus_vs.glsl");

		if(m_opengl->createFragmentShader(&m_fs) == -1) {
			std::cerr << "GBuffer::Could not compile fragment shader." << std::endl;
			return -1;
		}
		m_fs->compileFromFile("resources/shaders/blurGaus_fs.glsl");

		m_shaderProgram->attach(m_vs);
		m_shaderProgram->attach(m_fs);
		m_shaderProgram->link();

		m_textureid 			=  	m_shaderProgram->getUniformLocation("texture0");
		m_blurDirectionid	=  	m_shaderProgram->getUniformLocation("blurDirection");
		m_blurSizeid 			= 	m_shaderProgram->getUniformLocation("blurSize");

		// Create Framebuffer with 4 render targets and one depth buffer.
		m_opengl->createFrameBuffer(&m_frameBuffer);
		if(m_frameBuffer->init(m_width, m_height) != xdl::ERR_OK) {
			std::cerr << "GBuffer::Could not create Framebuffer." << std::endl;
			return -1;
		}
		m_frameBuffer->addColorTarget(0, textureFormat);
		m_frameBuffer->getTexture(0)->lock();
		m_frameBuffer->getTexture(0)->setTextureFilter(xdl::XDEVL_TEXTURE_MAG_FILTER, xdl::XDEVL_LINEAR);
		m_frameBuffer->getTexture(0)->setTextureFilter(xdl::XDEVL_TEXTURE_MIN_FILTER, xdl::XDEVL_LINEAR);	
		m_frameBuffer->getTexture(0)->setTextureWrap(xdl::XDEVL_TEXTURE_WRAP_S, xdl::XDEVL_CLAMP_TO_BORDER);
		m_frameBuffer->getTexture(0)->setTextureWrap(xdl::XDEVL_TEXTURE_WRAP_T, xdl::XDEVL_CLAMP_TO_BORDER);
		m_frameBuffer->getTexture(0)->unlock();	
		
		// Create Framebuffer with 4 render targets and one depth buffer.
		m_opengl->createFrameBuffer(&m_frameBuffer2);
		if(m_frameBuffer2->init(m_width, m_height) != xdl::ERR_OK) {
			std::cerr << "GBuffer::Could not create Framebuffer." << std::endl;
			return -1;
		}
		m_frameBuffer2->addColorTarget(0, textureFormat);
		
		m_frameBuffer2->getTexture(0)->lock();
		m_frameBuffer2->getTexture(0)->setTextureFilter(xdl::XDEVL_TEXTURE_MAG_FILTER, xdl::XDEVL_LINEAR);
		m_frameBuffer2->getTexture(0)->setTextureFilter(xdl::XDEVL_TEXTURE_MIN_FILTER, xdl::XDEVL_LINEAR);
		m_frameBuffer2->getTexture(0)->setTextureWrap(xdl::XDEVL_TEXTURE_WRAP_S, xdl::XDEVL_CLAMP_TO_BORDER);
		m_frameBuffer2->getTexture(0)->setTextureWrap(xdl::XDEVL_TEXTURE_WRAP_T, xdl::XDEVL_CLAMP_TO_BORDER);
		m_frameBuffer2->getTexture(0)->unlock();		
		
		return 0;

	}
	void GausBlur::apply() {
		assert(m_inputTexure[0] && "GausBlur::calculate: Input texture not specified");

	//	glViewport(0, 0, getWidth(), getHeight());



		m_opengl->setActiveShaderProgram(m_shaderProgram);
		m_opengl->setActiveVertexArray(m_va);

		xdl::xdl_uint targets [] = {xdl::XDEVL_COLOR_TARGET0};
		//
		// First pass.
		//
		m_frameBuffer->activate();

		m_frameBuffer->activateColorTargets(1, targets);
		m_frameBuffer->clearColorTargets(0.0f, 0.0f, 0.0f, 0.0f);
		m_frameBuffer->activateDepthTarget(xdl::xdl_false);
		m_shaderProgram->activate();

		
		m_shaderProgram->setUniformi(m_blurDirectionid, 0);
		m_shaderProgram->setUniform2v(m_blurSizeid,1, m_blurSize);

		m_shaderProgram->setUniformi(m_textureid, 0);
		m_inputTexure[0]->activate(0);

		m_opengl->drawVertexArray(xdl::XDEVL_PRIMITIVE_TRIANGLES, 6);


		m_frameBuffer->deactivate();


		//
		// Second pass
		//
	
		m_frameBuffer2->activate();
		m_frameBuffer2->activateColorTargets(1, targets);
		m_frameBuffer2->clearColorTargets(1.0f, 0.0f, 0.0f, 0.0f);

		m_shaderProgram->activate();
		m_shaderProgram->setUniformi(m_blurDirectionid, 1);
		m_shaderProgram->setUniform2v(m_blurSizeid, 1, m_blurSize);

		m_shaderProgram->setUniformi(m_textureid, 0);
		m_frameBuffer->getTexture(0)->activate(0);



		m_opengl->drawVertexArray(xdl::XDEVL_PRIMITIVE_TRIANGLES, 6);

		m_frameBuffer2->deactivate();
		
		

		// This is important. Set the output texture.
		setOutputTexture(0, m_frameBuffer2->getTexture(0));

	}




}
