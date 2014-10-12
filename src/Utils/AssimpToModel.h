#ifndef ASSIMP_TO_MODEL_H
#define ASSIMP_TO_MODEL_H

#include "Engine/Entity/Mesh.h"
#include <tm/tm.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <memory>

namespace soan {

	namespace utils {
		/**
			@class AssimpToModel
			@brief Importer for 3D modles.

			This class is using the Assimp project to import 3D assets.
		*/
		class AssimpToModel {
			public:

				AssimpToModel(xdl::XdevLOpenGL330* openGL);

				/// Import 3d model.
				int import(const xdl::xdl_char* filename, const std::shared_ptr<Model>& model);
			private:

				/// Add a mesh to the model.
				int addMesh(const aiMesh* aimesh, const std::shared_ptr<Model>& model);

				/// Calculate the bounding box of the model.
				void getBoundingBox();

				/// Helper function for the getBoudingBox method.
				void getBoundingBoxNode(const aiNode* nd,
				                        aiVector3D* min,
				                        aiVector3D* max,
				                        aiMatrix4x4* trafo
				                       );

			private:

				/// Holds the Assimp scene.
				aiScene* 								m_aiScene;

				/// The renderer.
				xdl::XdevLOpenGL330*		m_openGL;

				/// Bouding box min value.
				tmath::vec3							m_boundingBoxMin;

				/// Bounding box max value.
				tmath::vec3							m_boundingBoxMax;
		};

	}

}

#endif
