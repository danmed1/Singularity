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

#include "SignedDistanceFieldGenerator.h"
#include <cmath>
#include <string>
#include <iostream>
#include "FreeImage.h"

namespace soan {
	namespace utils {

		Point inside = { 0, 0 };
		Point empty = { 9999, 9999 };

		SignedDistanceFieldGenerator::SignedDistanceFieldGenerator() {
#ifdef FREEIMAGE_LIB
			FreeImage_Initialise();
#endif
		}

		SignedDistanceFieldGenerator::~SignedDistanceFieldGenerator() {
#ifdef FREEIMAGE_LIB
			FreeImage_DeInitialise();
#endif
		}

		int SignedDistanceFieldGenerator::generateFromFile(const char* filename, const char* outputFilename, int rescaleWidth, int rescaleHeight) {

			//
			// If not then a new texture will be created.
			//
			std::string filetmp;
			filetmp += filename;

			FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

			FIBITMAP* dib = nullptr;

			BYTE* bits 		= nullptr;

			//image width and height
			unsigned int width(0), height(0);



			// Check the file signature and deduce its format
			fif = FreeImage_GetFileType(filetmp.c_str(), 0);
			if(fif == FIF_UNKNOWN) {
				fif = FreeImage_GetFIFFromFilename(filetmp.c_str());
				if(fif == FIF_UNKNOWN) {
					std::cerr << "##TextureServer::import:FreeImage_GetFIFFromFilename() failed. File format unknown" << std::endl;
					return -1;
				}
			}


			//check that the plugin has reading capabilities and load the file
			if(FreeImage_FIFSupportsReading(fif)) {
				std::cout << "TextureServer::import: Import image: " << filetmp << std::endl;
				dib = FreeImage_Load(fif, filetmp.c_str());
				if(!dib) {
					std::cerr << "##TextureServer::import:FreeImage_Load() failed." << std::endl;
					return -1;
				}
			} else {
				std::cerr << "##TextureServer::import:FreeImage_FIFSupportsReading() failed." << std::endl;
			}

			// Convert non-32 bit images
			if(FreeImage_GetBPP(dib) != 32) {
				FIBITMAP* hOldImage = dib;
				dib = FreeImage_ConvertTo32Bits(hOldImage);
				FreeImage_Unload(hOldImage);
			}

			bits = FreeImage_GetBits(dib);
			width = FreeImage_GetWidth(dib);
			height = FreeImage_GetHeight(dib);
			if((bits == 0) || (width == 0) || (height == 0))
				return -1;

			m_rescaleWidth = width;
			m_rescaleHeight = height;

			FIBITMAP* distanceImageData = FreeImage_Allocate(width, height, 32);
			float tmp = 0.0f;
			float counter = 0.0f;

			Grid grid1(width,height);
			Grid grid2(width,height);

			for(unsigned int y = 0; y < height; y++) {
				for(unsigned int x = 0; x < width; x++) {
					RGBQUAD pixel;
					FreeImage_GetPixelColor(dib, x, y, &pixel);

					// Points inside get marked with a dx/dy of zero.
					// Points outside get marked with an infinitely large distance.
					if(pixel.rgbGreen < 128) {
						Put(grid1, x, y, inside);
						Put(grid2, x, y, empty);
					} else {
						Put(grid2, x, y, inside);
						Put(grid1, x, y, empty);
					}
					counter += 1.0f;
					tmp = 100.0f/(width*height)*counter;
				}
				printf("Pass 1: %f %%\r", tmp);

			}
			printf("\n ... finished\n");

			counter = 0.0f;

			// Generate the SDF.
			printf("Generating SDF for Grid 1.\n");
			generateSDF(grid1);

			printf("Generating SDF for Grid 2.\n");
			generateSDF(grid2);

			// Render out the results.
			for(unsigned int y=0; y<height; y++) {
				for(unsigned int x=0; x<width; x++) {
					// Calculate the actual distance from the dx/dy
					int dist1 = (int)(sqrt((double)Get(grid1, x, y).DistSq()));
					int dist2 = (int)(sqrt((double)Get(grid2, x, y).DistSq()));
					int dist = dist1 - dist2;

					// Clamp and scale it, just for display purposes.
					int c = dist*3 + 128;
					if(c < 0) c = 0;
					if(c > 255) c = 255;

					//		RGBQUAD pixel;
					RGBQUAD pixel;
					FreeImage_GetPixelColor(dib, x, y, &pixel);
					pixel.rgbReserved = c;
					FreeImage_SetPixelColor(distanceImageData, x, y, &pixel);
					counter += 1.0f;

					tmp = 100.0f/(width*height)*counter;

				}
				printf("Pass 2: %f %%\r", tmp);
			}

			printf("\n ... finished\n");

			bool rescale = false;
			if(rescaleWidth != -1) {
				m_rescaleWidth = rescaleWidth;
				rescale = true;
			}
			if(rescaleHeight != -1) {
				m_rescaleHeight = rescaleHeight;
				rescale = true;
			}
			if(rescale == true) {
				std::cout << "Recaling to: " << m_rescaleWidth << "x" << m_rescaleHeight << std::endl;
				FIBITMAP* rescaledImage = FreeImage_Rescale(distanceImageData, m_rescaleWidth, m_rescaleHeight, FILTER_BILINEAR);
				FreeImage_Save(FIF_PNG, rescaledImage, outputFilename, PNG_DEFAULT);
				FreeImage_Unload(rescaledImage);
			} else {
				FreeImage_Save(FIF_PNG, distanceImageData, outputFilename, PNG_DEFAULT);

			}

			FreeImage_Unload(distanceImageData);
			FreeImage_Unload(dib);

			return 0;
		}

