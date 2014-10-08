#ifndef GAUS_BLUR_H
#define GAUS_BLUR_H

#include "PostProcessEffect.h"

namespace soan {
	
	class GausBlur : public PostProcessEffect {
	public:
			GausBlur(xdl::XdevLOpenGL330* opengl);
			virtual ~GausBlur();
			virtual void setBlurSize(xdl::xdl_float xSize, xdl::xdl_float ySize);
			virtual int init(xdl::xdl_uint width, xdl::xdl_uint height, xdl::XdevLFrameBufferColorFormat textureFormat);
			virtual void apply();
	private:

		xdl::XdevLFrameBuffer* 			m_frameBuffer;	
		xdl::XdevLFrameBuffer* 			m_frameBuffer2;	
		xdl::XdevLShaderProgram* 		m_shaderProgram;
		xdl::XdevLVertexShader*			m_vs;
		xdl::XdevLFragmentShader*		m_fs;		
		
		xdl::xdl_uint 							m_textureid;
		xdl::xdl_uint 							m_blurDirectionid;
		xdl::xdl_uint 							m_blurSizeid;
	
	
		xdl::xdl_float 			 				m_blurSize[2];
	};

}


#endif
