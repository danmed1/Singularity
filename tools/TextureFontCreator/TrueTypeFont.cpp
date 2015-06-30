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

#include <TrueTypeFont.h>

#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>

class sdffnel;
namespace soan {

	namespace utils {

		Point inside = { 0, 0 };
		Point empty = { 9999, 9999 };



		TrueTypeFont::TrueTypeFont() throw()	: m_textureWidth(512),
			m_fontSize(32),
			m_centerHorizontal(true),
			m_centerVertical(true),
			m_imageData(nullptr),
			m_counter(0.0f),
			m_gapX(0),
			m_gapY(0) {

			//
			// Initialize freeimage library.
			//
#ifdef FREEIMAGE_LIB
			FreeImage_Initialise();
#endif

			//
			// Initialize freetype library.
			//
			if(FT_Init_FreeType(&library) != 0) {
				throw "TrueTypeFont::TrueTypeFont: Could not initialize FreeType library.";
			}

		}

		TrueTypeFont::~TrueTypeFont() {
			//
			// Uninitialize freeimage library.
			//
#ifdef FREEIMAGE_LIB
			FreeImage_DeInitialise();
#endif
			//
			// Uninitialize freetype library.
			//
			FT_Done_FreeType(library);
		}

		void TrueTypeFont::setGapBetweenGlyphBitmap(unsigned int width, unsigned int height) {
			m_gapX = width;
			m_gapY = height;
		}

		void TrueTypeFont::modifyFilename(const char* filename, const std::string& suffix, std::string& modifiedFilename) {
			std::string tmp(filename);
			std::string outputSDFFilename = tmp.substr(0, tmp.find_last_of("."));
			std::string ext = tmp.substr(tmp.find_last_of(".") + 1, tmp.size());
			outputSDFFilename += suffix + ext;
			modifiedFilename = outputSDFFilename;
		}

		int TrueTypeFont::create(const char* fontFilename,
		                         const char* outputFilename,
		                         unsigned int fontSize,
		                         unsigned int textureSize) {
			//
			// Initialize freetype and load a font.
			//
			FT_Face face;
			if(FT_New_Face(library, fontFilename, 0, &face) != 0) {
				std::cerr << "TrueTypeFont::create: Could not create new Freetype face." << std::endl;
				return -1;
			}

			//
			// Set the size of the font.
			//
			if(FT_Set_Pixel_Sizes(face, fontSize, fontSize) != 0) {
				std::cerr << "TrueTypeFont::create: Could not set the pixel size of the Freetype face." << std::endl;
				return -1;
			}

			FIBITMAP* imageData = FreeImage_Allocate(textureSize, textureSize, 32);
			RGBQUAD pixel = {0xff, 0x00, 0x00, 0x00};
			FreeImage_FillBackground(imageData, &pixel, FI_COLOR_IS_RGBA_COLOR);


			std::stringstream 	values;

			unsigned int left = 0;
			unsigned int top = 0;
			for(FT_ULong charcode = 32; charcode < 1024; charcode++) {

				// Check if the glyph exists, if not, for now just skip
				FT_UInt glyph_index = FT_Get_Char_Index(face, charcode);
				if( glyph_index == 0) {
					continue;
				}
				
				FT_Load_Glyph(face, glyph_index, FT_LOAD_NO_HINTING );
				FT_GlyphSlot slot = face->glyph;
				FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL);
				FT_Bitmap& bitmap = slot->bitmap;
				FT_Glyph_Metrics metrics = face->glyph->metrics;

				values << 0												// The texture id.
				       << " " << charcode			 					// id
				       << " " << (double)slot->bitmap_left/64.0			// left
				       << " " << (double)slot->bitmap_top/64.0			// top
				       << " " << (double)metrics.width/64.0				// width
				       << " " << (double)metrics.height/64.0			// height
				       << " " << (double)slot->advance.x/64.0			// Advance to next glyph for horizontal layout.
				       << " " << (double)slot->advance.y/64.0			// Advance to next glyph for horizontal layout.
				       << " " << (double)metrics.horiBearingX/64.0		// Horizontal layout bearing in x.
				       << " " << (double)metrics.horiBearingY/64.0		// Horizontal layout bearing in y.
				       << " " << (double)metrics.vertBearingX/64.0		// Vertical layout bearing in x.
				       << " " << (double)metrics.vertBearingY/64.0;		// Vertical layout bearing in y.



				if(left + bitmap.width > textureSize) {
					left = 0;
					top += fontSize;
				}

				for(int h = 0; h < bitmap.rows; ++h) {
					for(int w = 0; w < bitmap.width; ++w) {

						//
						// Set the position of the pixel.
						//
						int xpos = left + w;
						int ypos = top + h;

						uint8_t intensitiy	= bitmap.buffer[w + bitmap.pitch * h];
						pixel.rgbBlue 		= intensitiy;
						pixel.rgbGreen 		= intensitiy;
						pixel.rgbRed 		= intensitiy;
						pixel.rgbReserved 	= 255;

						FreeImage_SetPixelColor(imageData, xpos, ypos, &pixel);

					}
				}
				
				//
				// Save Texture coordinates.
				//
				values  << " " << left
				        << " " << top
				        << " " << (left + bitmap.width)
				        << " " << (top - bitmap.rows);
				
				values << std::endl;
				
				left += bitmap.width;
				if(left >= textureSize) {
					left = 0;
					top += fontSize;
				}
			}

