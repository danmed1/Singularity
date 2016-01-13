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

#ifndef POST_PROCESS_EFFECT_H
#define POST_PROCESS_EFFECT_H

#include <XdevLRAI/XdevLRAI.h>
#include <vector>

namespace soan {

	const unsigned int NUM_INPUT_TEXTURES = 7;
	const unsigned int NUM_OUTPUT_TEXTURES = 7;
	
	/**
		@class PostProcessEffect
		@brief Applies an effect on a input texture.
	*/
	class PostProcessEffect {
		public:
			PostProcessEffect(xdl::IPXdevLRAI opengl);

			virtual ~PostProcessEffect();

			/// Initialize the effect.
			int init(unsigned int width, unsigned height);

			/// Creates the output texture.
			virtual void apply() = 0;

			/// Sets the input texture for the calculatioin.
			void setInputTexture(unsigned int idx, xdl::IPXdevLTexture texture);

			/// Returns the new calculated texture.
			xdl::IPXdevLTexture getOutputTexture(unsigned int idx);
			
			/// Returns the internal framebuffer width.
			unsigned int getWidth();
			
			/// Return the internal framebuffer height.
			unsigned int getHeight();
			
		protected:
			/// Returns the input texture
			xdl::IPXdevLTexture getInputTexture(unsigned int idx);
			
			/// This method shall be used by the effects programmer.
			void setOutputTexture(unsigned int idx, xdl::IPXdevLTexture texture);

		protected:
			xdl::IPXdevLRAI	 						m_opengl;
			xdl::IPXdevLVertexArray 					m_va;

			std::vector<xdl::IPXdevLTexture>	m_inputTexure;
			std::vector<xdl::IPXdevLTexture>	m_outTexure;

			unsigned int m_width;
			unsigned int m_height;
	};

}

#endif
