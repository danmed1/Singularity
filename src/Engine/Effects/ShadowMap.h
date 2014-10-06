#ifndef SHADOW_MAP_H
#define SHADOW_MAP_H

#include "PostProcessEffect.h"
#include "Engine/Effects/GausBlur.h"
#include "Engine/Light.h"
#include "Engine/Camera.h"

namespace soan {

	class ShadowMap : public PostProcessEffect {
	public:
			ShadowMap(xdl::XdevLOpenGL330* opengl);
			virtual ~ShadowMap();
			virtual int init(unsigned int width, unsigned height);
			
			/// Sets the current light the shadow map generator will use.
			void setLight(Light* light);
			
			void setCamera(Camera* camera);
			void activate();
			void deactivate();
			virtual void apply();
			xdl::XdevLShaderProgram* getShaderProgram();
			tmath::mat4&	getDepthBias();
			
			void setProjectionMatrix(const tmath::mat4& matrix);
			void setViewMatrix(const tmath::mat4& matrix);
			void setModelMatrix(const tmath::mat4& matrix);
			
			/// Sets the minimum variance for the VS.
			void setMinVariance(xdl::xdl_float minVariance);
			
			/// Sets the amount to reduce light bleeding for the VS.
			void setReduceLightBleedingAmount(xdl::xdl_float reduceLightBleedingAmount);
			
			/// Returns the minimum variance of the vs.
			xdl::xdl_float getMinVariance();
			
			/// Returns the reduce amount of the light bleeding.
			xdl::xdl_float getReduceLightBleedingAmount();
	private:

		xdl::XdevLFrameBuffer* 			m_frameBuffer;	
		xdl::XdevLShaderProgram* 		m_shaderProgram;
		xdl::XdevLVertexShader*			m_vs;
		xdl::XdevLFragmentShader*		m_fs;		
		soan::Camera*								m_camera;
		xdl::xdl_uint 							m_projMatrix;
		xdl::xdl_uint 							m_viewMatrix;
		xdl::xdl_uint								m_modelMatrix;	
		
		Light* 											m_light;

		tmath::mat4									m_biasMatrix;
		tmath::mat4									m_projectionMatrix;
		tmath::mat4									m_biasProjectionMatrix;
		
		
		tmath::mat4 								m_depthBiasVP;
		
		GausBlur										m_gausBlur;
		
		xdl::xdl_float							m_minVariance;
		xdl::xdl_float							m_reduceLightBleedingAmount;
	
	};

}


#endif
