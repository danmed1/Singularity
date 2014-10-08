#ifndef GBUFFER_H
#define GBUFFER_H

#include <XdevLOpenGL/XdevLOpenGL.h>
#include <XdevLOpenGL/XdevLFrameBuffer.h>
#include "Light.h"
#include "Material.h"
#include "Effects/ShadowMap.h"

namespace soan {

	class Camera;

	/**
		@class GBuffer
		@brief The Deferred Rendering G-Buffer.
	*/
	class GBuffer {
		public:
			GBuffer(xdl::XdevLOpenGL330* opengl, Camera* camera);
			~GBuffer();

			/**
				@enum GBufferTexture
				@brief Types of G-Buffer textures.
			*/
			enum GBufferTexture {
			  POSITION 	= 0,		/// Holds the position of the fragment.
			  NORMAL		= 1,		/// Holds the normal of the fragment.
			  DIFFUSE		= 2,		/// Holds the diffuse color of the fragment.
			  LIGHTING	= 3,		/// Holds the calculated lighting of the scene.
			  DEPTH							/// Holds the depth of the fragment.
			};

			/// Initialize the G-Buffer.
			int init(unsigned int width, unsigned int height);

			/// Start the filling of the G-Buffer.
			int startFillGBuffer();

			/// stop the filling of the G-Buffer.
			int stopFillGBuffer();

			/// Begin the Lighting Stage.
			int startLightingStage();

			/// Return a specific texture of the G-Buffer.
			xdl::XdevLTexture* getTexture(GBufferTexture texture);

			/// Returns the width of the G-Buffer.
			unsigned int getViewPortWidth();

			/// Returns the height of the G-Buffer.
			unsigned int getViewPortHeight();

			/// Returns the G-Buffer object.
			xdl::XdevLFrameBuffer* 	getGBuffer();

			/// Returns the Lighting framebuffer object.
			xdl::XdevLFrameBuffer* 	getLightingFrameBuffer();

			/// Returns the Shader Program that fills the G-Buffer.
			xdl::XdevLShaderProgram* getFillGBufferShaderProgram();

			/// Returns the Shader Program that does the lighting/shadowing calculation.
			xdl::XdevLShaderProgram* getLightingShaderProgram();

			/// Add a light source to the G-Buffer.
			void addLight(Light* light);

			/// Return a light from the G-Buffer.
			Light* getLight(unsigned int idx);

			/// Set the depth bias matrix for the Shadow Mapping process.
			void setDepthBias(tmath::mat4& depthBias);

			/// Set the Shadow Map texture.
			void setShadowMap(ShadowMap* shadowMap);

			/// Sets the projection matrix which is used during filling the G-Buffer.
			void setProjectionMatrix(const tmath::mat4& projectionMatrix);

			/// Returns the projection Matrix for the G-Buffer filling stage.
			tmath::mat4& getProjectionMatrix();

			/// Sets the view matrix which is used during filling the G-Buffer.
			void setViewMatrix(const tmath::mat4& viewMatrix);

			/// Returns the view Matrix for the G-Buffer filling stage.
			tmath::mat4& getViewMatrix();

			/// Sets the model matrix which is used during filling the G-Buffer.
			void setModelMatrix(const tmath::mat4& modelMatrix);

			/// Returns the model Matrix for the G-Buffer filling stage.
			tmath::mat4& getModelMatrix();

			/// Sets the material properties during filling the G-Buffer.
			void setMaterial(const Material& material);

			/// Returns the material used during the filling of the G-Buffer.
			Material& getMaterial();

			/// Returns the modelView Matrix ID of the filling shader program.
			xdl::xdl_uint getModelViewMatrixID();

			/// Sets the debug rendering mode.
			void setDebugMode(xdl::xdl_int mode);

			/// Sets the SkyBox texture.
			void setReflectionTextureCube(xdl::XdevLTextureCube* textureCube, xdl::xdl_int state);

		private:
			xdl::XdevLOpenGL330* 					m_opengl;
			xdl::XdevLVertexArray*				m_va;
			xdl::XdevLFrameBuffer*				m_fb;


			xdl::XdevLTextureCube*				m_skyBoxTexture;
			xdl::XdevLTextureCube*				m_reflectionTextureCube;

			xdl::XdevLVertexShader*				m_stage1vs;
			xdl::XdevLFragmentShader*		m_stage1fs;
			xdl::XdevLShaderProgram*			m_stage1sp;

			xdl::XdevLVertexShader*				m_stage2vs;
			xdl::XdevLFragmentShader*		m_stage2fs;
			xdl::XdevLShaderProgram*			m_stage2sp;

			xdl::XdevLShaderProgram*			m_debugShaderProgram;
			xdl::XdevLFragmentShader* 		m_debugFragmentShader;



			xdl::xdl_uint projMatrixStage1;
			xdl::xdl_uint viewMatrixStage1;
			xdl::xdl_uint modelMatrixStage1;

			xdl::xdl_uint colorTexture;
			xdl::xdl_uint posTexture;
			xdl::xdl_uint normTexture;
			xdl::xdl_uint blendTexture;
			xdl::xdl_uint renderMode;

			xdl::xdl_uint lightPos;
			xdl::xdl_uint lightDirection;
			xdl::xdl_uint lightIntensity;
			xdl::xdl_uint lightSpecular;
			xdl::xdl_uint lightAmbient;

			xdl::xdl_uint cameraPosShader1;
			xdl::xdl_uint cameraPos;

			xdl::xdl_uint texture0;
			xdl::xdl_uint debug;

			xdl::xdl_uint reflectionTextureCube;
			xdl::xdl_uint renderSkyBox;

			xdl::xdl_int 	m_sh_diffuse;
			xdl::xdl_int 	m_sh_ambient;
			xdl::xdl_int 	m_sh_specular;
			xdl::xdl_int 	m_sh_roughness;
			xdl::xdl_int	m_sh_diffuseMap;
			xdl::xdl_int	m_sh_normalMap;
			xdl::xdl_int	m_sh_displacementMap;
			xdl::xdl_int	m_sh_specularMap;
			xdl::xdl_int 	m_sh_roughnessMap;
			xdl::xdl_int	m_sh_skyBoxTexture;
			xdl::xdl_int	m_sh_flags;



			xdl::xdl_uint depthBiasMatrix;
			xdl::xdl_uint shadowMapTexture;
			xdl::xdl_uint shadowMapSize;
			xdl::xdl_uint shadowMapMinVariance;
			xdl::xdl_uint shadowMapReduceLightBleedingAmount;

			xdl::xdl_uint viewMatrixStage2;

			xdl::xdl_bool m_begin;
			xdl::xdl_bool m_beginStage1;
			xdl::xdl_bool m_blendStage;

			Camera*							m_camera;
			std::vector<Light*>	m_lights;
			ShadowMap*					m_shadowMap;

			xdl::xdl_uint m_viewPortWidth;
			xdl::xdl_uint m_viewPortHeight;

			tmath::mat4 	m_depthBiasVP;
			tmath::mat4 	m_projectionMatrix;
			tmath::mat4 	m_viewMatrix;
			tmath::mat4 	m_modelMatrix;

			Material			m_material;

			xdl::xdl_int m_debugMode;

	};

}

#endif
