#ifndef PHYSICS_H
#define PHYSICS_H

#include <XdevLOpenGL/XdevLShaderProgram.h>
#include <btBulletDynamicsCommon.h>
#include "Utils/SingularityDebugDrawer.h"

// TODO Because of the OpenGL context, the Physcis class has to be initialized
// after initializing the OpenGL context.

namespace soan {

	namespace phys {

		class Physics {
		public:
			Physics();
			virtual ~Physics();
			int init(SingularityDebugDrawer* debugDraw);
			void addRigidBody(btRigidBody* body);
			void removeRigidBody(btRigidBody* body);
			void stepSimulation(double timeStep);
			void debugDrawWorld(tmath::mat4& projectionMatrix);
		private:

			btBroadphaseInterface*						m_broadphase;
			btCollisionDispatcher*						m_dispatcher;
			btConstraintSolver*								m_solver;
			btDefaultCollisionConfiguration* m_collisionConfiguration;
			btDynamicsWorld*									m_dynamicsWorld;
			SingularityDebugDrawer* 					m_debugDrawer;
		};

	}

}

#endif
