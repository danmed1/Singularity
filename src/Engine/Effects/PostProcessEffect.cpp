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

#include "PostProcessEffect.h"
#include "Engine/Types.h"

namespace soan {

	PostProcessEffect::PostProcessEffect(xdl::IPXdevLRAI opengl) : m_opengl(opengl) {
		m_outTexure.reserve(NUM_OUTPUT_TEXTURES);
		m_outTexure.resize(NUM_OUTPUT_TEXTURES);
		m_inputTexure.reserve(NUM_INPUT_TEXTURES);
		m_inputTexure.resize(NUM_INPUT_TEXTURES);


		for(auto& ib : m_outTexure) {
			ib = nullptr;
		}

		for(auto& ib : m_inputTexure) {
			ib = nullptr;
		}

	}

	PostProcessEffect::~PostProcessEffect() {

	}

	int PostProcessEffect::init(unsigned int width, unsigned height) {
		m_width = width;
		m_height = height;

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
		if(m_va->init(list.size(), list.data(), 6, vd) != xdl::ERR_OK) {
			return -1;
		}

		return 0;
	}

	unsigned int PostProcessEffect::getWidth() {
		return m_width;
	}

	unsigned int PostProcessEffect::getHeight() {
		return m_height;
	}

	xdl::IPXdevLTexture PostProcessEffect::getInputTexture(unsigned int idx) {
		assert((idx < NUM_INPUT_TEXTURES) && "PostProcessEffect::getInputTexture: Argument not valid.");
		return m_inputTexure[idx];
	}

	xdl::IPXdevLTexture PostProcessEffect::getOutputTexture(unsigned int idx) {
		return m_outTexure[idx];
	}

	void PostProcessEffect::setInputTexture(unsigned int idx, xdl::IPXdevLTexture texture) {
		m_inputTexure[idx] = texture;
	}

	void PostProcessEffect::setOutputTexture(unsigned int idx, xdl::IPXdevLTexture texture) {
		m_outTexure[idx] = texture;
	}
}
