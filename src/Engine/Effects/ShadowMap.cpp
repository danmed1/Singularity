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
#include <tm/tm.h>
#include "ShadowMap.h"


namespace soan {


	ShadowMap::ShadowMap(xdl::IPXdevLRAI opengl, ShadowMapModes mode) :
		PostProcessEffect(opengl),
		m_biasMatrix(	0.5f, 0.0f, 0.0f, 0.0f,
		              0.0f, 0.5f, 0.0f, 0.0f,
		              0.0f, 0.0f, 0.5f, 0.0f,
		              0.5f, 0.5f, 0.5f, 1.0f),
		m_minVariance(0.00002f),
		m_reduceLightBleedingAmount(0.2f),
		m_shadowMapMode(mode) {

		// Orthographic projection is used for directional light.
		tmath::ortho(-20.0f, 20.0f, -20.0f, 20.0f,-80.0f, 40.0f, m_projectionMatrix);

		// Perspective is used for point lights.
		//tmath::frustum(-2.85f, 2.85f, -2.15f, 2.15f, 1.0f, 100.0f, m_projectionMatrix);

		// Let's pre-calculate the matrix.
		m_biasProjectionMatrix = m_biasMatrix*m_projectionMatrix;

		if(m_shadowMapMode == VSM) {
			m_gausBlur = std::make_shared<GausBlur>(opengl);
		}
	}

	ShadowMap::~ShadowMap() {

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

	int ShadowMap::init(xdl::xdl_uint width, xdl::xdl_uint height) {

		// TODO The texture format can be xdl::XDEVL_RG16F

		PostProcessEffect::init(width, height);
		if(m_shadowMapMode == VSM) {
			m_gausBlur->init(width, height, xdl::XDEVL_FB_COLOR_RG32F);
		}

		m_shaderProgram = m_rai->createShaderProgram();

		m_vs = m_rai->createVertexShader();
		m_vs->compileFromFile("resources/shaders/shadowMap_vs.glsl");

		m_fs = m_rai->createFragmentShader();
		m_fs->compileFromFile("resources/shaders/shadowMap_fs.glsl");

		m_shaderProgram->attach(m_vs);
		m_shaderProgram->attach(m_fs);
		m_shaderProgram->link();

		m_projMatrix 		= 	m_shaderProgram->getUniformLocation("projMatrix");
		m_viewMatrix 		= 	m_shaderProgram->getUniformLocation("viewMatrix");
		m_modelMatrix 	= 	m_shaderProgram->getUniformLocation("modelMatrix");

		// Create Framebuffer with 4 render targets and one depth buffer.
		m_frameBuffer = m_rai->createFrameBuffer();
		if(m_frameBuffer->init(m_width, m_height) != xdl::ERR_OK) {
			std::cerr << "GBuffer::Could not create Framebuffer." << std::endl;
			return -1;
		}
		m_frameBuffer->addColorTarget(0, xdl::XDEVL_FB_COLOR_RG32F);
		m_frameBuffer->getTexture(0)->lock();
		m_frameBuffer->getTexture(0)->setTextureWrap(xdl::XDEVL_TEXTURE_WRAP_S, xdl::XDEVL_CLAMP_TO_BORDER);
		m_frameBuffer->getTexture(0)->setTextureWrap(xdl::XDEVL_TEXTURE_WRAP_T, xdl::XDEVL_CLAMP_TO_BORDER);

		if(m_shadowMapMode == VSM) {
			m_frameBuffer->getTexture(0)->setTextureFilter(xdl::XDEVL_TEXTURE_MAG_FILTER, xdl::XDEVL_LINEAR);
			m_frameBuffer->getTexture(0)->setTextureFilter(xdl::XDEVL_TEXTURE_MIN_FILTER, xdl::XDEVL_LINEAR_MIPMAP_LINEAR);
			m_frameBuffer->getTexture(0)->setTextureMaxAnisotropy(16.0);
			m_frameBuffer->getTexture(0)->generateMipMap();
		} else {
			m_frameBuffer->getTexture(0)->setTextureFilter(xdl::XDEVL_TEXTURE_MAG_FILTER, xdl::XDEVL_NEAREST);
			m_frameBuffer->getTexture(0)->setTextureFilter(xdl::XDEVL_TEXTURE_MIN_FILTER, xdl::XDEVL_NEAREST);
		}
		m_frameBuffer->getTexture(0)->unlock();


		m_frameBuffer->addDepthTarget(xdl::XDEVL_FB_DEPTH_COMPONENT24);



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

		m_shaderProgram->deactivate();
		m_frameBuffer->deactivate();

		if(m_shadowMapMode == VSM) {

			m_gausBlur->setInputTexture(0, m_frameBuffer->getTexture(0));
			m_gausBlur->setBlurSize(1.0, 1.0);
			m_gausBlur->apply();
			m_frameBuffer->getTexture(0)->lock();
			m_frameBuffer->getTexture(0)->generateMipMap();
			m_frameBuffer->getTexture(0)->unlock();
			setOutputTexture(0,	m_gausBlur->getOutputTexture(0));

		} else {

			setOutputTexture(0,	m_frameBuffer->getTexture(0));

		}

	}

	xdl::IPXdevLShaderProgram ShadowMap::getShaderProgram() {
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
