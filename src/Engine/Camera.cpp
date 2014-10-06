#include "Camera.h"

namespace soan {


	Camera::Camera() : 	m_type(PERSPECTIVE),
		m_trackedObject(nullptr),
		m_pitch(0.0f),
		m_heading(0.0f),
		m_forwardSpeed(10.0f),
		m_sideSpeed(2.0f),
		m_upSpeed(10.0f),
		m_slerpSpeed(10.0f) {

		setProjection(m_type);

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
			tmath::frustum(-0.35f, 0.35f, -0.25f, 0.25f, 0.5f, 4000.0f, m_projection);
		} else if(m_type == ORTHO) {
			tmath::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 20.0f, m_projection);
		}
	}

	tmath::mat4& Camera::getProjectionMatrix() {
		return m_projection;
	}

	void Camera::fpsView(float pitch, float yaw, float dT) {
		if(m_trackedObject != nullptr) {
			return;
		}

		m_pitch 	+= pitch;
		m_heading += yaw;

		tmath::quat q1,q2;
		q1 = m_orientation;
		tmath::e2q(m_heading, m_pitch, q2);

		tmath::slerp(q1, q2, m_slerpSpeed*dT, m_orientation);
	}

	void Camera::lookAt(tmath::vec3 target, tmath::vec3 up) {
		if(m_trackedObject != nullptr) {
			return;
		}
		tmath::mat4 tmp = tmath::look_at(m_position, target, up);
		tmath::convert(tmp, m_orientation);
	}

	void Camera::moveForward(float value) {
		if(m_trackedObject != nullptr) {
			return;
		}
		m_position += m_orientation * tmath::vec3(0.0f, 0.0f, -m_forwardSpeed)*value;
	}

	void Camera::moveSide(float value) {
		if(m_trackedObject != nullptr) {
			return;
		}
		m_position += m_orientation * tmath::vec3(m_sideSpeed, 0.0f, 0.0f)*value;
	}

	void Camera::moveUp(float value) {
		if(m_trackedObject != nullptr) {
			return;
		}
		m_position +=  m_orientation * tmath::vec3(0.0f, m_upSpeed, 0.0f)*value;
	}

	void Camera::doRoll(float value) {
		if(m_trackedObject != nullptr) {
			return;
		}
		rotateLocalZ(value);
	}

	void Camera::doPitch(float value) {
		if(m_trackedObject != nullptr) {
			return;
		}
		rotateLocalX(value);
	}

	void Camera::doYaw(float value) {
		if(m_trackedObject != nullptr) {
			return;
		}
		rotateLocalY(value);
	}

	void Camera::setForwardSpeed(float forwardSpeed) {
		m_forwardSpeed = forwardSpeed;
	}

	void Camera::setSideSpeed(float sideSpeed) {
		m_sideSpeed = sideSpeed;
	}

	void Camera::setUpSpeed(float upSpeed) {
		m_upSpeed = upSpeed;
	}

	void Camera::startTrackObject(const std::shared_ptr<Moveable>& moveable) {
		m_trackedObject = moveable;
	}


	void Camera::stopTrackObject() {
		m_trackedObject = nullptr;

		tmath::q2e(m_orientation, m_pitch, m_heading, m_roll);
	}

	void  Camera::setSLERPSpeed(float slerp) {
		m_slerpSpeed = slerp;
	}

	bool Camera::isTrackingModeActive() {
		if(m_trackedObject != nullptr) {
			return true;
		}

		return false;
	}

	void Camera::setTrackingProperties(float heading, float pitch, float distance) {
		if(m_trackedObject == nullptr) {
			return;
		}

		//
		// This rotation represents an offset rotation for the camera around the object.
		//
		tmath::quat qtmp;
		tmath::e2q(tmath::d2r(heading), tmath::d2r(pitch), qtmp);

		//
		// To get a smooth rotation from the previous to the new orientation we use slerp.
		//
		tmath::quat q1,q2,qresult;

		// Get previous orientation.
		q1 = m_orientation;

		// Get target orientation.
		q2 = m_trackedObject->getOrientation() * qtmp;

		// Do slerp.
		tmath::slerp(q1, q2, 0.04f, qresult);

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
