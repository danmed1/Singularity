
#include "Game/Actor.h"

namespace soan {

	namespace game {

		Actor::Actor() : m_name("Not Specified"), m_lifeTime(0), m_livedTime(0) {

		}

		Actor::~Actor() {

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
			
			return Renderable::update(timeStep);

		}

		xdl::xdl_int Actor::init() {
			
			return xdl::ERR_OK;
		}
	}

}