		Point SignedDistanceFieldGenerator::Get(Grid &g, int x, int y) {
			// OPTIMIZATION: you can skip the edge check code if you make your grid
			// have a 1-pixel gutter.
			if(x >= 0 && y >= 0 && x < g.w && y < g.h)
				return g.grid[x + y*g.w];
			else
				return empty;
		}

		void SignedDistanceFieldGenerator::Put(Grid &g, int x, int y, const Point &p) {
			g.grid[x + y*g.w] = p;
		}

		void SignedDistanceFieldGenerator::Compare(Grid &g, Point &p, int x, int y, int offsetx, int offsety) {
			Point other = Get(g, x+offsetx, y+offsety);
			other.dx += offsetx;
			other.dy += offsety;

			if(other.DistSq() < p.DistSq())
				p = other;
		}

		void SignedDistanceFieldGenerator::generateSDF(Grid &g) {
			// Pass 0
			for(int y=0; y<g.h; y++) {
				for(int x=0; x<g.w; x++) {
					Point p = Get(g, x, y);
					Compare(g, p, x, y, -1,  0);
					Compare(g, p, x, y,  0, -1);
					Compare(g, p, x, y, -1, -1);
					Compare(g, p, x, y,  1, -1);
					Put(g, x, y, p);
				}

				for(int x=g.w-1; x>=0; x--) {
					Point p = Get(g, x, y);
					Compare(g, p, x, y, 1, 0);
					Put(g, x, y, p);
				}
			}

			// Pass 1
			for(int y=g.w-1; y>=0; y--) {
				for(int x=g.h-1; x>=0; x--) {
					Point p = Get(g, x, y);
					Compare(g, p, x, y,  1,  0);
					Compare(g, p, x, y,  0,  1);
					Compare(g, p, x, y, -1,  1);
					Compare(g, p, x, y,  1,  1);
					Put(g, x, y, p);
				}

				for(int x=0; x<g.w; x++) {
					Point p = Get(g, x, y);
					Compare(g, p, x, y, -1, 0);
					Put(g, x, y, p);
				}
			}
		}


	}

}
