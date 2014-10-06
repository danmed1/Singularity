#ifndef TONE_MAPPING_H
#define TONE_MAPPING_H

#include "PostProcessEffect.h"

namespace soan {

	class ToneMapping : public PostProcessEffect {
	public:
			ToneMapping(xdl::XdevLOpenGL330* opengl);
			virtual ~ToneMapping();
			virtual int init(unsigned int width, unsigned height);
			virtual void calculate();
	private:

		xdl::XdevLFrameBuffer* 			m_frameBuffer;	
		xdl::XdevLShaderProgram* 		m_shaderProgram;
		xdl::XdevLVertexShader*			m_vs;
		xdl::XdevLFragmentShader*		m_fs;
		
		xdl::xdl_uint 							m_projMatrix;
		
		xdl::xdl_uint 							m_original;
		xdl::xdl_uint 							m_blur;
		
		xdl::xdl_uint 							m_blurDirection;
		xdl::xdl_uint 							m_blurSize;
	};

}


#endif
