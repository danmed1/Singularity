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
			xdl::XdevLVertexArray* getVertexArray();

			/// Returns the reference of the vertex array.
			xdl::IPXdevLVertexArray getVertexArrayRef();

			/// Returns the parent model for this mesh.
			Model* getParentModel();

			/// Returns the reference of the parent model.
			std::shared_ptr<Model> getParentModelRef();

			/// Returns the number of faces.
			xdl::xdl_uint getNumberOfFaces();

			/// Returns the number of vertices.
			xdl::xdl_uint getNumberOfVertices();

			/// Returns the primitive used for this mesh.
			xdl::XdevLPrimitiveType getPrimitive();

			//
			// Setters
			//

			/// Set the material.
			void setMaterial(const std::shared_ptr<Material>& material);

			/// Sets the vertex array.
			void setVertexArray(xdl::IPXdevLVertexArray vertexArray);

			/// Sets the parent model.
			void setParentModel(const std::shared_ptr<Model>& model);

			/// Sets the number of faces.
			void setNumberOfFaces(xdl::xdl_uint numberOfFaces);

			/// Sets the number of vertices.
			void setNumberOfVertices(xdl::xdl_uint numberOfVertices);

			/// Sets the primitive type of this mesh.
			void setPrimitive(xdl::XdevLPrimitiveType primitive);

		private:

			/// Holds the parent model.
			std::shared_ptr<Model> m_parentModel;

			/// Holds the vertex array object.
			xdl::IPXdevLVertexArray m_va;
			
			/// Holds the index buffer of the mesh.
			xdl::IPXdevLIndexBuffer m_indexBuffer;

			/// Holds the material for this object.
			std::shared_ptr<Material> m_material;

			/// Holds the number of faces.
			xdl::xdl_uint m_numberOfFaces;

			/// Holds the number of vertices.
			xdl::xdl_uint m_numberOfVertices;

			/// Holds the primitive type.
			xdl::XdevLPrimitiveType m_primitive;
	};

}

#endif
