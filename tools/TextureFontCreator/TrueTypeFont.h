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

#ifndef TRUETYPEFONT_H
#define TRUETYPEFONT_H

#include "FreeImage.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H
#include FT_TRIGONOMETRY_H
#include FT_LCD_FILTER_H

#include <string>

namespace soan {

	namespace utils {

		struct Pixel8 {
			unsigned char channel1;
		};

		struct Pixel16 {
			unsigned char channel1;
			unsigned char channel2;
		};

		struct Pixel24 {
			unsigned char channel1;
			unsigned char channel2;
			unsigned char channel3;
		};

		struct Pixel32 {
			unsigned char channel1;
			unsigned char channel2;
			unsigned char channel3;
			unsigned char channel4;
		};

		struct Point {
			int dx, dy;

			int DistSq() const {
				return dx*dx + dy*dy;
			}
		};

		struct Grid {
			Grid(int width, int height) : w(width), h(height) {
				grid = new Point[w*h];
			}
			~Grid() {
				delete [] grid;
			}

			Point* grid;
			int w;
			int h;
		};


		class TrueTypeFont	 {

			public:
				TrueTypeFont() throw();
				virtual ~TrueTypeFont();

				int create(const char* fontFilename,
				           const char* outputFilename,
				           unsigned int fonSize,
				           unsigned int textureSize);

				int create(const char* fontFilename,
				           const char* outputFilename,
				           unsigned int firstCharacter,
				           unsigned int numberOfCharacters,
				           unsigned int fontSize,
				           unsigned int textureSize,
				           int rescaleWidth = -1,
				           int rescaleHeight = -1);

				int createDistanceImage();

				virtual int saveImage(const char* filename);

				/// Sets the gap width and height of the glyph elements.
				virtual void setGapBetweenGlyphBitmap(unsigned int width, unsigned int height);
			protected:
				void modifyFilename(const char* filename, const std::string& suffix, std::string& modifiedFilename);

				void getPosition(unsigned int idx, unsigned int size, unsigned int& x, unsigned int& y);

				//	void transform( int scale_down, float spread);
				//		float signedDistance(int cx, int cy, float clamp);
				Point get(Grid &g, int x, int y);
				void put(Grid &g, int x, int y, const Point &p);
				void compare(Grid &g, Point &p, int x, int y, int offsetx, int offsety);
				void generateSDF(Grid &g);
				void getPixel(unsigned char* buffer,unsigned int width, unsigned int x, unsigned y, Pixel32& pixel);
				void setPixel(unsigned char* buffer,unsigned int width, unsigned int x, unsigned y, Pixel32& pixel);
			protected:
				unsigned int 	m_textureWidth;
				unsigned int 	m_fontSize;
				int 					m_rescaleWidth;
				int						m_rescaleHeight;
				bool					m_centerHorizontal;
				bool					m_centerVertical;

				FIBITMAP* 		m_imageData;
				FIBITMAP* 		m_distanceImageData;
				FT_Library 		library;

				float 				m_counter;
				unsigned int 	m_gapX;
				unsigned int 	m_gapY;


		};
	}
}

#endif