			FreeImage_FlipVertical(imageData);
			FreeImage_Save(FIF_PNG, imageData, outputFilename, PNG_Z_NO_COMPRESSION);
			FreeImage_Unload(imageData);
			FT_Done_Face(face);
			
			std::string outputFileNameString(outputFilename);
			std::string glyphInfoFile(outputFileNameString);
			std::string glyphInfoFileExtLess = glyphInfoFile.substr(0, glyphInfoFile.find_last_of("."));
			glyphInfoFile = glyphInfoFileExtLess + "_info.txt";
			std::string fileNames;
			fileNames+= outputFileNameString + "\n";
			std::ofstream 	fs(glyphInfoFile);
			if(fs.is_open()) {

				// Save something useless.
				fs << "(left right width height horizontal_advance vertical_advance h_bearing_x h_bearing_y v_bearing_x v_bearing_y) \n";

				// Save the version of the file.
				fs << "version: 1\n";

				// Save the font size.
				fs << fontSize << std::endl;

				fs << 1 << std::endl;

				fs << fileNames;

				fs << values.str();
			}


			if(fs.is_open()) {
				fs.close();
			}
			return 0;

		}

		int TrueTypeFont::create(const char* fontFilename,
		                         const char* outputFilename,
		                         unsigned int firstCharacter,
		                         unsigned int numberOfCharacters,
		                         unsigned int fontSize,
		                         unsigned int textureSize,
		                         int rescaleWidth,
		                         int rescaleHeight) {

			m_rescaleHeight = rescaleHeight;
			m_rescaleWidth = rescaleWidth;
			m_textureWidth = textureSize;


			std::string outputFileNameString(outputFilename);

			// Calculate texturesize
			m_fontSize = fontSize + m_gapX;

			unsigned int MaxCols = m_textureWidth/(fontSize + m_gapX);

			float scaleWidth = 1;
			float scaleHeight = 1;

			bool rescale = false;
			if(rescaleWidth != -1) {
				m_rescaleWidth = rescaleWidth;
				scaleWidth = (float)rescaleWidth/(float)textureSize;
				rescale = true;
			}
			if(rescaleHeight != -1) {
				m_rescaleHeight = rescaleHeight;
				scaleHeight = (float)rescaleHeight/(float)textureSize;
				rescale = true;
			}

			//
			// Initialize freetype and load a font.
			//
			FT_Face face;
			if(FT_New_Face(library, fontFilename, 0, &face) != 0) {
				std::cerr << "TrueTypeFont::create: Could not create new Freetype face." << std::endl;
				return -1;
			}

			//
			// Set the size of the font.
			//
			if(FT_Set_Pixel_Sizes(face, fontSize, fontSize) != 0) {
				std::cerr << "TrueTypeFont::create: Could not set the pixel size of the Freetype face." << std::endl;
				return -1;
			}

			//
			// Create the glyph info file.
			//


			std::string glyphInfoFile(outputFileNameString);
			std::string glyphInfoFileExtLess = glyphInfoFile.substr(0, glyphInfoFile.find_last_of("."));
			glyphInfoFile = glyphInfoFileExtLess + "_info.txt";

			std::string outputSDFFilename;
			modifyFilename(outputFileNameString.c_str(), "_dft.", outputSDFFilename);

			std::stringstream 	values;
			std::string					fileNames;
			std::string					sdfFileNames;
			fileNames+= outputFileNameString + "\n";

			//
			// Allocate memory for the font texture we will create.
			//
			// TODO, this part should be replaced to make the project indipendent
			// from FreeImage library.
			//
			m_imageData = FreeImage_Allocate(m_textureWidth, m_textureWidth, 32);

			RGBQUAD pixel = {0xff, 0x00, 0x00, 0x00};
			FreeImage_FillBackground(m_imageData, &pixel, FI_COLOR_IS_RGBA_COLOR);
			//
			// Go through all glyphs
			//

			unsigned int i = 0;
			unsigned int rows = 0;
			unsigned int cols = 0;
			unsigned int textureNumber = 0;


			//
			// Start extracting the glyph bitmap data.
			//
			while(i < numberOfCharacters) {

				if(FT_Get_Char_Index(face, i + firstCharacter) == 0) {
					i++;
					continue;
				}

				if(FT_Load_Char(face, i + firstCharacter, FT_LOAD_RENDER) != 0) {
					i++;
					continue;
				}

				FT_GlyphSlot 			glyphSlot = face->glyph;
				FT_Glyph_Metrics 	metrics 	= face->glyph->metrics;
				FT_Bitmap&				bitmap 		= glyphSlot->bitmap;


				values << textureNumber																					// The texture id.
				       << " " << firstCharacter + i 														// id
				       << " " << (double)glyphSlot->bitmap_left/64.0*scaleWidth	// left
				       << " " << (double)glyphSlot->bitmap_top/64.0*scaleHeight	// top
				       << " " << (double)metrics.width/64.0*scaleWidth					// width
				       << " " << (double)metrics.height/64.0*scaleHeight				// height
				       << " " << (double)glyphSlot->advance.x/64.0*scaleWidth		// Advance to next glyph for horizontal layout.
				       << " " << (double)glyphSlot->advance.y/64.0*scaleHeight	// Advance to next glyph for horizontal layout.
				       << " " << (double)metrics.horiBearingX/64.0*scaleWidth		// Horizontal layout bearing in x.
				       << " " << (double)metrics.horiBearingY/64.0*scaleHeight	// Horizontal layout bearing in y.
				       << " " << (double)metrics.vertBearingX/64.0*scaleWidth		// Vertical layout bearing in x.
				       << " " << (double)metrics.vertBearingY/64.0*scaleHeight;	// Vertical layout bearing in y.



				unsigned int currentLine = rows*m_fontSize;
				unsigned int currentColumn = cols*m_fontSize;

				unsigned int center_dx = (m_fontSize - bitmap.width)/2;
				unsigned int center_dy = (m_fontSize - bitmap.rows)/2;

				if(m_centerHorizontal == false) {
					center_dx = 0;
				}

				if(m_centerVertical == false) {
					center_dy = 0;
				}

				//
				// Copy glyph image into memory.
				//
				int left 	= (center_dx + currentColumn + m_gapX) ;
				int top 	= (-center_dy + m_textureWidth - currentLine - m_gapX);

				for(int h = 0; h < bitmap.rows; ++h) {
					for(int w = 0; w < bitmap.width; ++w) {

						//
						// Set the position of the pixel.
						//
						int xpos = left + w;
						int ypos = top - h - 1 ;

						uint8_t intensitiy	= bitmap.buffer[w + bitmap.pitch * h];
						pixel.rgbBlue 			= intensitiy;
						pixel.rgbGreen 			= intensitiy;
						pixel.rgbRed 				= intensitiy;
						pixel.rgbReserved 	= intensitiy;

						FreeImage_SetPixelColor(m_imageData, xpos, ypos, &pixel);

					}
				}

				//
				// Save Texture coordinates.
				//
				values  << " " << left*scaleWidth
				        << " " << top*scaleHeight
				        << " " << (left + bitmap.width)*scaleWidth
				        << " " << (top - bitmap.rows)*scaleHeight;

				cols++;
				if(cols % MaxCols == 0) {
					cols = 0;
					rows++;
				}

				values << std::endl;

				i++;


				if(rows == MaxCols) {

					//
					// Create SDF's
					//
					createDistanceImage();

					if(rescale == true) {
						FIBITMAP* rescaledDistanceImage = FreeImage_Rescale(m_distanceImageData, m_rescaleWidth, m_rescaleHeight, FILTER_BILINEAR);
						FreeImage_Unload(m_distanceImageData);
						m_distanceImageData = rescaledDistanceImage;
					}

					FreeImage_Save(FIF_PNG, m_distanceImageData, outputSDFFilename.c_str(), PNG_DEFAULT);


					//
					// Save file font texture.
					//
					if(rescale == true) {
						std::cout << "Recaling to: " << m_rescaleWidth << "x" << m_rescaleHeight << std::endl;
						FIBITMAP* rescaledImage = FreeImage_Rescale(m_imageData, m_rescaleWidth, m_rescaleHeight, FILTER_BILINEAR);
						FreeImage_Unload(m_imageData);
						m_imageData = rescaledImage;
					}

					FreeImage_Save(FIF_PNG, m_imageData, outputFileNameString.c_str(), PNG_DEFAULT);

					//	RGBQUAD background = {0x00, 0x00, 0x00, 0x00};
					//FreeImage_FillBackground(m_imageData, &background, FI_COLOR_IS_RGBA_COLOR);
					// TODO I tried to delete the buffer but it is not really doing what it is supposed to do.
					FreeImage_Unload(m_imageData);

					m_imageData = FreeImage_Allocate(m_textureWidth, m_textureWidth, 32);

					cols = 0;
					rows = 0;

					//
					// Create new filenames.
					//
					if(i < numberOfCharacters) {
						textureNumber++;

						std::stringstream ss;
						ss << textureNumber;
						std::string suffix 		= "_" + ss.str() + ".";
						std::string sdfSuffix = "_" + ss.str() + "_dft.";
						modifyFilename(outputFilename, suffix, outputFileNameString);
						modifyFilename(outputFilename, sdfSuffix, outputSDFFilename);


						fileNames+= outputFileNameString + "\n";
						sdfFileNames+= outputSDFFilename + "\n";
					}

				}

			}
			//
			// Save last image.
			//
			createDistanceImage();
			if(rescale == true) {
				FIBITMAP* rescaledDistanceImage = FreeImage_Rescale(m_distanceImageData, m_rescaleWidth, m_rescaleHeight, FILTER_BILINEAR);
				FreeImage_Unload(m_distanceImageData);
				m_distanceImageData = rescaledDistanceImage;
			}

			FreeImage_Save(FIF_PNG, m_distanceImageData, outputSDFFilename.c_str(), PNG_DEFAULT);


			if(rescale == true) {
				std::cout << "Recaling to: " << m_rescaleWidth << "x" << m_rescaleHeight << std::endl;
				FIBITMAP* rescaledImage = FreeImage_Rescale(m_imageData, m_rescaleWidth, m_rescaleHeight, FILTER_BILINEAR);
				FreeImage_Unload(m_imageData);
				m_imageData = rescaledImage;
			}
			FreeImage_Save(FIF_PNG, m_imageData, outputFileNameString.c_str(), PNG_DEFAULT);




			// Destroy everything
			//
			FT_Done_Face(face);
			FreeImage_Unload(m_imageData);
			FreeImage_Unload(m_distanceImageData);



			//
			// Save all glyph information.
			//
			std::cout << values.str() << std::endl;
			std::cout << fileNames << std::endl;
			std::cout << sdfFileNames << std::endl;
			std::cout << glyphInfoFile << std::endl;



			std::ofstream 	fs(glyphInfoFile);

			if(fs.is_open()) {

				// Save something useless.
				fs << "(left right width height horizontal_advance vertical_advance h_bearing_x h_bearing_y v_bearing_x v_bearing_y) \n";

				// Save the version of the file.
				fs << "version: 1\n";

				// Save the font size.
				fs << fontSize*scaleWidth << std::endl;

				fs << (textureNumber+1) << std::endl;

				fs << fileNames;

				fs << values.str();
			}


			if(fs.is_open()) {
				fs.close();
			}


			return 0;
		}

