#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "AssimpToModel.h"
#include "Engine/TextureServer.h"
#include "Engine/Entity/Mesh.h"
#include "Engine/Entity/Model.h"

#include <string>

namespace soan {

	namespace utils {

#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)

		AssimpToModel::AssimpToModel(xdl::XdevLOpenGL330* openGL) : m_openGL(openGL) {

		}

		int AssimpToModel::import(const xdl::xdl_char* filename, const std::shared_ptr<Model>& model) {

			// we are taking one of the postprocessing presets to avoid
			// spelling out 20+ single postprocessing flags here.
			m_aiScene = (aiScene*)aiImportFile(filename, aiProcessPreset_TargetRealtime_Quality);
			if(m_aiScene == nullptr) {
				return xdl::ERR_ERROR;
			}
			// Get the current path and assign it to the texture server.
			std::string tmp(filename);
			std::string currentPath = tmp.substr(0, tmp.find_last_of("\\/") + 1);
			TextureServer::Inst()->setResourcePathPrefix(currentPath.c_str());

			// Read all neccessary mesh data from the model.
			for(unsigned int n = 0; n < m_aiScene->mNumMeshes; ++n) {
				const aiMesh* mesh = m_aiScene->mMeshes[n];

				addMesh(mesh, model);
			}

			getBoundingBox();

			model->setBoundingBox(m_boundingBoxMin, m_boundingBoxMax);

			if(m_aiScene != nullptr) {
				aiReleaseImport(m_aiScene);
			}

			return xdl::ERR_OK;
		}

