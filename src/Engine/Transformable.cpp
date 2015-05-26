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

#include "Transformable.h"

namespace soan {

	Transformable::Transformable() : 	m_position(0.0f, 0.0f, 0.0f),
		m_scale(1.0f, 1.0f, 1.0f) {
		tmath::identity(m_orientation);
	}

	tmath::vec3 Transformable::getDirection() {
		tmath::vec3 tmp = m_orientation * tmath::vec3(0.0f, 0.0f, -1.0f);
		tmath::normalize(tmp);
		return tmp;
	}

	tmath::vec3 Transformable::getForwardVector() {
		tmath::vec4 tmp(getTransformationMatrix() * tmath::vec4(0.0f, 0.0f, -1.0f, 1.0f));
		return tmath::vec3(tmp);
	}

	tmath::vec3 Transformable::getRightVector() {
		tmath::vec4 tmp(getTransformationMatrix() * tmath::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		return tmath::vec3(tmp);
	}

	tmath::vec3 Transformable::getUpVector() {
		tmath::vec4 tmp(getTransformationMatrix() * tmath::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		return tmath::vec3(tmp);
	}

	tmath::vec3& Transformable::getPosition() {
		return m_position;
	}

	tmath::quat& Transformable::getOrientation() {
		return m_orientation;
	}

	tmath::quat Transformable::getInverseOrientation() {
		return tmath::inverse(m_orientation);
	}

	tmath::vec3&  Transformable::getScale() {
		return m_scale;
	}

	tmath::mat4 Transformable::getOrientationMatrix() {
		tmath::mat4 tmp;
		tmath::convert(m_orientation, tmp);
		return tmp;
	}

	tmath::mat4 Transformable::getTransformationMatrix() {
		tmath::mat4 rotation, trans, sc, tmp;

		tmath::translate(m_position, trans);
		tmath::scale(m_scale, sc);

		tmath::convert(m_orientation, rotation);

		return (trans * rotation * sc);

	}

	tmath::mat4 Transformable::getInverseTransformationMatrix() {

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

	void Transformable::setPosition(xdl::xdl_float x, xdl::xdl_float y, xdl::xdl_float z) {
		m_position.x = x;
		m_position.y = y;
		m_position.z = z;
	}

	void  Transformable::setPosition(const tmath::vec3& position) {
		m_position = position;
	}

	void Transformable::setScale(xdl::xdl_float sx, xdl::xdl_float sy, xdl::xdl_float sz) {
		m_scale.x = sx;
		m_scale.y = sy;
		m_scale.z = sz;
	}


	void Transformable::setScale(const tmath::vec3& scale) {
		m_scale = scale;
	}

	void Transformable::setOrientation(xdl::xdl_float x, xdl::xdl_float y, xdl::xdl_float z, xdl::xdl_float w) {
		m_orientation.x = x;
		m_orientation.y = y;
		m_orientation.z = z;
		m_orientation.w = w;
	}

	void  Transformable::setOrientation(const tmath::quat& orientation) {
		m_orientation = orientation;
	}

	void Transformable::moveRelative(xdl::xdl_float x, xdl::xdl_float y, xdl::xdl_float z) {
		m_position.x += x;
		m_position.y += y;
		m_position.z += z;
	}

	void Transformable::moveRelative(const tmath::vec3& distance) {
		m_position += distance;
	}

	void  Transformable::rotateLocalX(xdl::xdl_float angle) {

		// Create x axis vector in global space.
		tmath::vec3 tmp = m_orientation * tmath::vec3(1.0f, 0.0f, 0.0f);

		// Create rotation quaternion around that axis.
		tmath::quat rotation;
		tmath::rotate(angle, tmp, 	rotation);

		// Rotate object.
		m_orientation = rotation * m_orientation;
		tmath::normalize(m_orientation);
	}

	void  Transformable::rotateLocalY(xdl::xdl_float angle) {

		// Create x axis vector in global space.
		tmath::vec3 tmp = m_orientation * tmath::vec3(0.0f, 1.0f, 0.0f);

		// Create rotation quaternion around that axis.
		tmath::quat rotation;
		tmath::rotate(angle, tmp, 	rotation);

		// Rotate object.
		m_orientation = rotation*m_orientation;
		tmath::normalize(m_orientation);

	}


	void  Transformable::rotateLocalZ(xdl::xdl_float angle) {

		// Create x axis vector in global space.
		tmath::vec3 tmp = m_orientation * tmath::vec3(0.0f, 0.0f, 1.0f);

		// Create rotation quaternion around that axis.
		tmath::quat rotation;
		tmath::rotate(angle, tmp, 	rotation);

		// Rotate object.
		m_orientation = rotation*m_orientation;
		tmath::normalize(m_orientation);
	}

}