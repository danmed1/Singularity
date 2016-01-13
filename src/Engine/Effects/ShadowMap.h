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

			ShadowMap(xdl::IPXdevLRAI opengl, ShadowMapModes mode);

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
			xdl::IPXdevLShaderProgram getShaderProgram();

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
			SPGausBlur									m_gausBlur;
			xdl::IPXdevLFrameBuffer			m_frameBuffer;
			xdl::IPXdevLShaderProgram		m_shaderProgram;
			xdl::IPXdevLVertexShader		m_vs;
			xdl::IPXdevLFragmentShader	m_fs;
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
