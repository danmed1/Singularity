#ifndef UNIVERSAL_UNITS_H
#define UNIVERSAL_UNITS_H

#include <iostream>

namespace soan {

	namespace game {

		class Radius {
			public:
				Radius() : m_radius(0.0) {} 
				Radius(float radius) : m_radius(radius) {}

				inline const Radius& operator+=(const Radius& v) {
					m_radius += v.m_radius;
					return *this;
				}

				inline const Radius& operator-=(const Radius& vVector) {
					m_radius -= vVector.m_radius;

					return *this;
				}

				inline const Radius& operator*=(const float& num) {
					m_radius *= num;
					return *this;
				}

				inline const Radius& operator/=(const float& num) {
					const float r = 1.0/num;
					m_radius *= r;
					return *this;
				}
				
				inline friend const Radius operator+(const Radius& r1, const Radius& r2) {
					return Radius(r1.m_radius + r2.m_radius);
				}
				
				inline friend const Radius operator-(const Radius& r1, const Radius& r2) {
					return Radius(r1.m_radius - r2.m_radius);
				}
				
				inline friend const Radius operator*(const Radius& r1,const Radius& r2) {
					return Radius(r1.m_radius * r2.m_radius);
				}
				
				inline const Radius operator*(const float& num) const {
					return Radius( m_radius * num);
				}
				
				friend inline const Radius operator * (const float& s, const Radius& r) {
					return (r * s);
				}
				
				inline const Radius operator/(const float& num) const {
					const float r = 1.0/num;
					return Radius(m_radius*r);
				}

				inline const bool operator == (const Radius& r) const {
					return (r.m_radius == m_radius);
				}

				inline const bool operator != (const Radius& r) const {
					return !(r == *this);
				}

				inline friend std::ostream& operator<<(std::ostream& out, const Radius& r) {
					out << "Radius: " << r.m_radius;
					return out;
				}

				operator float*() {
					return &m_radius;
				}

				operator const float*() const	{
					return &m_radius;
				}

			private:
				float m_radius;
		};

	}

}

#endif
