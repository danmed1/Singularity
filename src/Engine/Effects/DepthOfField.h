#ifndef DEPTH_OF_FIELD_H
#define DEPTH_OF_FIELD_H

#include "PostProcessEffect.h"

namespace soan {

	class DepthOfField : public PostProcessEffect {
	public:
			DepthOfField(xdl::XdevLOpenGL330* opengl);
			virtual ~DepthOfField();
			virtual int init(unsigned int width, unsigned height);
			virtual void apply();
	private:

		xdl::XdevLFrameBuffer* 			m_frameBuffer;	
		xdl::XdevLShaderProgram* 		m_shaderProgram;
		xdl::XdevLVertexShader*			m_vs;
		xdl::XdevLFragmentShader*		m_fs;
		
		xdl::xdl_uint 							m_original;
		xdl::xdl_uint 							m_depth;
		xdl::xdl_uint 							m_textureWidth;
		xdl::xdl_uint 							m_textureHeight;
		
	};

}


#endif
