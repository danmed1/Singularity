#include "Mesh.h"

namespace soan {

	Mesh::Mesh() :
		m_numberOfFaces(0),
		m_numberOfVertices(0) {

	}

	Mesh::~Mesh() {
	}

	xdl::XdevLVertexArray* Mesh::getVertexArray() {
		return m_va;
	}
	
	void Mesh::setParent(const std::shared_ptr<Model>&	model) {
		m_parentModel = model;
	}
	
	void Mesh::setMaterial(const std::shared_ptr<Material>& material) {
		m_material = material;
	}

	void  Mesh::setNumberOfFaces(xdl::xdl_uint numberOfFaces) {
		m_numberOfFaces = numberOfFaces;
	}

	void Mesh::setNumberOfVertices(xdl::xdl_uint numberOfVertices) {
		m_numberOfVertices = numberOfVertices;
	}

	const std::shared_ptr<Material>& Mesh::getMaterial() {
		return m_material;
	}

	const std::shared_ptr<Model>&	Mesh::getParentModel() {
		return m_parentModel;
	}

	void  Mesh::setVertexArray(xdl::XdevLVertexArray* vertexArray) {
		m_va = vertexArray;
	}

	xdl::xdl_uint Mesh::getNumberOfFaces() {
		return m_numberOfFaces;
	}

	xdl::xdl_uint Mesh::getNumberOfVertices() {
		return m_numberOfVertices;
	}
}
