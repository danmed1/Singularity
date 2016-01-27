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

#include <XdevLRAI/XdevLRAI.h>
#include "Model.h"

namespace soan {

	Model::Model() : m_openGL(nullptr) {

	}

	Model::Model(xdl::IPXdevLRAI openGL) : m_openGL(openGL) {

	}

	Model::~Model() {
//		for(auto& mesh : m_meshes) {
//			delete mesh;
//		}
	}

	std::shared_ptr<Model> Model::refCopy() {
		std::shared_ptr<Model> m (new Model(m_openGL));
	
		for(auto& mesh : m_meshes) {
			mesh->setMaterial(mesh->getMaterialRef());
			m->m_meshes.push_back(mesh);
		}

		m->m_boundingBoxMin = m_boundingBoxMin;
		m->m_boundingBoxMax	=	m_boundingBoxMax;

		return m;
	}

	void Model::add(std::shared_ptr<Mesh> mesh) {
		m_meshes.push_back(std::move(mesh));
	}

	Mesh* Model::getMesh(xdl::xdl_uint idx) {
		assert((m_meshes.size() > idx) && "Model::getMesh(): Specified index is biger than the number of meshes.");
		return m_meshes[idx].get();
	}

	std::vector<std::shared_ptr<Mesh>>& Model::getMeshList() {
		return m_meshes;
	}

	xdl::xdl_float  Model::getBoundingBoxX() {
		return(m_boundingBoxMax.x - m_boundingBoxMin.x);
	}

	xdl::xdl_float  Model::getBoundingBoxY() {
		return(m_boundingBoxMax.y - m_boundingBoxMin.y);
	}

	xdl::xdl_float  Model::getBoundingBoxZ() {
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
