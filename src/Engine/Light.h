#ifndef LIGHT_H
#define LIGHT_H

#include <XdevLTypes.h>
#include "Moveable.h"
#include "Color.h"

namespace soan {

	class Light : public Moveable {
		public:

			enum Type {
			  POINT,
			  SPOT,
			  DIRECTIONAL
			};

			Light(const Type& type);

			virtual ~Light();

			/// Returns the type of the light.
			const Type& getType() const;

			/// Returns the intensity of the light.
			xdl::xdl_float getIntensity();

			/// Returns the specular color of the light.
			xdl::xdl_float* getSpecular();

			xdl::xdl_float* getAmbient();

		private:
			Type m_type;
			xdl::xdl_float m_intensity;
			xdl::xdl_float m_specular[3];
			xdl::xdl_float m_ambient[3];

	};

}


#endif