		void TrueTypeFont::getPosition(unsigned int idx, unsigned int size, unsigned int& x, unsigned int& y) {
			x = idx % size;
			y = idx / size;
		}

		int TrueTypeFont::createDistanceImage() {
			//
			// Create the Glyph texture.
			//

			m_distanceImageData = FreeImage_Allocate(m_textureWidth, m_textureWidth, 32);


			std::cout << "Initializing Grid 1&2 ... " << std::flush;

			Grid grid1(m_textureWidth,m_textureWidth);
			Grid grid2(m_textureWidth,m_textureWidth);

			for(unsigned int y = 0; y < m_textureWidth; y++) {
				for(unsigned int x = 0; x < m_textureWidth; x++) {
					RGBQUAD pixel;
					FreeImage_GetPixelColor(m_imageData, x, y, &pixel);

					// Points inside get marked with a dx/dy of zero.
					// Points outside get marked with an infinitely large distance.
					if(pixel.rgbGreen < 128) {
						put(grid1, x, y, inside);
						put(grid2, x, y, empty);
					} else {
						put(grid2, x, y, inside);
						put(grid1, x, y, empty);
					}
				}
			}

			std::cout << "finished." << std::endl;

			//
			// Generate the SDF.
			//
			std::cout << "Generating SDF for Grid 1 ... " << std::flush;
			generateSDF(grid1);
			std::cout << "finished." << std::endl;

			std::cout << "Generating SDF for Grid 2 ... " << std::flush;
			generateSDF(grid2);
			std::cout << "finished." << std::endl;


			std::cout << "Calculating signed distance values ... " << std::flush;
			//
			// Render out the results.
			//
			for(unsigned int y=0; y<m_textureWidth; y++) {
				for(unsigned int x=0; x<m_textureWidth; x++) {
					// Calculate the actual distance from the dx/dy
					int dist1 = (int)(sqrt((double)get(grid1, x, y).DistSq()));
					int dist2 = (int)(sqrt((double)get(grid2, x, y).DistSq()));
					int dist = dist1 - dist2;

					// Clamp and scale it, just for display purposes.
					int c = dist*3 + 128;
					if(c < 0) c = 0;
					if(c > 255) c = 255;

					RGBQUAD pixel;
					FreeImage_GetPixelColor(m_imageData, x, y, &pixel);
					pixel.rgbReserved = c;

					FreeImage_SetPixelColor(m_distanceImageData, x, y, &pixel);

				}

			}

			std::cout << " ... finished" << std::endl;


			return 0;
		}





