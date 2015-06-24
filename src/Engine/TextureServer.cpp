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

//**********************************************
//Singleton Texture Manager class
//Written by Ben English
//benjamin.english@oit.edu
//
//For use with OpenGL and the FreeImage library
//**********************************************

#include "TextureServer.h"
#include "FreeImage.h"

namespace soan {

	TextureServer* TextureServer::m_inst(0);

	TextureServer* TextureServer::Inst() {
		if(!m_inst)
			m_inst = new TextureServer();

		return m_inst;
	}

	TextureServer::TextureServer() :
		m_opengl(nullptr),
		m_defaultTexture(nullptr)  {
		// call this ONLY when linking with FreeImage as a static library
#ifdef FREEIMAGE_LIB
		FreeImage_Initialise();
#endif
	}

//these should never be called
//TextureServer::TextureServer(const TextureServer& tm){}
//TextureServer& TextureServer::operator=(const TextureServer& tm){}

	TextureServer::~TextureServer() {
		// call this ONLY when linking with FreeImage as a static library
#ifdef FREEIMAGE_LIB
		FreeImage_DeInitialise();
#endif

		removeAll();
		m_inst = 0;
	}

	xdl::xdl_int TextureServer::init(xdl::XdevLOpenGL330* opengl, const char* resourcePathPrefix) {
		m_opengl = opengl;
		m_resourcePathPrefix = resourcePathPrefix;

		m_defaultTexture = import("base_texture.png");
		if(m_defaultTexture == nullptr) {
			std::cerr << "TextureServer::init: Could not assign default texture" << std::endl;
			return xdl::ERR_ERROR;
		}
		return xdl::ERR_OK;
	}

	xdl::XdevLTexture*  TextureServer::getDefaultTexture() {
		return m_defaultTexture;
	}

	void TextureServer::setResourcePathPrefix(const char* prefix) {
		m_resourcePathPrefix = prefix;
	}

	const char* TextureServer::getResourcePathPrefix() {
		return m_resourcePathPrefix.c_str();
	}

	xdl::XdevLTexture* TextureServer::import(const char* filename,
	        xdl::xdl_uint image_format,
	        xdl::xdl_uint internal_format,
	        unsigned int level,
	        unsigned int border) {

		assert(m_opengl && "TextureServer not initialized.");

		xdl::XdevLTexture* tmp = nullptr;

		// Here we try to figure out if a image with the same filename was already loaded.
		// If yes then only a referenced texture will be assigned.
		xdl::xdl_int value = m_texInfo[filename];
		if(value == 0) {
			//
			// If not then a new texture will be created.
			//
			std::string filetmp;
			if(m_resourcePathPrefix.size() > 0) {
				filetmp += m_resourcePathPrefix + "/";
			}
			filetmp += filename;

			FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

			FIBITMAP* dib = nullptr;

			BYTE* bits 		= nullptr;

			//image width and height
			unsigned int width(0), height(0);



			// Check the file signature and deduce its format
			fif = FreeImage_GetFileType(filetmp.c_str(), 0);
			if(fif == FIF_UNKNOWN) {
				fif = FreeImage_GetFIFFromFilename(filetmp.c_str());
				if(fif == FIF_UNKNOWN) {
					std::cerr << "##TextureServer::import:FreeImage_GetFIFFromFilename() failed. File format unknown" << std::endl;
					return nullptr;
				}
			}


			//check that the plugin has reading capabilities and load the file
			if(FreeImage_FIFSupportsReading(fif)) {
				std::cout << "TextureServer::import: Import image: " << filetmp << std::endl;
				dib = FreeImage_Load(fif, filetmp.c_str());
				if(!dib) {
					std::cerr << "##TextureServer::import:FreeImage_Load() failed." << std::endl;
					return nullptr;
				}
			} else {
				std::cerr << "##TextureServer::import:FreeImage_FIFSupportsReading() failed." << std::endl;
			}

			// Convert non-32 bit images
			if(FreeImage_GetBPP(dib) != 32) {
				FIBITMAP* hOldImage = dib;
				dib = FreeImage_ConvertTo32Bits(hOldImage);
				FreeImage_Unload(hOldImage);
			}

			bits = FreeImage_GetBits(dib);
			width = FreeImage_GetWidth(dib);
			height = FreeImage_GetHeight(dib);
			if((bits == 0) || (width == 0) || (height == 0))
				return nullptr;


			unsigned bytespp = FreeImage_GetLine(dib) / width;
			if(bytespp == 3) {
				image_format = xdl::XDEVL_BGR;
			} else if(bytespp == 4) {
				image_format = xdl::XDEVL_BGRA;
			}


			// Create the Texture object.
			m_opengl->createTexture(&tmp);
			tmp->init(width, height, internal_format,image_format, (xdl::xdl_uint8*)bits);

			//store the texture ID mapping
			m_texID[tmp->id()] = tmp;
			m_texInfo[filename] = tmp->id();


			//Free FreeImage's copy of the data
			FreeImage_Unload(dib);
			std::cout << "TextureServer::import: File: " << filetmp << " imported succesful." << std::endl;

		} else {
			tmp = m_texID[value];
			std::cout << "TextureServer::import: Assigned succesful." << std::endl;
		}

		return tmp;
	}

