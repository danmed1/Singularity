#include "Moveable.h"

namespace soan {

	Moveable::Moveable() : 	m_position(0.0f, 0.0f, 0.0f),
		m_scale(1.0f, 1.0f, 1.0f) {
		tmath::identity(m_orientation);
	}

	tmath::vec3 Moveable::getDirection() {
		tmath::vec3 tmp = m_orientation * tmath::vec3(0.0f, 0.0f, -1.0f);
		tmath::normalize(tmp);
		return tmp;
	}

	tmath::vec3 Moveable::getForwardVector() {
		tmath::vec4 tmp(getTransformationMatrix() * tmath::vec4(0.0f, 0.0f, -1.0f, 1.0f));
		return tmath::vec3(tmp);
	}

	tmath::vec3 Moveable::getRightVector() {
		tmath::vec4 tmp(getTransformationMatrix() * tmath::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		return tmath::vec3(tmp);
	}

	tmath::vec3 Moveable::getUpVector() {
		tmath::vec4 tmp(getTransformationMatrix() * tmath::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		return tmath::vec3(tmp);
	}

	tmath::vec3& Moveable::getPosition() {
		return m_position;
	}

	tmath::quat& Moveable::getOrientation() {
		return m_orientation;
	}

	tmath::quat Moveable::getInverseOrientation() {
		return tmath::inverse(m_orientation);
	}

	tmath::vec3&  Moveable::getScale() {
		return m_scale;
	}

	tmath::mat4 Moveable::getOrientationMatrix() {
		tmath::mat4 tmp;
		tmath::convert(m_orientation, tmp);
		return tmp;
	}

	tmath::mat4 Moveable::getTransformationMatrix() {
		tmath::mat4 rotation, trans, sc, tmp;

		tmath::translate(m_position, trans);
		tmath::scale(m_scale, sc);

		tmath::convert(m_orientation, rotation);

		return (trans * rotation * sc);

	}

	tmath::mat4 Moveable::getInverseTransformationMatrix() {

		// We have to calculate the inverse of the transformation.
		// The transformation                = Translation * Rotation*Scale
		// The inverse of the transformation = Scale^-1 * Rotation^-1 * Translation^-1

		tmath::mat4 rotation, trans, sc;
		tmath::quat qtmp;

		// Create inverse scale matrix;
		sc.xx = 1.0f/m_scale.x;
		sc.yy = 1.0f/m_scale.y;
		sc.zz = 1.0f/m_scale.z;
		sc.ww = 1.0f;

		// Create inverse rotation matrix.
		qtmp = tmath::inverse(m_orientation);
		tmath::convert(qtmp, rotation);

		// Create inverse translation matrix.
		tmath::translate(-m_position, trans);

		return (sc * rotation * trans);
	}

	void Moveable::setPosition(float x, float y, float z) {
		m_position.x = x;
		m_position.y = y;
		m_position.z = z;
	}

	void  Moveable::setPosition(const tmath::vec3& position) {
		m_position = position;
	}

	void Moveable::setScale(float sx, float sy, float sz) {
		m_scale.x = sx;
		m_scale.y = sy;
		m_scale.z = sz;
	}


	void Moveable::setScale(const tmath::vec3& scale) {
		m_scale = scale;
	}

	void Moveable::setOrientation(float x, float y, float z, float w) {
		m_orientation.x = x;
		m_orientation.y = y;
		m_orientation.z = z;
		m_orientation.w = w;
	}

	void  Moveable::setOrientation(const tmath::quat& orientation) {
		m_orientation = orientation;
	}

	void Moveable::moveRelative(float x, float y, float z) {
		m_position.x += x;
		m_position.y += y;
		m_position.z += z;
	}

	void Moveable::moveRelative(const tmath::vec3& distance) {
		m_position += distance;
	}

	void  Moveable::rotateLocalX(float angle) {

		// Create x axis vector in global space.
		tmath::vec3 tmp = m_orientation * tmath::vec3(1.0f, 0.0f, 0.0f);

		// Create rotation quaternion around that axis.
		tmath::quat rotation;
		tmath::rot(angle, tmp, 	rotation);

		// Rotate object.
		m_orientation = rotation * m_orientation;
		tmath::normalize(m_orientation);
	}

	void  Moveable::rotateLocalY(float angle) {

		// Create x axis vector in global space.
		tmath::vec3 tmp = m_orientation * tmath::vec3(0.0f, 1.0f, 0.0f);

		// Create rotation quaternion around that axis.
		tmath::quat rotation;
		tmath::rot(angle, tmp, 	rotation);

		// Rotate object.
		m_orientation = rotation*m_orientation;
		tmath::normalize(m_orientation);

	}


	void  Moveable::rotateLocalZ(float angle) {

		// Create x axis vector in global space.
		tmath::vec3 tmp = m_orientation * tmath::vec3(0.0f, 0.0f, 1.0f);

		// Create rotation quaternion around that axis.
		tmath::quat rotation;
		tmath::rot(angle, tmp, 	rotation);

		// Rotate object.
		m_orientation = rotation*m_orientation;
		tmath::normalize(m_orientation);
	}

}
