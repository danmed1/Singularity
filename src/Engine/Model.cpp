#include <XdevLOpenGL/XdevLOpenGL.h>
#include "Model.h"

namespace soan {

	Model::Model() : m_openGL(nullptr) {

	}

	Model::Model(xdl::XdevLOpenGL330* openGL) : m_openGL(openGL) {

	}

	Model::~Model() {
//		for(auto& mesh : m_meshes) {
//			delete mesh;
//		}
	}

	std::shared_ptr<Model> Model::refCopy() {
		std::shared_ptr<Model> m (new Model(m_openGL));
	
		for(auto& mesh : m_meshes) {
			std::shared_ptr<Material> material(mesh->getMaterial()->refCopy());
			std::cout << "Material: " << material.get() << std::endl;

			mesh->setMaterial(material);
			m->m_meshes.push_back(mesh);
		}

		m->m_boundingBoxMin = m_boundingBoxMin;
		m->m_boundingBoxMax	=	m_boundingBoxMax;

		return m;
	}

	void Model::add(Mesh* mesh) {
		assert(mesh && "Model::addMesh(): You can not add a nullptr mesh.");
		m_meshes.push_back(std::shared_ptr<Mesh>(mesh));
	}

	Mesh* Model::getMesh(unsigned int idx) {
		assert((m_meshes.size() > idx) && "Model::getMesh(): Specified index is biger than the number of meshes.");
		return m_meshes[idx].get();
	}

	std::vector<std::shared_ptr<Mesh>>& Model::getMeshList() {
		return m_meshes;
	}

	float  Model::getBoundingBoxX() {
		return(m_boundingBoxMax.x - m_boundingBoxMin.x);
	}

	float  Model::getBoundingBoxY() {
		return(m_boundingBoxMax.y - m_boundingBoxMin.y);
	}

	float  Model::getBoundingBoxZ() {
		return(m_boundingBoxMax.z - m_boundingBoxMin.z);
	}

	void  Model::setBoundingBox(tmath::vec3& min, tmath::vec3& max) {
		m_boundingBoxMin = min;
		m_boundingBoxMax = max;
	}

	xdl::xdl_uint Model::getNumberOfVertices() {
		xdl::xdl_uint tmp = 0;
		for(auto& mesh : m_meshes) {
			tmp += mesh->getNumberOfVertices();
		}
		return tmp;
	}

	xdl::xdl_uint Model::getNumberOfFaces() {
		xdl::xdl_uint tmp = 0;
		for(auto& mesh : m_meshes) {
			tmp += mesh->getNumberOfFaces();
		}
		return tmp;
	}
}
