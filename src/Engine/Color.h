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
			Color(float red, float green, float blue, float alpha) : r(red), g(green), b(blue), a(alpha) {}
			union {
				struct {
					float c;
					float m;
					float y;
					float k;
				};

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
		
		Color getCMYK() {
			Color color(r, g, b, a);
			color.covertToCMYK();
			return std::move(color);
		}
		
		void covertToCMYK() {
			float K = 1.0f - std::max(std::max(r, g), b);
			float C = (1.0f - r - K) / (1.0f - K);
			float M = (1.0f - g - K) / (1.0f - K);
			float Y = (1.0f - b - K) / (1.0f - K);
			c = C;
			y = Y;
			m = M;
			k = K;
		}
		
	};

}

#endif
