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

#ifndef TEXTURE_SERVER_H
#define TEXTURE_SERVER_H

#include <XdevLOpenGL/XdevLOpenGL.h>
#include <map>

namespace soan {

	class TextureServer {
		public:
			typedef std::map<xdl::xdl_int, xdl::XdevLTexture*> 	textureMapType;
			typedef std::map<std::string, xdl::xdl_int> 					textureInfoMapType;
			
			typedef std::map<xdl::xdl_int, xdl::XdevLTextureCube*> 	textureCubeMapType;
			typedef std::map<std::string, xdl::xdl_int> 							textureInfoCubeMapType;

			static TextureServer* Inst();
			virtual ~TextureServer();


			//if texID is already in use, it will be unloaded and replaced with this texture
			xdl::XdevLTexture* import(const char* filename,
			                          xdl::xdl_uint image_format = xdl::XDEVL_BGR,
			                          xdl::xdl_uint internal_format = xdl::XDEVL_RGBA,
			                          unsigned int level = 0,
			                          unsigned int border = 0);

			xdl::XdevLTextureCube*	importCube(const char* filenames[],
			                          xdl::xdl_uint image_format = xdl::XDEVL_BGR,
			                          xdl::xdl_uint internal_format = xdl::XDEVL_RGBA,
			                          unsigned int level = 0,
			                          unsigned int border = 0);

			/// Remove texture.
			bool remove(xdl::XdevLTexture* texture);

			/// Returns the texture with a specific id.
			xdl::XdevLTexture* getTexture(xdl::xdl_int idx);

			/// Find texture.
			bool find(xdl::XdevLTexture* texture);

			//free all texture memory
			void removeAll();

			/// Initialize the server
			xdl::xdl_int init(xdl::XdevLOpenGL330* opengl, const char* resourcePathPrefix);

			/// Sets the resource path prefix.
			void setResourcePathPrefix(const char* prefix);

			/// Returns the resource path prefix.
			const char* getResourcePathPrefix();

			/// Returns the default textures id.
			xdl::XdevLTexture* getDefaultTexture();

		protected:
			TextureServer();
			TextureServer(const TextureServer& tm);
			TextureServer& operator=(const TextureServer& tm);

			xdl::XdevLOpenGL330* m_opengl;
			static TextureServer* m_inst;
			
			textureMapType 			m_texID;
			textureInfoMapType 	m_texInfo;
			
			textureCubeMapType 			m_texCubeID;
			textureInfoCubeMapType 	m_texCubeInfo;
			

			std::string m_resourcePathPrefix;
			xdl::XdevLTexture*			m_defaultTexture;
	};
}
#endif
