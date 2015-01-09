#ifndef COLOR_H
#define COLOR_H

namespace soan {
	
	/**
		@class Color
		@brief A class that describes a color.
		
	*/
	class Color {
		public:
			Color() : r(0.0f), g(0.0f), b(0.0f), a(0.0f) {}
			union {

				struct {
					float r;
					float g;
					float b;
					float a;
				};
				float components[4];
			};
		bool operator == (const Color& color) const { return ( (color.r == r) && (color.g == g) && (color.b == b) && (color.a == a) );}
		bool operator != (const Color& color) const { return !( (color.r == r) && (color.g == g) && (color.b == b) && (color.a == a) );}
		
	};

}

#endif
