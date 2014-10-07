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
			m_collisionConfiguration 	= new btDefaultCollisionConfiguration();
			m_dispatcher 								= new	btCollisionDispatcher(m_collisionConfiguration);
			m_broadphase 								= new btDbvtBroadphase();
			m_solver 										= new btSequentialImpulseConstraintSolver;
			m_dynamicsWorld 						= new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);
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
