#include "Frustum.h"

namespace soan {

	Frustum::Frustum() {

	}

	Frustum::~Frustum() {

	}

	tmath::matrix<float, 4, 6>& Frustum::getFrustumMatrix() {
		return m_frustumMatrix;
	}
	
	tmath::vec3& Frustum::getPoints(xdl::xdl_uint idx) {
		return m_points[idx];
	}

	void Frustum::update(Camera* camera, xdl::xdl_bool normalize) {

		tmath::mat4 projViewMatrix = camera->getTransformationMatrix() * camera->getProjectionMatrix() ;

		m_points[0] =  tmath::vec3();
		m_points[1] =  tmath::vec3(projViewMatrix.row(0)*3);
		m_points[2] =  tmath::vec3(projViewMatrix.row(1)*3);
		m_points[3] =  tmath::vec3(projViewMatrix.row(2)*3);
		

		//
		// Right Plane.
		//
		m_frustumMatrix.at(RIGHT_FRUSTUM_PLANE, 0) = projViewMatrix.xw - projViewMatrix.xx;
		m_frustumMatrix.at(RIGHT_FRUSTUM_PLANE, 1) = projViewMatrix.yw - projViewMatrix.yx;
		m_frustumMatrix.at(RIGHT_FRUSTUM_PLANE, 2) = projViewMatrix.zw - projViewMatrix.zx;
		m_frustumMatrix.at(RIGHT_FRUSTUM_PLANE, 3) = projViewMatrix.ww - projViewMatrix.wx;

		//
		// Left Plane.
		//
		m_frustumMatrix.at(LEFT_FRUSTUM_PLANE, 0) = projViewMatrix.xw + projViewMatrix.xx;
		m_frustumMatrix.at(LEFT_FRUSTUM_PLANE, 1) = projViewMatrix.yw + projViewMatrix.yx;
		m_frustumMatrix.at(LEFT_FRUSTUM_PLANE, 2) = projViewMatrix.zw + projViewMatrix.zx;
		m_frustumMatrix.at(LEFT_FRUSTUM_PLANE, 3) = projViewMatrix.ww + projViewMatrix.wx;

		//
		// Bottom Plane.
		//
		m_frustumMatrix.at(BOTTOM_FRUSTUM_PLANE, 0) = projViewMatrix.xw + projViewMatrix.xy;
		m_frustumMatrix.at(BOTTOM_FRUSTUM_PLANE, 1) = projViewMatrix.yw + projViewMatrix.yy;
		m_frustumMatrix.at(BOTTOM_FRUSTUM_PLANE, 2) = projViewMatrix.zw + projViewMatrix.zy;
		m_frustumMatrix.at(BOTTOM_FRUSTUM_PLANE, 3) = projViewMatrix.ww + projViewMatrix.wy;

		//
		// Top Plane.
		//
		m_frustumMatrix.at(TOP_FRUSTUM_PLANE, 0) = projViewMatrix.xw - projViewMatrix.xy;
		m_frustumMatrix.at(TOP_FRUSTUM_PLANE, 1) = projViewMatrix.yw - projViewMatrix.yy;
		m_frustumMatrix.at(TOP_FRUSTUM_PLANE, 2) = projViewMatrix.zw - projViewMatrix.zy;
		m_frustumMatrix.at(TOP_FRUSTUM_PLANE, 3) = projViewMatrix.ww - projViewMatrix.wy;

		//
		// Near Plane.
		//
		m_frustumMatrix.at(NEAR_FRUSTUM_PLANE, 0) = projViewMatrix.xw + projViewMatrix.xz;
		m_frustumMatrix.at(NEAR_FRUSTUM_PLANE, 1) = projViewMatrix.yw + projViewMatrix.yz;
		m_frustumMatrix.at(NEAR_FRUSTUM_PLANE, 2) = projViewMatrix.zw + projViewMatrix.zz;
		m_frustumMatrix.at(NEAR_FRUSTUM_PLANE, 3) = projViewMatrix.ww + projViewMatrix.wz;

		//
		// Far Plane.
		//
		m_frustumMatrix.at(FAR_FRUSTUM_PLANE, 0) = projViewMatrix.xw - projViewMatrix.xz;
		m_frustumMatrix.at(FAR_FRUSTUM_PLANE, 1) = projViewMatrix.yw - projViewMatrix.yz;
		m_frustumMatrix.at(FAR_FRUSTUM_PLANE, 2) = projViewMatrix.zw - projViewMatrix.zz;
		m_frustumMatrix.at(FAR_FRUSTUM_PLANE, 3) = projViewMatrix.ww - projViewMatrix.wz;

		if(normalize) {
			for(xdl::xdl_uint a = 0; a < 6; ++a) {
				xdl::xdl_float t = 1.0/sqrt(m_frustumMatrix.at(a, 0) * m_frustumMatrix.at(a, 0) +
				                            m_frustumMatrix.at(a, 1) * m_frustumMatrix.at(a, 1) +
				                            m_frustumMatrix.at(a, 2) * m_frustumMatrix.at(a, 2));

				m_frustumMatrix.at(a, 0) *= t;
				m_frustumMatrix.at(a, 1) *= t;
				m_frustumMatrix.at(a, 2) *= t;
				m_frustumMatrix.at(a, 3) *= t;
			}
		}
//		std::cout << 	m_frustumMatrix.at(FAR_FRUSTUM_PLANE, 0)	<< "," <<
//		          m_frustumMatrix.at(FAR_FRUSTUM_PLANE, 1) 	<< "," <<
//		          m_frustumMatrix.at(FAR_FRUSTUM_PLANE, 2) 	<< "," <<
//		          m_frustumMatrix.at(FAR_FRUSTUM_PLANE, 3) 	<< std::endl;
	}

