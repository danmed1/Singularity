#include "DepthOfField.h"
#include <tm/tm.h>
#include <tm/tmg.h>

namespace soan {


	DepthOfField::DepthOfField(xdl::XdevLOpenGL330* opengl) : PostProcessEffect(opengl) {

	}

	DepthOfField::~DepthOfField() {
		m_opengl->destroy(m_vs);
		m_opengl->destroy(m_fs);
		m_opengl->destroy(m_frameBuffer);
		m_opengl->destroy(m_shaderProgram);
	}


	int DepthOfField::init(unsigned int width, unsigned height) {

		PostProcessEffect::init(width, height);


		std::string filename("depthOfField");
		std::string tmp("resources/shaders/");
		std::string vs_filename = tmp + filename;
		std::string fs_filename = tmp + filename;

		vs_filename += ".vs";
		fs_filename += ".fs";


		m_opengl->createShaderProgram(&m_shaderProgram);

		if(m_opengl->createVertexShader( &m_vs) == -1) {
			std::cerr << "GBuffer::Could not compile vertex shader." << std::endl;
			return -1;
		}
		m_vs->compileFromFile(vs_filename.c_str());

		if(m_opengl->createFragmentShader(&m_fs) == -1) {
			std::cerr << "GBuffer::Could not compile fragment shader." << std::endl;
			return -1;
		}

		m_fs->compileFromFile(fs_filename.c_str());

		m_shaderProgram->attach(m_vs);
		m_shaderProgram->attach(m_fs);
		m_shaderProgram->link();

		m_original 			=  	m_shaderProgram->getUniformLocation("texture0");
		m_depth 				=  	m_shaderProgram->getUniformLocation("texture1");
		m_textureWidth	=  	m_shaderProgram->getUniformLocation("textureWidth");;
		m_textureHeight	=  	m_shaderProgram->getUniformLocation("textureHeight");;

		// Create Framebuffer with 4 render targets and one depth buffer.
		m_opengl->createFrameBuffer(&m_frameBuffer);
		if(m_frameBuffer->init(m_width, m_height) != xdl::ERR_OK) {
			std::cerr << "GBuffer::Could not create Framebuffer." << std::endl;
			return -1;
		}
		
		//
		// Now we are going to create the texture we are going to render to it.
		//
		xdl::XdevLTexture* texture;
		m_opengl->createTexture(&texture);
		texture->init(m_width, m_height, xdl::XDEVL_RGBA32F);
		
		// 
		// That texture we use as a color target to render to it.
		//
		m_frameBuffer->addColorTarget(0, texture);

		// This is important. Set the output texture.
		setOutputTexture(0, texture);

		return 0;
	}
	void DepthOfField::apply() {
		assert(m_inputTexure[0] && "DepthOfField::calculate: Input texture not specified");

		glViewport(0, 0, getWidth(), getHeight());

		//
		// First pass.
		//
		m_frameBuffer->activate();

		// Let's clear the diffuse target.
		xdl::xdl_uint l1[] = {xdl::XDEVL_COLOR_TARGET0};
		m_frameBuffer->activateColorTargets(1, l1);


		m_shaderProgram->activate();

		m_shaderProgram->setUniformi(m_original, 	0);
		m_shaderProgram->setUniformi(m_depth,			1);
		getInputTexture(0)->activate(0);
		getInputTexture(1)->activate(1);

		m_shaderProgram->setUniform(m_textureWidth, 	getWidth());
		m_shaderProgram->setUniform(m_textureHeight, 	getHeight());



		m_opengl->setActiveVertexArray(m_va);
		m_opengl->setActiveShaderProgram(m_shaderProgram);
		m_opengl->drawVertexArray(xdl::XDEVL_PRIMITIVE_TRIANGLES, 6);

		m_shaderProgram->deactivate();
		m_frameBuffer->deactivate();




	}




}
