#ifndef COLOR_H
#define COLOR_H

namespace soan {
	

	class Color {
	public:
		Color();
		Color(float red, float green, float blue) : r(red), g(green), b(blue), a(1.0f) {}
		Color(float red, float green, float blue, float alpha) : r(red), g(green), b(blue), a(alpha) {}
		float r;
		float g;
		float b;
		float a;
	};
	
}

#endif