	xdl::XdevLTextureCube* TextureServer::importCube(const char* filenames[],
	        xdl::xdl_uint image_format,
	        xdl::xdl_uint internal_format,
	        unsigned int level,
	        unsigned int border) {

		xdl::XdevLTextureCube* tmp = nullptr;

		struct imagesInfo {
			std::vector<FIBITMAP*> 	dibs;
			std::vector<BYTE*> 			bits;
			unsigned int 						w;
			unsigned int 						h;
		};

		imagesInfo info;


		// Here we try to figure out if a image with the same filename was already loaded.
		// If yes then only a referenced texture will be assigned.
		for(unsigned int idx = 0; idx < 6; idx++) {
			xdl::xdl_int value = m_texCubeInfo[filenames[idx]];
			if(value == 0) {
				//
				// If not then a new texture will be created.
				//
				std::string filetmp = m_resourcePathPrefix + "/";
				filetmp += filenames[idx];

				FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

				FIBITMAP* dib = nullptr;


				//image width and height
				unsigned int width(0), height(0);



				// Check the file signature and deduce its format
				fif = FreeImage_GetFileType(filetmp.c_str(), 0);
				if(fif == FIF_UNKNOWN) {
					fif = FreeImage_GetFIFFromFilename(filetmp.c_str());
					if(fif == FIF_UNKNOWN) {
						std::cerr << "##TextureServer::import:FreeImage_GetFIFFromFilename() failed. File format unknown" << std::endl;
						return nullptr;
					}
				}


				//check that the plugin has reading capabilities and load the file
				if(FreeImage_FIFSupportsReading(fif)) {
					std::cout << "TextureServer::import: Import image: " << filetmp << std::endl;
					dib = FreeImage_Load(fif, filetmp.c_str());
					if(!dib) {
						std::cerr << "##TextureServer::import:FreeImage_Load() failed." << std::endl;
						return nullptr;
					}
				} else {
					std::cerr << "##TextureServer::import:FreeImage_FIFSupportsReading() failed." << std::endl;
				}

				// Convert non-32 bit images
				if(FreeImage_GetBPP(dib) != 32) {
					FIBITMAP* hOldImage = dib;
					dib = FreeImage_ConvertTo32Bits(hOldImage);
					FreeImage_Unload(hOldImage);
				}

				BYTE* bits = FreeImage_GetBits(dib);

				width = FreeImage_GetWidth(dib);
				height = FreeImage_GetHeight(dib);
				if((bits == 0) || (width == 0) || (height == 0))
					return nullptr;

				info.w = width;
				info.h = height;
				info.dibs.push_back(dib);
				info.bits.push_back(bits);

				unsigned bytespp = FreeImage_GetLine(dib) / width;
				if(bytespp == 3) {
					image_format = xdl::XDEVL_BGR;
				} else if(bytespp == 4) {
					image_format = xdl::XDEVL_BGRA;
				}


				std::cout << "TextureServer::import: File: " << filetmp << " imported succesful." << std::endl;

			} else {
				tmp = m_texCubeID[value];
				std::cout << "TextureServer::import: Assigned succesful." << std::endl;
			}
		} // for

		// Create the Texture object.
		m_opengl->createTextureCube(&tmp);
		xdl::XdevLCubemapPosition cubemapPositionList[] = {	xdl::XDEVL_TEXTURE_CUBE_MAP_POSITIVE_X,
		        xdl::XDEVL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		        xdl::XDEVL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		        xdl::XDEVL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		        xdl::XDEVL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		        xdl::XDEVL_TEXTURE_CUBE_MAP_NEGATIVE_Z
		                                                  };
		tmp->init(info.w, info.h, internal_format, image_format, cubemapPositionList, info.bits.data());

		//store the texture ID mapping
		m_texCubeID[tmp->id()] = tmp;
		m_texCubeInfo["SkyBox"] = tmp->id();

		//Free FreeImage's copy of the data
		for(auto& dib : info.dibs) {
			FreeImage_Unload(dib);
		}
		return tmp;
	}


	bool TextureServer::remove(xdl::XdevLTexture* texture) {
		bool result(true);
		//if this texture ID mapped, unload it's texture, and remove it from the map
		if(m_texID.find(texture->id()) != m_texID.end()) {
			delete m_texID[texture->id()];
			m_texID.erase(texture->id());
		}
		//otherwise, unload failed
		else {
			result = false;
		}

		return result;
	}

	xdl::XdevLTexture*  TextureServer::getTexture(xdl::xdl_int idx) {
		return m_texID[idx];
	}

	bool TextureServer::find(xdl::XdevLTexture* texture) {
		bool result(true);
		//if this texture ID mapped, bind it's texture as current
		if(m_texID.find(texture->id()) == m_texID.end())
			result = false;

		return result;
	}

	void TextureServer::removeAll() {

		//Unload the textures untill the end of the texture map is found
		for(auto& texture : m_texID) {
			m_opengl->destroy(texture.second);
			remove(texture.second);
		}

		//clear the texture map
		m_texID.clear();
	}

}
