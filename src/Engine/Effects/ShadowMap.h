#ifndef SHADOW_MAP_H
#define SHADOW_MAP_H

#include "PostProcessEffect.h"
#include "Engine/Effects/GausBlur.h"
#include "Engine/Light.h"
#include "Engine/Camera.h"

namespace soan {

	class ShadowMap : public PostProcessEffect {
		public:

			enum ShadowMapModes {
			  NORMAL,
			  VSM
			};

			ShadowMap(xdl::XdevLOpenGL330* opengl, ShadowMapModes mode);

			virtual ~ShadowMap();

			virtual int init(xdl::xdl_uint width, xdl::xdl_uint height);

			/// Sets the current light the shadow map generator will use.
			void setLight(Light* light);

			/// Sets the current camera the shadow map generator will use.
			void setCamera(Camera* camera);

			/// Activates the shadow map creation.
			void activate();

			/// Deactivates the shadow map creation.
			void deactivate();

			virtual void apply();

			/// Returns the shader program.
			xdl::XdevLShaderProgram* getShaderProgram();

			/// Returns the depth bias matrix.
			tmath::mat4&	getDepthBias();

			/// Sets the projection matrix.
			void setProjectionMatrix(const tmath::mat4& matrix);

			/// Sets the view matrix.
			void setViewMatrix(const tmath::mat4& matrix);

			/// Sets the model matrix.
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
			GausBlur*										m_gausBlur;
			xdl::XdevLFrameBuffer*			m_frameBuffer;
			xdl::XdevLShaderProgram*		m_shaderProgram;
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
			xdl::xdl_float							m_minVariance;
			xdl::xdl_float							m_reduceLightBleedingAmount;
			ShadowMapModes							m_shadowMapMode;

	};

}


#endif
