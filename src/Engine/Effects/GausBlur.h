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

#ifndef GAUS_BLUR_H
#define GAUS_BLUR_H

#include "PostProcessEffect.h"

namespace soan {
	
	class GausBlur : public PostProcessEffect {
	public:
			GausBlur(xdl::IPXdevLRAI opengl);
			virtual ~GausBlur();
			virtual void setBlurSize(xdl::xdl_float xSize, xdl::xdl_float ySize);
			virtual int init(xdl::xdl_uint width, xdl::xdl_uint height, xdl::XdevLFrameBufferColorFormat textureFormat);
			virtual void apply();
	private:

		xdl::IPXdevLFrameBuffer 			m_frameBuffer;	
		xdl::IPXdevLFrameBuffer 			m_frameBuffer2;	
		xdl::IPXdevLShaderProgram 		m_shaderProgram;
		xdl::IPXdevLVertexShader			m_vs;
		xdl::IPXdevLFragmentShader		m_fs;		
		
		xdl::xdl_uint 							m_textureid;
		xdl::xdl_uint 							m_blurDirectionid;
		xdl::xdl_uint 							m_blurSizeid;
	
	
		xdl::xdl_float 			 				m_blurSize[2];
	};

}


#endif
