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

#ifndef MESH_H
#define MESH_H

#include <XdevLRAI/XdevLVertexArray.h>
#include "Engine/Entity/Material.h"
#include "Engine/Transformable.h"
#include "Engine/Types.h"
#include <memory>

namespace soan {

	class Model;

	class Mesh {
		public:

			Mesh();

			virtual ~Mesh();

			//
			// Getters
			//

			/// Returns the material.
			Material* getMaterial();

			/// Returns the reference of the material.
			std::shared_ptr<Material> getMaterialRef();

			/// Returns the vertex array.
			xdl::IPXdevLVertexArray getVertexArray();

			/// Returns the reference of the vertex array.
			std::shared_ptr<xdl::XdevLVertexArray> getVertexArrayRef();

			/// Returns the parent model for this mesh.
			Model* getParentModel();

			/// Returns the reference of the parent model.
			std::shared_ptr<Model> getParentModelRef();

			/// Returns the number of faces.
			xdl::xdl_uint getNumberOfFaces();

			/// Returns the number of vertices.
			xdl::xdl_uint getNumberOfVertices();

			//
			// Setters
			//

			/// Set the material.
			void setMaterial(const std::shared_ptr<Material>& material);

			/// Sets the vertex array.
			void setVertexArray(std::shared_ptr<xdl::XdevLVertexArray> vertexArray);

			/// Sets the parent model.
			void setParentModel(const std::shared_ptr<Model>& model);

			/// Sets the number of faces.
			void setNumberOfFaces(xdl::xdl_uint numberOfFaces);

			/// Sets the number of vertices.
			void setNumberOfVertices(xdl::xdl_uint numberOfVertices);

		private:
			/// Holds the parent model.
			std::shared_ptr<Model> m_parentModel;

			/// Holds the vertex array object.
			std::shared_ptr<xdl::XdevLVertexArray> m_va;

			/// Holds the material for this object.
			std::shared_ptr<Material> m_material;

			/// Holds the number of faces.
			xdl::xdl_uint m_numberOfFaces;

			/// Holds the number of vertices.
			xdl::xdl_uint m_numberOfVertices;

	};

}

#endif
