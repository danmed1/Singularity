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

#include <XdevLError.h>
#include "Engine/TextureServer.h"
#include "Engine/SkyBox.h"
#include "Types.h"
#include <vector>
#include <string>

namespace soan {

	namespace game {

		SkyBox::SkyBox(xdl::IPXdevLRAI openGL) :
			m_openGL(openGL),
			m_vertexArray(nullptr),
			m_vertexBuffer(nullptr) {

		}

		SkyBox::~SkyBox() {
			delete m_material;
			m_openGL->destroy(m_vertexArray);
			m_openGL->destroy(m_vertexBuffer);
			
			// 
			// The Cube Texture will be deleted by the Texture Server.
			//
			
		}

		xdl::xdl_int SkyBox::init() {

			const char* names [] = { 	"Skybox/right.jpg",
			                          "Skybox/left.jpg",
			                          "Skybox/bottom.jpg",
			                          "Skybox/top.jpg",
			                          "Skybox/back.jpg",
			                          "Skybox/front.jpg"
			                       };

			soan::TextureServer::Inst()->setResourcePathPrefix("resources/models");
			m_textureCube = soan::TextureServer::Inst()->importCube(names);
			if(m_textureCube == nullptr) {
				return xdl::ERR_ERROR;
			}
			m_textureCube->lock();
			m_textureCube->generateMipMap();
			m_textureCube->unlock();
			

			xdl::xdl_float points[] = {

				1.0f, -1.0f, -1.0f,
				1.0f, -1.0f,  1.0f,
				1.0f,  1.0f,  1.0f,
				1.0f,  1.0f,  1.0f,
				1.0f,  1.0f, -1.0f,
				1.0f, -1.0f, -1.0f,

				-1.0f, -1.0f,  1.0f,
				-1.0f, -1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f,  1.0f,
				-1.0f, -1.0f,  1.0f,

				-1.0f,  1.0f, -1.0f,
				-1.0f, -1.0f, -1.0f,
				1.0f, -1.0f, -1.0f,
				1.0f, -1.0f, -1.0f,
				1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,

				-1.0f, -1.0f, 1.0f,
				-1.0f,  1.0f, 1.0f,
				1.0f,  1.0f,  1.0f,
				1.0f,  1.0f,  1.0f,
				1.0f, -1.0f,  1.0f,
				-1.0f, -1.0f, 1.0f,

				-1.0f,  1.0f, -1.0f,
				1.0f,  1.0f, -1.0f,
				1.0f,  1.0f,  1.0f,
				1.0f,  1.0f,  1.0f,
				-1.0f,  1.0f,  1.0f,
				-1.0f,  1.0f, -1.0f,

				-1.0f, -1.0f, -1.0f,
				-1.0f, -1.0f,  1.0f,
				1.0f, -1.0f, -1.0f,
				1.0f, -1.0f, -1.0f,
				-1.0f, -1.0f,  1.0f,
				1.0f, -1.0f,  1.0f
			};

			//
			// Create the Vertex Buffer for the SkyBox Mesh.
			//
			xdl::XdevLVertexDeclaration* vd = new xdl::XdevLVertexDeclaration();
			vd->add(3, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, VERTEX_POSITION);

			m_openGL->createVertexBuffer(&m_vertexBuffer);

			m_vertexBuffer->init((xdl::xdl_uint8*)points, vd->vertexSize()*36);

			//
			// Create the Vertex Array for this SkyBox.
			//
			m_openGL->createVertexArray(&m_vertexArray);
			m_vertexArray->init(m_vertexBuffer, vd);

			m_material = new soan::Material();
			m_material->setTexture(m_textureCube);

			return xdl::ERR_OK;
		}

		void SkyBox::render() {
			m_openGL->setActiveVertexArray(m_vertexArray);
			m_openGL->drawVertexArray(xdl::XDEVL_PRIMITIVE_TRIANGLES, 36);
		}

		xdl::XdevLTextureCube*  SkyBox::getSkyBoxTexture() {
			return m_textureCube;
		}

		Material& SkyBox::getMaterial() {
			return *m_material;
		}
	}
}
