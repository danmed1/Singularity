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

#ifndef FRUSTUM_H
#define FRUSTUM_H

#include <XdevLTypes.h>
#include <Engine/Camera.h>
#include <Utils/SingularityDebugDrawer.h>
#include <tm/tm.h>

namespace soan {

	class Frustum {
		public:

			enum FrustumPlanes {
			  LEFT_FRUSTUM_PLANE,
			  RIGHT_FRUSTUM_PLANE,
			  TOP_FRUSTUM_PLANE,
			  BOTTOM_FRUSTUM_PLANE,
			  NEAR_FRUSTUM_PLANE,
			  FAR_FRUSTUM_PLANE
			};

			Frustum();

			~Frustum();
			
			/// Returns the frustum matrix.
			tmath::matrix<float, 4, 6>& getFrustumMatrix();
			
			/// Updates the frustum matrix.
			void update(Camera* camera, xdl::xdl_bool normalize);
			
			/// Checks if a point is inside the frustum.
			xdl::xdl_bool isPointInside(xdl::xdl_float x, xdl::xdl_float y, xdl::xdl_float z);
			
			/// Checks if a point is inside the frustum.
			xdl::xdl_bool isPointInside(const tmath::vec3& point);
			
			/// Checks if a sphere is inside the frustum.
			xdl::xdl_bool isSphereInside(xdl::xdl_float x, xdl::xdl_float y, xdl::xdl_float z, xdl::xdl_float radius);
			
			/// Checks if a sphere is inside the frustum.
			xdl::xdl_bool isSphereInside(const tmath::vec3& point, xdl::xdl_float radius);
			
			/// Checks if a bounding box inside the frustum.
			xdl::xdl_bool isCubeInside(xdl::xdl_float x, xdl::xdl_float y, xdl::xdl_float z, xdl::xdl_float size);

			/// Checks if a bounding box is inside the frustum.
			xdl::xdl_bool isCubeInside(const tmath::vec3& point, xdl::xdl_float size);
			
			/// Returns the edge points of the frustum.
			tmath::vec3& getPoints(xdl::xdl_uint idx);
		private:

			tmath::matrix<float, 4, 6> m_frustumMatrix;
			tmath::vec3 m_points[8];
			
	};

}

#endif // FRUSTUM_H
