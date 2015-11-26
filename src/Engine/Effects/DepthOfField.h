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

#ifndef DEPTH_OF_FIELD_H
#define DEPTH_OF_FIELD_H

#include "PostProcessEffect.h"

namespace soan {

	class DepthOfField : public PostProcessEffect {
	public:
			DepthOfField(xdl::IPXdevLRAI opengl);
			virtual ~DepthOfField();
			virtual int init(unsigned int width, unsigned height);
			virtual void apply();
	private:

		xdl::XdevLFrameBuffer* 			m_frameBuffer;	
		xdl::XdevLShaderProgram* 		m_shaderProgram;
		xdl::XdevLVertexShader*			m_vs;
		xdl::XdevLFragmentShader*		m_fs;
		
		xdl::xdl_uint 							m_original;
		xdl::xdl_uint 							m_depth;
		xdl::xdl_uint 							m_textureWidth;
		xdl::xdl_uint 							m_textureHeight;
		
	};

}


#endif
