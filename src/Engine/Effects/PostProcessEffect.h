#ifndef POST_PROCESS_EFFECT_H
#define POST_PROCESS_EFFECT_H

#include <XdevLOpenGL/XdevLOpenGL.h>
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
			PostProcessEffect(xdl::XdevLOpenGL330* opengl);

			virtual ~PostProcessEffect();

			/// Initialize the effect.
			int init(unsigned int width, unsigned height);

			/// Creates the output texture.
			virtual void apply() = 0;

			/// Sets the input texture for the calculatioin.
			void setInputTexture(unsigned int idx, xdl::XdevLTexture* texture);

			/// Returns the new calculated texture.
			xdl::XdevLTexture* getOutputTexture(unsigned int idx);
			
			/// Returns the internal framebuffer width.
			unsigned int getWidth();
			
			/// Return the internal framebuffer height.
			unsigned int getHeight();
			
		protected:
			/// Returns the input texture
			xdl::XdevLTexture* getInputTexture(unsigned int idx);
			
			/// This method shall be used by the effects programmer.
			void setOutputTexture(unsigned int idx, xdl::XdevLTexture* texture);

		protected:
			xdl::XdevLOpenGL330* 						m_opengl;
			xdl::XdevLVertexArray* 					m_va;

			std::vector<xdl::XdevLTexture*>	m_inputTexure;
			std::vector<xdl::XdevLTexture*>	m_outTexure;

			unsigned int m_width;
			unsigned int m_height;
	};

}

#endif
