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
		class Actor : public Renderable {
			public:

				Actor();

				virtual ~Actor();

				/// Returns the name.
				std::string& getName();

				/// Sets the name.
				void setName(const std::string& name);

				/// Returns the lifetime.
				xdl::xdl_uint64	 getLifeTime();

				/// Sets the lifetime.
				/**
					@param lifeTime Life time of the actor. If this is zero the actor lives infinit.
				*/
				void setLifeTime(xdl::xdl_uint64 lifeTime);

				/// Increase the lived time.
				/**
					@param timeStep A time step in milliseconds.
				*/
				void increaseLivedTime(xdl::xdl_uint64 timeStep);

				/// Update actors state.
				virtual bool update(xdl::xdl_uint64 timeStep);

				/// Initialize the actor.
				virtual xdl::xdl_int init();
				
			private:
				/// Holds the name of the actor.
				std::string 		m_name;

				/// Holds the lifetime of the actor.
				xdl::xdl_uint64	m_lifeTime;

				/// Holds the lived time.
				xdl::xdl_uint64 m_livedTime;

		};

	}

}

#endif
