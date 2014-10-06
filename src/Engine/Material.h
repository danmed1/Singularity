#ifndef MATERIAL_H
#define MATERIAL_H

#include <XdevLOpenGL/XdevLOpenGL.h>
#include <map>

namespace soan {

	class Material {
		public:

			Material();

			enum MaterialTextures {
			  DIFFUSE_MAP,
			  NORMAL_MAP,
				DISPLACEMENT_MAP,
			  SPECULAR_MAP,
			  ROUGHNESS_MAP
			};

			/// Returns a referenced copy of the mesh.
			/**
				A referenced copy is a not a full copy. The mesh of this model will
				be shared.
			*/
			Material* refCopy();

			void setUseDiffuseConst(xdl::xdl_bool state);
			void setUseNormalConst(xdl::xdl_bool state);
			void setUseSpecularConst(xdl::xdl_bool state);
			void setRoughnessConst(xdl::xdl_bool state);
			
			void setUseDiffuseMap(xdl::xdl_bool state);
			void setUseNormalMap(xdl::xdl_bool state);
			void setUseDisplacementMap(xdl::xdl_bool state);
			void setUseSpecularMap(xdl::xdl_bool state);
			void setRoughnessMap(xdl::xdl_bool state);

			/// Returns the state if a diffuse map should be used.
			xdl::xdl_bool getUseDiffuseMap();

			/// Returns the state if a normal map should be used.
			xdl::xdl_bool getUseNormalMap();
			
			/// Returns the state if a displacement map should be used.
			xdl::xdl_bool getUseDisplacementMap();
			
			/// Returns the state if a specular map should be used.
			xdl::xdl_bool getUseSpecularMap();

			/// Returns the state if a roughness map should be used.
			xdl::xdl_bool getUseRoughnessMap();

			xdl::xdl_uint getStates();

			/// Sets the diffuse color of the material.
			void setDiffuse(float r, float g, float b, float a);

			/// Sets the ambient color of the material.
			void setAmbient(float r, float g, float b, float a);

			/// Sets the specular color of the material.
			void setSpecular(float r, float g, float b, float a);

			/// Sets the roughness of the material.
			void setRoughness(float roughness);

			/// Returns a pointer to the diffuse color array.
			float* getDiffuse();

			/// Returns a pointer to the ambient color array.
			float* getAmbient();

			/// Returns a pointer to the specular color array.
			float* getSpecular();

			/// Returns the shininess factor.
			float getShininess();

			/// Returns the roughness.
			float getRoughness();

			/// Sets the texture.
			void setTexture(MaterialTextures texture_index, xdl::XdevLTexture* texture);

			/// Sets the skybox texture.
			void setTexture(xdl::XdevLTextureCube* skybox);

			/// Returns the number of available textures.
			unsigned int getNumTextures();

			/// Returns the texture.
			xdl::XdevLTexture* getTexture(unsigned int idx);

			/// Returns the skybox texture.
			xdl::XdevLTextureCube* getSkyBoxTexture();
	
		private:
			// Holds the diffuse part of the material.
			float diffuse[4];

			// Holds the ambient part of the material.
			float ambient[4];

			// Holds the specular part of the material.
			float specular[4];

			// Holds the roughness of the material.
			float m_roughness;

			xdl::XdevLTexture*				m_textures[7];

			xdl::XdevLTextureCube*		m_skyBoxTexture;


			xdl::xdl_bool							m_useDiffuseConst;
			xdl::xdl_bool							m_useNormalConst;
			xdl::xdl_bool							m_useSpecularConst;
			xdl::xdl_bool							m_useRoughnessConst;

			xdl::xdl_bool							m_useDiffuseMap;
			xdl::xdl_bool							m_useNormalMap;
			xdl::xdl_bool							m_useDisplacementMap;
			xdl::xdl_bool							m_useSpecularMap;
			xdl::xdl_bool							m_useRoughnessMap;
			

	};

}

#endif
