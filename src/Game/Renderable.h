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

#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "Engine/Physics/Physics.h"
#include "Engine/Entity/Model.h"
#include <btBulletDynamicsCommon.h>
#include <memory>

namespace soan {

	namespace game {

		class Renderable {
			public:
				Renderable();

				Renderable(phys::Physics* physics, Model* model);

				virtual ~Renderable();

				xdl::xdl_int init() ;

				/// Returns the assigned model
				const std::shared_ptr<Model>& getModel();

				/// Sets the model.
				void setModel(const std::shared_ptr<Model>& model, xdl::xdl_uint collisionShape = 0);

				/// Return the rigid body.
				btRigidBody*	getRigidBody();
				
				/// Returns the collision shape.
				btBoxShape* getCollisionShape();

				virtual bool update(xdl::xdl_uint64 timeStep);

				xdl::xdl_bool isRenderingEnabled();

				xdl::xdl_bool getCastShadow();

				void setRenderingEnabled(xdl::xdl_bool state);

				void setCastShadow(xdl::xdl_bool state);

				void setPhysics(phys::Physics* physics, xdl::xdl_float mass);

				void reset();

			private:
				phys::Physics*						m_physics;
				std::shared_ptr<Model>		m_model;
				btRigidBody* 							m_body;
				btDefaultMotionState* 		m_motionState;
				btBoxShape*								m_colShape;
				xdl::xdl_bool							m_renderingEnabled;
				xdl::xdl_bool							m_castShadow;
				xdl::xdl_float						m_mass;
		};

	}

}

#endif
