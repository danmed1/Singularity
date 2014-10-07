#include <XdevLError.h>
#include "Engine/TextureServer.h"
#include "Engine/SkyBox.h"
#include "Types.h"
#include <vector>
#include <string>

namespace soan {

	namespace game {

		SkyBox::SkyBox(xdl::XdevLOpenGL330* openGL) :
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
