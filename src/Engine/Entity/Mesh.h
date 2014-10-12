#ifndef MESH_H
#define MESH_H

#include <XdevLOpenGL/XdevLVertexArray.h>
#include "Engine/Entity/Material.h"
#include "Engine/Moveable.h"
#include "Engine/Types.h"
#include <memory>

namespace soan {

	class Model;

	class Mesh : public Moveable {
		public:

			Mesh();

			virtual ~Mesh();

			/// Set the material.
			void setMaterial(const std::shared_ptr<Material>& material);

			/// Sets the number of faces.
			void setNumberOfFaces(xdl::xdl_uint numberOfFaces);

			/// Sets the number of vertices.
			void setNumberOfVertices(xdl::xdl_uint numberOfVertices);

			/// Return the material.
			const std::shared_ptr<Material>& getMaterial();

			/// Return the vertex array.
			xdl::XdevLVertexArray* getVertexArray();

			/// Returns the parent model for this mesh.
			const std::shared_ptr<Model>&	getParentModel();

			/// Sets the vertex array.
			void setVertexArray(xdl::XdevLVertexArray* vertexArray);
			
			/// Sets the parent model.
			void setParent(const std::shared_ptr<Model>&	model);
			
			/// Returns the number of faces.
			xdl::xdl_uint getNumberOfFaces();

			/// Returns the number of vertices.
			xdl::xdl_uint getNumberOfVertices();

		private:
			/// Holds the parent model.
			std::shared_ptr<Model>		m_parentModel;

			/// Holds the vertex array object.
			xdl::XdevLVertexArray*		m_va;

			/// Holds the material for this object.
			std::shared_ptr<Material>	m_material;

			/// Holds the number of faces.
			xdl::xdl_uint 						m_numberOfFaces;

			/// Holds the number of vertices.
			xdl::xdl_uint							m_numberOfVertices;

	};

}

#endif
