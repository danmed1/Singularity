#ifndef MODEL_H
#define MODEL_H

#include "Moveable.h"
#include "Mesh.h"
#include <memory>

namespace soan {

	class Camera;

	/**
		@class Model
		@brief A 3D Model.

		A model may have more then one mesh. The main purpose of this class is to provide a mechanism
		that allows connecting different types of mesh into one object.
	*/
	class Model : public Moveable {
	public:
		Model();
		
		Model(xdl::XdevLOpenGL330* openGL);
		
		~Model();
		
		void add(Mesh* mesh);
		
		/// Returns one specific mesh from the mesh list.
		Mesh* getMesh(xdl::xdl_uint idx);
		
		/// Returns a list of all meshes attached to this model.
		std::vector<std::shared_ptr<Mesh>>& getMeshList();
				
		/// Returns a referenced copy of the mesh.
		/**
			A referenced copy is a not a full copy. The mesh of this model will
			be shared.
		*/
		std::shared_ptr<Model> refCopy();
		
		/// Returns the length of the bounding box in x-direction.		
		xdl::xdl_float getBoundingBoxX();

		/// Returns the length of the bounding box in y-direction.
		xdl::xdl_float getBoundingBoxY();
		
		/// Returns the length of the bounding box in z-direction.
		xdl::xdl_float getBoundingBoxZ();		
		
		/// sets the bounding box.
		void setBoundingBox(tmath::vec3& min, tmath::vec3& max);

		/// Returns the number of vertices of all meshes together.
		xdl::xdl_uint getNumberOfVertices();
		
		/// Returns the number of faces of all meshes together.
		xdl::xdl_uint getNumberOfFaces();
		
	private:

		xdl::XdevLOpenGL330* 								m_openGL;
		std::vector<std::shared_ptr<Mesh>>	m_meshes;
		
		tmath::vec3							m_boundingBoxMin;
		tmath::vec3							m_boundingBoxMax;
	};

}


#endif
