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