		int TrueTypeFont::saveImage(const char* filename) {

			//
			// If not then a new texture will be created.
			//
			std::string filetmp = filename;

			FreeImage_Save(FIF_PNG, m_imageData, filetmp.c_str(), PNG_DEFAULT);

			FreeImage_Unload(m_imageData);


			std::cout << "TrueTypeFont::saveImage: File: " << filetmp << " saved succesfully." << std::endl;

			return 0;

		}

		void TrueTypeFont::getPixel(unsigned char* buffer,unsigned int width, unsigned int x, unsigned y, Pixel32& pixel) {
			pixel.channel1 = buffer[x + y*width*4 + 0];
			pixel.channel2 = buffer[x + y*width*4 + 1];
			pixel.channel3 = buffer[x + y*width*4 + 2];
			pixel.channel4 = buffer[x + y*width*4 + 3];
			return ;
		}

		void TrueTypeFont::setPixel(unsigned char* buffer,unsigned int width, unsigned int x, unsigned y, Pixel32& pixel) {
			buffer[x + y*width*4 + 0] = pixel.channel1;
			buffer[x + y*width*4 + 1] = pixel.channel2;
			buffer[x + y*width*4 + 2] = pixel.channel3;
			buffer[x + y*width*4 + 3] = pixel.channel4;
		}

