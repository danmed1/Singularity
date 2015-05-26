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

#ifndef ACTOR_H
#define ACTOR_H

#include <XdevLTypes.h>
#include <string>

#include "Game/Renderable.h"

namespace soan {

	namespace game {

		/**
			@class Actor
			@brief Objects that have some sort of behaviour in the game.
			@author Cengiz Terzibas
		*/
		class Actor : public Renderable, public Transformable  {
		public:

			Actor();

			virtual ~Actor();

			/// Returns the name.
			std::string& getName();

			/// Sets the name.
			void setName(const std::string& name);

			/// Returns the lifetime.
			xdl::xdl_uint64 getLifeTime();

			/// Sets the lifetime.
			/**
				@param lifeTime Life time of the actor. If this is zero the actor lives infinit.
			*/
			void setLifeTime(xdl::xdl_uint64 lifeTime);

			/// Returns the lived time.
			xdl::xdl_uint64 getLivedTime() const;

			/// Increase the lived time.
			/**
				@param timeStep A time step in milliseconds.
			*/
			void increaseLivedTime(xdl::xdl_uint64 timeStep);

			/// Update actors state.
			virtual bool update(xdl::xdl_uint64 timeStep);

			/// Initialize the actor.
			virtual xdl::xdl_int init();

			void setPhysics(phys::Physics* physics, xdl::xdl_float mass);
			
			/// Apply force to the center.
			void applyForce(const tmath::vec3& force);
			
			/// Apply impulse to the center.
			void applyImpulse(const tmath::vec3& impulse);
			
			
			tmath::quat getOrientation();
			
			/// Return the rigid body.
			btRigidBody* getRigidBody();

			/// Returns the collision shape.
			btBoxShape* getCollisionShape();
			
			void reset();
			
			/// Change the state of the physics system.
			void enablePhysics(xdl::xdl_bool enableIt);
			
			/// Is the physics system enabled?
			xdl::xdl_bool isPhysicsEnabled() const;
			
		private:
			/// Holds the name of the actor.
			std::string				m_name;

			/// Holds the lifetime of the actor.
			xdl::xdl_uint64			m_lifeTime;

			/// Holds the current lived time.
			xdl::xdl_uint64			m_livedTime;

			phys::Physics*			m_physics;
			btRigidBody* 			m_rigidBody;
			btDefaultMotionState* 	m_motionState;
			btBoxShape*				m_colShape;
			xdl::xdl_float			m_mass;
			xdl::xdl_bool			m_physicsEnabled;

		};

	}

}

#endif
