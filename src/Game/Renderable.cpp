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

#include "Game/Renderable.h"

namespace soan {

	namespace game {

		Renderable::Renderable() :	m_physics(nullptr),	 
																m_model(nullptr), 
																m_body(nullptr),
																m_motionState(nullptr),
																m_colShape(nullptr),
																m_renderingEnabled(xdl::xdl_false),
																m_castShadow(xdl::xdl_true),
																m_mass(1.0f) {

		}

		Renderable::Renderable(phys::Physics* physics, Model* model) : m_physics(physics), m_model(model), m_renderingEnabled(xdl::xdl_true) {

		}

		Renderable::~Renderable() {
			if(m_body != nullptr) {
				m_physics->removeRigidBody(m_body);
				delete m_motionState;
				delete m_body;
				m_body = nullptr;
			}
		}

		xdl::xdl_int Renderable::init() {
			if(m_physics != nullptr) {

				btTransform startTransform;
				startTransform.setIdentity();


				startTransform.setOrigin(btVector3(getPosition().x,getPosition().y,getPosition().z));


				m_colShape = new btBoxShape(btVector3(m_model->getBoundingBoxX()/2.0, m_model->getBoundingBoxY()/2.0f, m_model->getBoundingBoxZ()/2.0f));
				btVector3 localInertia(0,0,0);

				m_colShape->calculateLocalInertia(m_mass, localInertia);


				//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
				m_motionState = new btDefaultMotionState(startTransform);
				btRigidBody::btRigidBodyConstructionInfo rbInfo(m_mass, m_motionState, m_colShape, localInertia);
				m_body = new btRigidBody(rbInfo);

				m_body->setActivationState(DISABLE_DEACTIVATION);
				m_physics->addRigidBody(m_body);
			}
			return xdl::ERR_OK;
		}

		void Renderable::setPhysics(phys::Physics* physics, xdl::xdl_float mass) {
			m_mass = mass;
			m_physics = physics;
			init();
		}

		const std::shared_ptr<Model>& Renderable::getModel() {
			assert(isRenderingEnabled() && "Renderable::getModel(): Rendering not enabled");
			return m_model;
		}

		void Renderable::setModel(const std::shared_ptr<Model>& model, xdl::xdl_uint collisionShape) {
			m_model = model;
			m_renderingEnabled = xdl::xdl_true;

//
//			switch(collisionShape) {
//			case 1: {
//
//					btSphereShape* sphereShape = new btSphereShape(m_model->getBoundingBoxX()/2.0);
//					btVector3 localInertia(0,0,0);
//
//					sphereShape->calculateLocalInertia(m_mass, localInertia);
//
//					m_body->setCollisionShape(sphereShape);
//				}
//				break;
//				case 0:
//				default: {
//					m_colShape = new btBoxShape(btVector3(m_model->getBoundingBoxX()/2.0, m_model->getBoundingBoxY()/2.0f, m_model->getBoundingBoxZ()/2.0f));
//					btVector3 localInertia(0,0,0);
//					m_colShape->calculateLocalInertia(m_mass, localInertia);
//					m_body->setCollisionShape(m_colShape);
//				}break;
//			}


		}

		void Renderable::setRenderingEnabled(xdl::xdl_bool state) {
			m_renderingEnabled = state;
		}

		xdl::xdl_bool Renderable::getCastShadow() {
			return m_castShadow;
		}

		void Renderable::setCastShadow(xdl::xdl_bool state) {
			m_castShadow = state;
		}

		btRigidBody* Renderable::getRigidBody() {
			return m_body;
		}
		
		btBoxShape* Renderable::getCollisionShape() {
			return m_colShape;
		}

		void Renderable::reset() {
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

		bool Renderable::update(xdl::xdl_uint64 timeStep) {
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
			// We have to return false because it did not die yet.
			return false;

		}

		xdl::xdl_bool Renderable::isRenderingEnabled() {
			return m_renderingEnabled;
		}

	}

}