		Point TrueTypeFont::get(Grid &g, int x, int y) {
			// OPTIMIZATION: you can skip the edge check code if you make your grid
			// have a 1-pixel gutter.
			if(x >= 0 && y >= 0 && x < g.w && y < g.h)
				return g.grid[x + y*g.w];
			else
				return empty;
		}

		void TrueTypeFont::put(Grid &g, int x, int y, const Point &p) {
			g.grid[x + y*g.w] = p;
		}

		void TrueTypeFont::compare(Grid &g, Point &p, int x, int y, int offsetx, int offsety) {
			Point other = get(g, x+offsetx, y+offsety);
			other.dx += offsetx;
			other.dy += offsety;

			if(other.DistSq() < p.DistSq())
				p = other;
		}

		void TrueTypeFont::generateSDF(Grid &g) {

			// Pass 0
			for(int y=0; y<g.h; y++) {
				for(int x=0; x<g.w; x++) {
					Point p = get(g, x, y);
					compare(g, p, x, y, -1,  0);
					compare(g, p, x, y,  0, -1);
					compare(g, p, x, y, -1, -1);
					compare(g, p, x, y,  1, -1);
					put(g, x, y, p);
				}

				for(int x=g.w-1; x>=0; x--) {
					Point p = get(g, x, y);
					compare(g, p, x, y, 1, 0);
					put(g, x, y, p);
				}
			}

			// Pass 1
			for(int y=g.w-1; y>=0; y--) {
				for(int x=g.h-1; x>=0; x--) {
					Point p = get(g, x, y);
					compare(g, p, x, y,  1,  0);
					compare(g, p, x, y,  0,  1);
					compare(g, p, x, y, -1,  1);
					compare(g, p, x, y,  1,  1);
					put(g, x, y, p);

				}

				for(int x=0; x<g.w; x++) {
					Point p = get(g, x, y);
					compare(g, p, x, y, -1, 0);
					put(g, x, y, p);
				}

			}

		}

	}



}
