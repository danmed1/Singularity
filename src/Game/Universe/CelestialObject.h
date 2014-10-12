#ifndef CELESTIAL_OBJECTS_H
#define CELESTIAL_OBJECTS_H


#include <Game/Actor.h>
#include <Engine/Entity/Model.h>

namespace soan {

	namespace game {

		class CelestialObject : public Actor {
		public:
			
				CelestialObject();
				
				virtual ~CelestialObject();
							
				/// Return the mass of the astroid.
				float& 				getMass();
											
				/// Sets the mass.
				void setMass(float mass);

				/// Update the objects state.
				virtual bool update(xdl::xdl_uint64 timeStep);

				/// Add a new mesh to the astroid.
				void add(float mass, const std::shared_ptr<Model>& model);
				
			protected:

				float											m_mass;
				
		};

	}

}

#endif
