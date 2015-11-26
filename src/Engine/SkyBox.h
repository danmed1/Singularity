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

#ifndef SKYBOX_H
#define SKYBOX_H

#include <XdevLRAI/XdevLTextureCube.h>
#include <XdevLTypes.h>
#include <Game/Actor.h>

namespace soan {

	namespace game {

		/**
			@class SkyBox
			@brief Represents a SkyBox that holds 6 Textures as a Cube Texture.
		*/
		class SkyBox  {
			public:

				SkyBox(xdl::IPXdevLRAI openGL);

				virtual ~SkyBox();

				/// Initialize the SkyBox.
				xdl::xdl_int init();

				/// Returns the Cube Texture of this SkyBox.
				xdl::XdevLTextureCube* getSkyBoxTexture();

				/// Returns the material.
				Material&	getMaterial();

				/// Render the SkyBox.
				void render();

			private:
				xdl::IPXdevLRAI		m_openGL;
				xdl::XdevLVertexArray*	m_vertexArray;
				xdl::XdevLVertexBuffer* m_vertexBuffer;				
				xdl::XdevLTextureCube*	m_textureCube;
				Material*								m_material;

		};
	}
}

#endif
