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

#include <GL/glew.h>

#include "DepthOfField.h"
#include <tm/tm.h>


namespace soan {


	DepthOfField::DepthOfField(xdl::IPXdevLRAI opengl) : PostProcessEffect(opengl) {

	}

	DepthOfField::~DepthOfField() {

	}


	int DepthOfField::init(unsigned int width, unsigned height) {

		PostProcessEffect::init(width, height);


		std::string filename("depthOfField");
		std::string tmp("resources/shaders/");
		std::string vs_filename = tmp + filename;
		std::string fs_filename = tmp + filename;

		vs_filename += ".vs";
		fs_filename += ".fs";


		m_shaderProgram = m_rai->createShaderProgram();

		m_vs = m_rai->createVertexShader();
		m_vs->compileFromFile(vs_filename.c_str());
		m_fs = m_rai->createFragmentShader();
		m_fs->compileFromFile(fs_filename.c_str());

		m_shaderProgram->attach(m_vs);
		m_shaderProgram->attach(m_fs);
		m_shaderProgram->link();

		m_original 			=  	m_shaderProgram->getUniformLocation("texture0");
		m_depth 				=  	m_shaderProgram->getUniformLocation("texture1");
		m_textureWidth	=  	m_shaderProgram->getUniformLocation("textureWidth");;
		m_textureHeight	=  	m_shaderProgram->getUniformLocation("textureHeight");;

		// Create Framebuffer with 4 render targets and one depth buffer.
		m_frameBuffer = m_rai->createFrameBuffer();
		if(m_frameBuffer->init(m_width, m_height) != xdl::ERR_OK) {
			std::cerr << "GBuffer::Could not create Framebuffer." << std::endl;
			return -1;
		}
		
		//
		// Now we are going to create the texture we are going to render to it.
		//
		auto texture = m_rai->createTexture();
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



		m_rai->setActiveVertexArray(m_va);
		m_rai->setActiveShaderProgram(m_shaderProgram);
		m_rai->drawVertexArray(xdl::XDEVL_PRIMITIVE_TRIANGLES, 6);

		m_shaderProgram->deactivate();
		m_frameBuffer->deactivate();




	}




}