		int AssimpToModel::addMesh(const aiMesh* aimesh, const std::shared_ptr<Model>& model) {

			// Create Vertex Declaration. This describes the strucure of a vertex.
			xdl::XdevLVertexDeclaration* vd = new xdl::XdevLVertexDeclaration();

			xdl::XdevLVertexArray* va;
			m_openGL->createVertexArray(&va);


			std::vector<xdl::xdl_uint8*> list;

			// Do we have position data?
			if(aimesh->HasPositions()) {
				list.push_back((xdl::xdl_uint8*)aimesh->mVertices);
				vd->add(3, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, VERTEX_POSITION);
			}

			// Do we have normal data?
			if(aimesh->HasNormals()) {
				list.push_back((xdl::xdl_uint8*)aimesh->mNormals);
				vd->add(3, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, VERTEX_NORMAL);
			}

			if(aimesh->HasTangentsAndBitangents()) {
				list.push_back((xdl::xdl_uint8*)aimesh->mTangents);
				vd->add(3, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, VERTEX_TANGENT);

				list.push_back((xdl::xdl_uint8*)aimesh->mBitangents);
				vd->add(3, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, VERTEX_BITANGENT);
			}

			xdl::xdl_float *texCoords0 = nullptr;
			if(aimesh->HasTextureCoords(0)) {
				vd->add(2, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, VERTEX_TEXTURE_COORD);
				texCoords0 = new float[sizeof(float)*2*aimesh->mNumVertices];
				for(unsigned int k = 0; k < aimesh->mNumVertices; ++k) {

					texCoords0[k*2]   = aimesh->mTextureCoords[0][k].x;
					texCoords0[k*2+1] = aimesh->mTextureCoords[0][k].y;

				}
				list.push_back((xdl::xdl_uint8*)texCoords0);
			}
			xdl::xdl_float *texCoords1 = nullptr;
			if(aimesh->HasTextureCoords(1)) {
				vd->add(2, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, VERTEX_TEXTURE_COORD);
				texCoords1 = new float[sizeof(float)*2*aimesh->mNumVertices];
				for(unsigned int k = 0; k < aimesh->mNumVertices; ++k) {

					texCoords1[k*2]   = aimesh->mTextureCoords[0][k].x;
					texCoords1[k*2+1] = aimesh->mTextureCoords[0][k].y;

				}
				list.push_back((xdl::xdl_uint8*)texCoords1);
			}


			xdl::xdl_uint *faceArray = nullptr;
			faceArray = (xdl::xdl_uint *)new xdl::xdl_uint[aimesh->mNumFaces * 3];
			xdl::xdl_uint faceIndex = 0;

			for(xdl::xdl_uint t = 0; t < aimesh->mNumFaces; ++t) {
				const aiFace* face = &aimesh->mFaces[t];

				memcpy(&faceArray[faceIndex], face->mIndices,3 * sizeof(xdl::xdl_uint));
				faceIndex += 3;
			}


			va->init(aimesh->mNumFaces* 3,
			         (xdl::xdl_uint8*)faceArray,
			         list.size(),
			         list.data(),
			         aimesh->mNumVertices,
			         vd);

			if(texCoords0 != nullptr) {
				delete [] texCoords0;
			}
			if(texCoords1 != nullptr) {
				delete [] texCoords1;
			}
			if(faceArray != nullptr) {
				delete [] faceArray;
			}

			xdl::xdl_uint m_numFaces = aimesh->mNumFaces;
			xdl::xdl_uint m_numVertices = aimesh->mNumVertices;


			std::shared_ptr<Material> material(new Material());

			//
			// Do Material stuff here.
			//
			aiMaterial *mtl = m_aiScene->mMaterials[aimesh->mMaterialIndex];
			aiColor4D color;
			if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &color)) {
				material->setDiffuse(color.r, color.g, color.b, color.a);
			}
			if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &color)) {
				material->setAmbient(color.r, color.g, color.b, color.a);
			}
			if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &color)) {
				material->setSpecular(color.r, color.g, color.b, color.a);
			}

			float shininess 				= 0.0;
			float shininessStrength = 0.0;
			unsigned int max = 1;;
			if(AI_SUCCESS == aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max)) {
				max = 1;;
				if(AI_SUCCESS == aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &shininessStrength, &max)) {
					// TODO This is a hack, using 128.0f as normalization between shineniness and roughness.
					material->setRoughness(1.0f - shininessStrength*shininess/320.0f);
				} else {
					material->setRoughness(1.0f - shininess/320.0f);
				}
			} else {
				material->setRoughness(1.0f);
				material->setSpecular(0.0f, 0.0f, 0.0f, 0.0f);
			}


			//
			// First set default texture for all meshes. This will ensure that we have a mesh da mo neee :D.
			//
			for(unsigned int idx = 0; idx < material->getNumTextures(); idx++) {
				if(material->getTexture(Material::DIFFUSE_MAP) == nullptr) {
					material->setTexture(Material::DIFFUSE_MAP, soan::TextureServer::Inst()->getDefaultTexture());
//					material->getTexture(Material::DIFFUSE_MAP)->lock();
//					material->getTexture(Material::DIFFUSE_MAP)->setTextureFilter(xdl::XDEVL_TEXTURE_MAG_FILTER, xdl::XDEVL_NEAREST);
//					material->getTexture(Material::DIFFUSE_MAP)->setTextureFilter(xdl::XDEVL_TEXTURE_MIN_FILTER, xdl::XDEVL_NEAREST);
//					material->getTexture(Material::DIFFUSE_MAP)->setTextureWrap(xdl::XDEVL_TEXTURE_WRAP_S, xdl::XDEVL_REPEAT);
//					material->getTexture(Material::DIFFUSE_MAP)->setTextureWrap(xdl::XDEVL_TEXTURE_WRAP_T, xdl::XDEVL_REPEAT);
//					material->getTexture(Material::DIFFUSE_MAP)->generateMipMap();
//					material->getTexture(Material::DIFFUSE_MAP)->unlock();
				}
			}

			//
			// Import all textures into the system and make references if neccessary.
			//
			for(unsigned int m=0; m<m_aiScene->mNumMaterials; ++m) {
				int texIndex = 0;
				aiString path;

				//
				// Import all diffuse textures.
				//
				aiReturn texFound = m_aiScene->mMaterials[aimesh->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
				while(texFound == AI_SUCCESS) {
					// FIXME The import of the texture server can return nullptr. FIX THIS LINE HERE.
					xdl::XdevLTexture* diffuseMap =soan::TextureServer::Inst()->import(path.data);
					diffuseMap->lock();
					diffuseMap->setTextureFilter(xdl::XDEVL_TEXTURE_MAG_FILTER, xdl::XDEVL_LINEAR);
					diffuseMap->setTextureFilter(xdl::XDEVL_TEXTURE_MIN_FILTER, xdl::XDEVL_LINEAR_MIPMAP_LINEAR);
					diffuseMap->setTextureWrap(xdl::XDEVL_TEXTURE_WRAP_S, xdl::XDEVL_REPEAT);
					diffuseMap->setTextureWrap(xdl::XDEVL_TEXTURE_WRAP_T, xdl::XDEVL_REPEAT);
					diffuseMap->generateMipMap();
					diffuseMap->unlock();
					material->setTexture(Material::DIFFUSE_MAP, diffuseMap);

					material->setUseDiffuseMap(xdl::xdl_true);

					texIndex++;
					texFound = m_aiScene->mMaterials[aimesh->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
				}
				texIndex = 0;

				//
				// Import all specular textures.
				//
				aiReturn heightlFound = m_aiScene->mMaterials[aimesh->mMaterialIndex]->GetTexture(aiTextureType_SPECULAR, texIndex, &path);
				while(heightlFound == AI_SUCCESS) {
					// FIXME The import of the texture server can return nullptr. FIX THIS LINE HERE.
					material->setTexture(Material::SPECULAR_MAP, soan::TextureServer::Inst()->import(path.data));
					material->setUseSpecularMap(xdl::xdl_true);

					texIndex++;
					heightlFound = m_aiScene->mMaterials[aimesh->mMaterialIndex]->GetTexture(aiTextureType_SPECULAR, texIndex, &path);
				}
				texIndex = 0;

				//
				// Import all normal textures.
				//
				aiReturn normalFound = m_aiScene->mMaterials[aimesh->mMaterialIndex]->GetTexture(aiTextureType_HEIGHT , texIndex, &path);
				while(normalFound == AI_SUCCESS) {
					// FIXME The import of the texture server can return nullptr. FIX THIS LINE HERE.
					xdl::XdevLTexture* normalMap =soan::TextureServer::Inst()->import(path.data);
					normalMap->lock();
					normalMap->setTextureFilter(xdl::XDEVL_TEXTURE_MAG_FILTER, xdl::XDEVL_LINEAR);
					normalMap->setTextureFilter(xdl::XDEVL_TEXTURE_MIN_FILTER, xdl::XDEVL_LINEAR_MIPMAP_LINEAR);
					normalMap->setTextureWrap(xdl::XDEVL_TEXTURE_WRAP_S, xdl::XDEVL_REPEAT);
					normalMap->setTextureWrap(xdl::XDEVL_TEXTURE_WRAP_T, xdl::XDEVL_REPEAT);
					normalMap->generateMipMap();
					normalMap->unlock();
					material->setTexture(Material::NORMAL_MAP, normalMap);
					material->setUseNormalMap(xdl::xdl_true);

					texIndex++;
					normalFound = m_aiScene->mMaterials[aimesh->mMaterialIndex]->GetTexture(aiTextureType_HEIGHT , texIndex, &path);
				}

				texIndex = 0;

				//
				// Import all normal textures.
				//
				aiReturn displacementFound = m_aiScene->mMaterials[aimesh->mMaterialIndex]->GetTexture(aiTextureType_DISPLACEMENT, texIndex, &path);
				while(displacementFound == AI_SUCCESS) {
					// FIXME The import of the texture server can return nullptr. FIX THIS LINE HERE.
					xdl::XdevLTexture* displacementMap = soan::TextureServer::Inst()->import(path.data);
					displacementMap->lock();
					displacementMap->setTextureFilter(xdl::XDEVL_TEXTURE_MAG_FILTER, xdl::XDEVL_LINEAR);
					displacementMap->setTextureFilter(xdl::XDEVL_TEXTURE_MIN_FILTER, xdl::XDEVL_LINEAR_MIPMAP_LINEAR);
					displacementMap->generateMipMap();
					displacementMap->unlock();
					material->setTexture(Material::DISPLACEMENT_MAP, displacementMap);
					material->setUseDisplacementMap(xdl::xdl_true);

					texIndex++;
					displacementFound = m_aiScene->mMaterials[aimesh->mMaterialIndex]->GetTexture(aiTextureType_DISPLACEMENT, texIndex, &path);
				}


//								//
//				// Import all normal textures.
//				//
//				aiReturn heightFound = m_aiScene->mMaterials[aimesh->mMaterialIndex]->GetTexture(aiTextureType_HEIGHT, texIndex, &path);
//				while(heightFound == AI_SUCCESS) {
//					// FIXME The import of the texture server can return nullptr. FIX THIS LINE HERE.
//					xdl::XdevLTexture* displacementMap =soan::TextureServer::Inst()->import(path.data);
//					displacementMap->lock();
//					displacementMap->setTextureFilter(xdl::XDEVL_TEXTURE_MAG_FILTER, xdl::XDEVL_LINEAR);
//					displacementMap->setTextureFilter(xdl::XDEVL_TEXTURE_MIN_FILTER, xdl::XDEVL_LINEAR_MIPMAP_LINEAR);
//					displacementMap->generateMipMap();
//					displacementMap->unlock();
//					material->setTexture(Material::DISPLACEMENT_MAP, displacementMap);
//					material->setUseDisplacementMap(xdl::xdl_true);
//
//					texIndex++;
//					heightFound = m_aiScene->mMaterials[aimesh->mMaterialIndex]->GetTexture(aiTextureType_HEIGHT, texIndex, &path);
//				}


			}







			Mesh* mesh = new Mesh();
			mesh->setParent(model);
			mesh->setVertexArray(va);
			mesh->setMaterial(material);
			mesh->setNumberOfFaces(m_numFaces);
			mesh->setNumberOfVertices(m_numVertices);
			model->add(mesh);

			return 1;
		}


		void AssimpToModel::getBoundingBoxNode(const aiNode* nd,
		                                       aiVector3D* min,
		                                       aiVector3D* max,
		                                       aiMatrix4x4* trafo
		                                      ) {
			aiMatrix4x4 prev;
			unsigned int n = 0, t;

			prev = *trafo;
			aiMultiplyMatrix4(trafo,&nd->mTransformation);

			for(; n < nd->mNumMeshes; ++n) {
				const aiMesh* mesh = m_aiScene->mMeshes[nd->mMeshes[n]];
				for(t = 0; t < mesh->mNumVertices; ++t) {

					aiVector3D tmp = mesh->mVertices[t];
					aiTransformVecByMatrix4(&tmp,trafo);

					min->x = aisgl_min(min->x,tmp.x);
					min->y = aisgl_min(min->y,tmp.y);
					min->z = aisgl_min(min->z,tmp.z);

					max->x = aisgl_max(max->x,tmp.x);
					max->y = aisgl_max(max->y,tmp.y);
					max->z = aisgl_max(max->z,tmp.z);
				}
			}

			for(n = 0; n < nd->mNumChildren; ++n) {
				getBoundingBoxNode(nd->mChildren[n],min,max,trafo);
			}
			*trafo = prev;
		}


		void AssimpToModel::getBoundingBox() {
			aiMatrix4x4 trafo;
			aiIdentityMatrix4(&trafo);
			aiVector3D min, max;

			min.x = min.y = min.z =  1e10f;
			max.x = max.y = max.z = -1e10f;

			getBoundingBoxNode(m_aiScene->mRootNode, &min, &max, &trafo);

			m_boundingBoxMin.x = min.x;
			m_boundingBoxMin.y = min.y;
			m_boundingBoxMin.z = min.z;

			m_boundingBoxMax.x = max.x;
			m_boundingBoxMax.y = max.y;
			m_boundingBoxMax.z = max.z;


		}

	}

}
