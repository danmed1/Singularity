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
