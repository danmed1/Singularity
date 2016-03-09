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

#ifndef MODEL_H
#define MODEL_H

#include "Engine/Transformable.h"
#include "Engine/Entity/Mesh.h"
#include <memory>

namespace soan {

	class Camera;

	/**
		@class Model
		@brief A 3D Model.

		A model may have one or more meshes at the same time. The main purpose of this
		class is to provide a mechanism that allows connecting different types of mesh
		into one object.
	*/
	class Model {
		public:
			Model();

			Model(xdl::IPXdevLRAI openGL);

			~Model();

			/// Add a new mesh to the model.
			void add(std::shared_ptr<Mesh> mesh);

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
			void setBoundingBox(const tmath::vec3& min, const tmath::vec3& max);

			/// Returns the number of vertices of all meshes together.
			xdl::xdl_uint getNumberOfVertices();

			/// Returns the number of faces of all meshes together.
			xdl::xdl_uint getNumberOfFaces();

		private:

			xdl::IPXdevLRAI						m_openGL;
			std::vector<std::shared_ptr<Mesh>>	m_meshes;

			tmath::vec3							m_boundingBoxMin;
			tmath::vec3							m_boundingBoxMax;
	};

}


#endif
