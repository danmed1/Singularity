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

#include "Camera.h"

namespace soan {


	Camera::Camera() : 	m_type(PERSPECTIVE),
		m_trackedObject(nullptr),
		m_pitch(0.0f),
		m_heading(0.0f),
		m_forwardSpeed(10.0f),
		m_sideSpeed(10.0f),
		m_upSpeed(10.0f),
		m_slerpSpeed(10.0f) {

		setProjection(m_type);

	}

	Camera::~Camera() {
	}

	unsigned int Camera::getViewPortWidth() {
		return m_viewPortWidth;
	}

	unsigned int Camera::getViewPortHeight() {
		return m_viewPortHeight;
	}

	void  Camera::setViewPort(unsigned int width, unsigned int height) {
		m_viewPortHeight 	= height;
		m_viewPortWidth 	= width;
	}

	void Camera::setProjection(CameraProjectionType type) {
		m_type = type;
		if(m_type == PERSPECTIVE) {
			tmath::frustum(-0.35f, 0.35f, -0.25f, 0.25f, 0.5f, 10000.0f, m_projection);
		} else if(m_type == ORTHO) {
			tmath::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 20.0f, m_projection);
		}
	}

	tmath::mat4& Camera::getProjectionMatrix() {
		return m_projection;
	}

	tmath::mat4  Camera::getProjectionsViewMatrix() {
		return (m_projection * getTransformationMatrix());
	}

	void Camera::fpsView(xdl::xdl_float pitch, xdl::xdl_float yaw, xdl::xdl_float dT) {
		if(m_trackedObject != nullptr) {
			return;
		}

		m_pitch 	+= pitch;
		m_heading += yaw;

		tmath::quat q1,q2;
		q1 = m_orientation;
		tmath::euler_to_quaternion(m_heading, m_pitch, q2);

		tmath::slerp(q1, q2, m_slerpSpeed*dT, m_orientation);
	}

	void Camera::lookAt(tmath::vec3 target, tmath::vec3 up) {
		if(m_trackedObject != nullptr) {
			return;
		}
		tmath::mat4 tmp;
		tmp = tmath::look_at(m_position, target, up, tmp);
		
		tmath::convert(tmp, m_orientation);
	}

	void Camera::moveForward(xdl::xdl_float value) {
		if(m_trackedObject != nullptr) {
			return;
		}
		m_position += m_orientation * tmath::vec3(0.0f, 0.0f, -m_forwardSpeed)*value;
	}

	void Camera::moveSide(xdl::xdl_float value) {
		if(m_trackedObject != nullptr) {
			return;
		}
		m_position += m_orientation * tmath::vec3(m_sideSpeed, 0.0f, 0.0f)*value;
	}

	void Camera::moveUp(xdl::xdl_float value) {
		if(m_trackedObject != nullptr) {
			return;
		}
		m_position +=  m_orientation * tmath::vec3(0.0f, m_upSpeed, 0.0f)*value;
	}

	void Camera::doRoll(xdl::xdl_float value) {
		if(m_trackedObject != nullptr) {
			return;
		}
		rotateLocalZ(value);
	}

	void Camera::doPitch(xdl::xdl_float value) {
		if(m_trackedObject != nullptr) {
			return;
		}
		rotateLocalX(value);
	}

	void Camera::doYaw(xdl::xdl_float value) {
		if(m_trackedObject != nullptr) {
			return;
		}
		rotateLocalY(value);
	}

	void Camera::setForwardSpeed(xdl::xdl_float forwardSpeed) {
		m_forwardSpeed = forwardSpeed;
	}

	void Camera::setSideSpeed(xdl::xdl_float sideSpeed) {
		m_sideSpeed = sideSpeed;
	}

	void Camera::setUpSpeed(xdl::xdl_float upSpeed) {
		m_upSpeed = upSpeed;
	}

	void Camera::startTrackObject(const std::shared_ptr<Moveable>& moveable) {
		m_trackedObject = moveable;
	}


	void Camera::stopTrackObject() {
		m_trackedObject = nullptr;

		tmath::quaternion_to_euler(m_orientation, m_pitch, m_heading, m_roll);
	}

	void  Camera::setSLERPSpeed(xdl::xdl_float slerp) {
		m_slerpSpeed = slerp;
	}

	bool Camera::isTrackingModeActive() {
		if(m_trackedObject != nullptr) {
			return true;
		}

		return false;
	}

	void Camera::setTrackingProperties(xdl::xdl_float heading, xdl::xdl_float pitch, xdl::xdl_float distance, xdl::xdl_float dT) {
		if(m_trackedObject == nullptr) {
			return;
		}

		//
		// This rotation represents an offset rotation for the camera around the object.
		//
		tmath::quat qtmp;
		tmath::euler_to_quaternion(tmath::d2r(heading), tmath::d2r(pitch), qtmp);

		//
		// To get a smooth rotation from the previous to the new orientation we use slerp.
		//
		tmath::quat q1,q2,qresult;

		// Get previous orientation.
		q1 = m_orientation;

		// Get target orientation.
		q2 = m_trackedObject->getOrientation() * qtmp;

		// Do slerp.
		tmath::slerp(q1, q2, m_slerpSpeed*dT, qresult);

		// This is the new position for the camera which is around the tracked object.
		// The vector (0.0f, 0.0f, distance) is in the tracked object space the back position.
		// We rotate that vector by the offset rotation. The multiplication with qresult will
		// transform the tracked object local position into world coordinate.
		// That plus the position of the tracked object is the new position for the camera.
		tmath::vec3 tmp = m_trackedObject->getPosition() + qresult * qtmp * tmath::vec3(0.0f, 0.0f, distance);


		setOrientation(qresult);
		setPosition(tmp);
	}

}
