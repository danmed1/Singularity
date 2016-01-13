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

#include "Mesh.h"

namespace soan {

	Mesh::Mesh() :
		m_numberOfFaces(0),
		m_numberOfVertices(0) {

	}

	Mesh::~Mesh() {
	}

	xdl::IPXdevLVertexArray Mesh::getVertexArray() {
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

	void  Mesh::setVertexArray(xdl::IPXdevLVertexArray vertexArray) {
		m_va = vertexArray;
	}

	xdl::xdl_uint Mesh::getNumberOfFaces() {
		return m_numberOfFaces;
	}

	xdl::xdl_uint Mesh::getNumberOfVertices() {
		return m_numberOfVertices;
	}
}
