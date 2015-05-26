
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

#include "Game/Actor.h"

namespace soan {

	namespace game {

		Actor::Actor() :
			m_name("Not Specified"),
			m_lifeTime(0),
			m_livedTime(0),
			m_physics(nullptr),
			m_body(nullptr),
			m_motionState(nullptr),
			m_colShape(nullptr),
			m_mass(1.0f),
			m_physicsEnabled(xdl::xdl_no) {

		}

		Actor::~Actor() {
			if(isPhysicsEnabled()) {
				enablePhysics(xdl::xdl_no);
			}
		}


		std::string& Actor::getName() {
			return m_name;
		}


		void Actor::setName(const std::string& name) {
			m_name = name;
		}


		xdl::xdl_uint64	 Actor::getLifeTime() {
			return m_lifeTime;
		}


		void Actor::setLifeTime(xdl::xdl_uint64	 lifeTime) {
			m_lifeTime = lifeTime;
		}

		xdl::xdl_uint64 Actor::getLivedTime() const {
			return m_livedTime;
		}

		void  Actor::increaseLivedTime(xdl::xdl_uint64 timeStep) {
			m_livedTime += timeStep;
		}

		bool Actor::update(xdl::xdl_uint64 timeStep) {

			//
			// Only activate lifetime if enabled.
			//
			if(m_lifeTime != 0) {

				increaseLivedTime(timeStep);

				/// Actor must die.
				if(m_livedTime >= m_lifeTime) {
					return true;
				}
			}

			if(m_physics != nullptr) {
				if(m_body != nullptr) {
					btMotionState* ms = m_body->getMotionState();
					btTransform ts;
					ms->getWorldTransform(ts);
					btQuaternion 	qr 	= ts.getRotation();
					btVector3 		pos = ts.getOrigin();
					setOrientation(qr.x(), qr.y(), qr.z(), qr.w());
					setPosition(pos.x(), pos.y(), pos.z());
				}
			}

			return Renderable::update(timeStep);

		}
		
		xdl::xdl_bool Actor::isPhysicsEnabled() const {
			return m_physicsEnabled;
		}

		void Actor::enablePhysics(xdl::xdl_bool enableIt) {
			m_physicsEnabled = enableIt;
			
			if(m_physicsEnabled == xdl::xdl_yes) {
				if(m_physics != nullptr) {

					btTransform startTransform;
					startTransform.setIdentity();


					startTransform.setOrigin(btVector3(getPosition().x,getPosition().y,getPosition().z));


					m_colShape = new btBoxShape(btVector3(getModel()->getBoundingBoxX()/2.0, getModel()->getBoundingBoxY()/2.0f, getModel()->getBoundingBoxZ()/2.0f));
					btVector3 localInertia(0,0,0);

					m_colShape->calculateLocalInertia(m_mass, localInertia);


					//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
					m_motionState = new btDefaultMotionState(startTransform);
					btRigidBody::btRigidBodyConstructionInfo rbInfo(m_mass, m_motionState, m_colShape, localInertia);
					m_body = new btRigidBody(rbInfo);

					m_body->setActivationState(DISABLE_DEACTIVATION);
					m_physics->addRigidBody(m_body);
				}
			} else {
				if(m_body != nullptr) {
					m_physics->removeRigidBody(m_body);
					delete m_motionState;
					delete m_body;
					m_body = nullptr;
				}
			}
		}

		xdl::xdl_int Actor::init() {
			if(isPhysicsEnabled()) {
				enablePhysics(xdl::xdl_yes);
			}
			return xdl::ERR_OK;
		}

		void Actor::setPhysics(phys::Physics* physics, xdl::xdl_float mass) {
			m_mass = mass;
			m_physics = physics;
			init();
		}

		btRigidBody* Actor::getRigidBody() {
			return m_body;
		}

		btBoxShape* Actor::getCollisionShape() {
			return m_colShape;
		}

		void Actor::reset() {
			if(m_body == nullptr) {
				return;
			}

			// TODO This part can be optimized. :D I was just to lazy to do, later I will.
			btMotionState* ms = m_body->getMotionState();
			btTransform tf;
			tf.setOrigin(btVector3(getPosition().x, getPosition().y, getPosition().z));
			tf.setRotation(btQuaternion(getOrientation().x, getOrientation().y,getOrientation().z,getOrientation().w));
			ms->setWorldTransform(tf);
		}


	}

}
