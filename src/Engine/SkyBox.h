#ifndef SKYBOX_H
#define SKYBOX_H

#include <XdevLOpenGL/XdevLTextureCube.h>
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

				SkyBox(xdl::XdevLOpenGL330* openGL);

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
				xdl::XdevLOpenGL330*		m_openGL;
				xdl::XdevLVertexArray*	m_vertexArray;
				xdl::XdevLVertexBuffer* m_vertexBuffer;				
				xdl::XdevLTextureCube*	m_textureCube;
				Material*								m_material;

		};
	}
}

#endif
