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

#include "Engine/Physics/Physics.h"

namespace soan {

	namespace phys {

		Physics::Physics() {

		}

		Physics::~Physics() {
			delete m_dynamicsWorld;
			delete m_solver;
			delete m_broadphase;
			delete m_dispatcher;
			delete m_collisionConfiguration;
		}

		int Physics::init(SingularityDebugDrawer* debugDraw) {
			//
			// Do physics setup.
			//
			m_collisionConfiguration	= new btDefaultCollisionConfiguration();
			m_dispatcher 							= new btCollisionDispatcher(m_collisionConfiguration);
			m_broadphase 							= new btDbvtBroadphase();
			m_solver 									= new btSequentialImpulseConstraintSolver;
			m_dynamicsWorld 					= new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);
			m_dynamicsWorld->setGravity(btVector3(0.0f, -10.0f,0.0f));


			m_debugDrawer = debugDraw;
			if(debugDraw != nullptr) {
				m_dynamicsWorld->setDebugDrawer(m_debugDrawer);
				m_debugDrawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe |
				                            btIDebugDraw::DBG_DrawAabb |
				                            btIDebugDraw::DBG_DrawContactPoints |
				                            btIDebugDraw::DBG_DrawNormals |
				                            btIDebugDraw::DBG_DrawFrames |
				                            btIDebugDraw::DBG_DrawText |
				                            btIDebugDraw::DBG_EnableCCD);
			}
			return 0;
		}

		void Physics::addRigidBody(btRigidBody* body) {
			m_dynamicsWorld->addRigidBody(body);
		}

		void Physics::removeRigidBody(btRigidBody* body) {
			m_dynamicsWorld->removeRigidBody(body);
		}

		void Physics::stepSimulation(double timeStep) {
			m_dynamicsWorld->stepSimulation(timeStep);
		}

		void Physics::debugDrawWorld(tmath::mat4& projectionMatrix) {
			if(m_debugDrawer != nullptr) {
				m_dynamicsWorld->debugDrawWorld();
				m_debugDrawer->doDrawing(projectionMatrix);
			}
		}

	}

}
