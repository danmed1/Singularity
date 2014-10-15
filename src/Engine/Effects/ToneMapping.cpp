#include "ToneMapping.h"
#include <tm/tm.h>


namespace soan {

	
	ToneMapping::ToneMapping(xdl::XdevLOpenGL330* opengl) : PostProcessEffect(opengl) {

	}
	
	ToneMapping::~ToneMapping() {
		delete m_vs;
		delete m_fs;
		delete m_shaderProgram;
		delete m_frameBuffer;
	}
	
	int ToneMapping::init(unsigned int width, unsigned height) {
		
		PostProcessEffect::init(width, height);
			
		
		std::string filename("toneMapping");
		std::string tmp("resources/shaders/");
		std::string vs_filename = tmp + filename;
		std::string fs_filename = tmp + filename;
		
		vs_filename += ".vs";
		fs_filename += ".fs";


		m_opengl->createShaderProgram(&m_shaderProgram);

		if(m_opengl->createVertexShader(&m_vs) == -1) {
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
		
		m_projMatrix 		= 	m_shaderProgram->getUniformLocation("projMatrix");
		
		m_original 			=  	m_shaderProgram->getUniformLocation("texture0");
		m_blur 					=  	m_shaderProgram->getUniformLocation("texture1");

		m_blurDirection	=  	m_shaderProgram->getUniformLocation("blurDirection");		
		m_blurSize 			= 	m_shaderProgram->getUniformLocation("blurSize");
				
		// Create Framebuffer with 4 render targets and one depth buffer.
		m_opengl->createFrameBuffer(&m_frameBuffer);
		if(m_frameBuffer->init(m_width, m_height) != xdl::ERR_OK) {
			std::cerr << "GBuffer::Could not create Framebuffer." << std::endl;
			return -1;
		}
		m_frameBuffer->addColorTarget(0, xdl::XDEVL_FB_COLOR_RGBA32F);
		
		return 0;
	}
	void ToneMapping::calculate() {
		assert(m_inputTexure[0] && "ToneMapping::calculate: Input texture not specified");
		
		glViewport(0, 0, getWidth(), getHeight());
		tmath::mat4 fbProjection;
		tmath::ortho(0.0f,
		                          (float)getWidth(),
		                          0.0f,
		                          (float)getHeight(),
		                          -1.0f,
		                          1.0f, fbProjection);
															


			// 
			// First pass.
			//
			m_frameBuffer->activate();
			
			m_shaderProgram->activate();
			m_shaderProgram->setUniformMatrix4(m_projMatrix, 1, fbProjection);
			m_shaderProgram->setUniformui(m_blurDirection, 0);

			
			m_shaderProgram->setUniformi(m_original, 0);
			m_inputTexure[0]->activate(0);

			m_shaderProgram->deactivate();
			
			m_opengl->setActiveShaderProgram(m_shaderProgram);
			m_opengl->setActiveVertexArray(m_va);
			
			m_opengl->drawVertexArray(xdl::XDEVL_PRIMITIVE_TRIANGLES, 6);
			
			m_frameBuffer->deactivate();
			

			// This is important. Set the output texture.
			setOutputTexture(0, m_frameBuffer->getTexture(0));

	}




}