	xdl::xdl_bool Frustum::isPointInside(xdl::xdl_float x, xdl::xdl_float y, xdl::xdl_float z) {
		for(xdl::xdl_int p = 0; p < 6; p++)
			if(m_frustumMatrix.at(p, 0) * x + m_frustumMatrix.at(p, 1) * y + m_frustumMatrix.at(p, 2) * z + m_frustumMatrix.at(p, 3) <= 0)
				return false;
		return true;
	}

	xdl::xdl_bool Frustum::isPointInside(const tmath::vec3& point) {
		return isPointInside(point.x, point.z, point.z);
	}

	xdl::xdl_bool Frustum::isSphereInside(xdl::xdl_float x, xdl::xdl_float y, xdl::xdl_float z, xdl::xdl_float radius) {
		for(xdl::xdl_int p = 0; p < 6; p++)
			if(m_frustumMatrix.at(p, 0) * x + m_frustumMatrix.at(p, 1) * y + m_frustumMatrix.at(p, 2) * z + m_frustumMatrix.at(p, 3) <= -radius)
				return false;
		return true;
	}

	xdl::xdl_bool Frustum::isSphereInside(const tmath::vec3& point, xdl::xdl_float radius) {
		return isSphereInside(point.x, point.y, point.z, radius);
	}

	xdl::xdl_bool Frustum::isCubeInside(xdl::xdl_float x, xdl::xdl_float y, xdl::xdl_float z, xdl::xdl_float size) {

		for(xdl::xdl_uint p = 0; p < 6; p++) {
			if(m_frustumMatrix.at(p,0) * (x - size) + m_frustumMatrix.at(p,1) * (y - size) + m_frustumMatrix.at(p,2) * (z - size) + m_frustumMatrix.at(p,3) > 0)
				continue;
			if(m_frustumMatrix.at(p,0) * (x + size) + m_frustumMatrix.at(p,1) * (y - size) + m_frustumMatrix.at(p,2) * (z - size) + m_frustumMatrix.at(p,3) > 0)
				continue;
			if(m_frustumMatrix.at(p,0) * (x - size) + m_frustumMatrix.at(p,1) * (y + size) + m_frustumMatrix.at(p,2) * (z - size) + m_frustumMatrix.at(p,3) > 0)
				continue;
			if(m_frustumMatrix.at(p,0) * (x + size) + m_frustumMatrix.at(p,1) * (y + size) + m_frustumMatrix.at(p,2) * (z - size) + m_frustumMatrix.at(p,3) > 0)
				continue;
			if(m_frustumMatrix.at(p,0) * (x - size) + m_frustumMatrix.at(p,1) * (y - size) + m_frustumMatrix.at(p,2) * (z + size) + m_frustumMatrix.at(p,3) > 0)
				continue;
			if(m_frustumMatrix.at(p,0) * (x + size) + m_frustumMatrix.at(p,1) * (y - size) + m_frustumMatrix.at(p,2) * (z + size) + m_frustumMatrix.at(p,3) > 0)
				continue;
			if(m_frustumMatrix.at(p,0) * (x - size) + m_frustumMatrix.at(p,1) * (y + size) + m_frustumMatrix.at(p,2) * (z + size) + m_frustumMatrix.at(p,3) > 0)
				continue;
			if(m_frustumMatrix.at(p,0) * (x + size) + m_frustumMatrix.at(p,1) * (y + size) + m_frustumMatrix.at(p,2) * (z + size) + m_frustumMatrix.at(p,3) > 0)
				continue;
			return false;
		}
		return true;
	}
	
	xdl::xdl_bool Frustum::isCubeInside(const tmath::vec3& point, xdl::xdl_float size) {
		return isCubeInside(point.x, point.y, point.z, size);
	}


}
