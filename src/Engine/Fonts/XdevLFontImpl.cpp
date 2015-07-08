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

#include "XdevLFontImpl.h"
#include "lodepng.h"
#include <Engine/Types.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>


namespace soan {


	XdevLFontImpl::XdevLFontImpl(xdl::xdl_uint screenWidth, xdl::xdl_uint screenHeight, xdl::XdevLOpenGL330* openGL) :
		m_screenWidth(screenWidth),
		m_screenHeight(screenHeight),
		m_openGL(openGL),
		m_numberOfTextures(0),
		m_fontSize(0.0f),
		m_newLine(0.0f),
		m_unitX(1.0f),
		m_unitY(1.0f),
		createTextureFromFile(nullptr) {

		//
		// Create dummy glyph for unknown glyphs. This will be returned if the method getGlyphMetric()
		// fails.
		//
		m_dummyGlyph.width		= (xdl::xdl_float)m_screenWidth/20.0f;
		m_dummyGlyph.height		= (xdl::xdl_float)m_screenHeight/20.0f;
		m_dummyGlyph.advance_h	= m_dummyGlyph.width;
		m_dummyGlyph.advance_v	= m_dummyGlyph.height;

		m_dummyGlyph.vertices[0].x = m_dummyGlyph.brearing_x;
		m_dummyGlyph.vertices[0].y = (m_dummyGlyph.height - m_dummyGlyph.brearing_y);
		m_dummyGlyph.vertices[0].s = 0.0f;
		m_dummyGlyph.vertices[0].t = 0.0f;

		m_dummyGlyph.vertices[1].x = m_dummyGlyph.brearing_x;
		m_dummyGlyph.vertices[1].y = m_dummyGlyph.brearing_y;
		m_dummyGlyph.vertices[1].s = 0.0f;
		m_dummyGlyph.vertices[1].t = 1.0f;

		m_dummyGlyph.vertices[2].x = m_dummyGlyph.brearing_x + m_dummyGlyph.width;
		m_dummyGlyph.vertices[2].y = m_dummyGlyph.brearing_y;
		m_dummyGlyph.vertices[2].s = 1.0f;
		m_dummyGlyph.vertices[2].t = 1.0f;

		m_dummyGlyph.vertices[3].x = m_dummyGlyph.brearing_x + m_dummyGlyph.width;
		m_dummyGlyph.vertices[3].y = (m_dummyGlyph.height - m_dummyGlyph.brearing_y);
		m_dummyGlyph.vertices[3].s = 1.0f;
		m_dummyGlyph.vertices[3].t = 0.0f;


	}

	XdevLFontImpl::~XdevLFontImpl() {

	}

	void  XdevLFontImpl::setCreateTextureCallback(createTextureFromFileCallbackFunction function) {
		assert(function && " XdevLFontImpl::setCreateTextureCallback: Parameter not valid.");
		createTextureFromFile = function;
	}

	xdl::xdl_int XdevLFontImpl::importFromFontFile(const xdl::xdl_char* fontInfoFilename) {

		std::vector<std::string> fileNames;

		std::ifstream infile(fontInfoFilename);
		if(infile.is_open()) {
			std::string tmp;
			std::getline(infile, tmp);
			std::getline(infile, tmp);

			std::stringstream ss(tmp);
			std::getline(infile, tmp);
			ss << tmp;
			ss >> m_fontSize;

			ss.str("");
			ss.clear();

			std::getline(infile, tmp);
			ss << tmp;
			ss >> m_numberOfTextures;


			for(auto id = 0; id < m_numberOfTextures; id++) {
				std::string filename;
				std::getline(infile, filename);

				xdl::XdevLTexture* texture = nullptr;

				if(createTextureFromFile) {
					texture = createTextureFromFile(filename.c_str());
				} else {

					std::vector<unsigned char> image;
					unsigned width, height;
					unsigned error = lodepng::decode(image, width, height, filename.c_str());

					// TODO This flipping is neccessary because the library flips the picture up side down.
					std::vector<unsigned char> vflipedimage;
					vflipedimage.reserve(width*height*4);
					vflipedimage.resize(width*height*4);

					for(auto y = 0; y < height; y++) {
						for(auto x = 0; x < width*4; x++) {
							vflipedimage[x + y*width*4] = image[x + (height - y)*width*4];
						}
					}

					m_openGL->createTexture(&texture);

					texture->init(width, height, xdl::XDEVL_RGBA, xdl::XDEVL_RGBA, vflipedimage.data() );
					image.clear();
				}
				if(texture == nullptr) {
					std::cout << "XdevLFontImpl::importFromFontFile: Could not create texture: " << filename << std::endl;
					continue;
				}
				texture->lock();
				texture->setTextureFilter(xdl::XDEVL_TEXTURE_MIN_FILTER, xdl::XDEVL_LINEAR);
				texture->setTextureFilter(xdl::XDEVL_TEXTURE_MAG_FILTER, xdl::XDEVL_LINEAR);
				texture->setTextureWrap(xdl::XDEVL_TEXTURE_WRAP_S, xdl::XDEVL_CLAMP_TO_BORDER);
				texture->setTextureWrap(xdl::XDEVL_TEXTURE_WRAP_T, xdl::XDEVL_CLAMP_TO_BORDER);
				texture->unlock();

				m_textureList.push_back(texture);
		
			}

			calculateGlyphInformation(infile);
			
			return xdl::ERR_OK;
		}
		
		return xdl::ERR_ERROR;

	}

	xdl::xdl_int XdevLFontImpl::createFontFromTexture(const xdl::xdl_char* fontInfoFilename, xdl::XdevLTexture* texture) {

		xdl::XdevLTexture* tx = texture;
		m_textureList.push_back(texture);

		tx->lock();
		tx->setTextureFilter(xdl::XDEVL_TEXTURE_MIN_FILTER, xdl::XDEVL_LINEAR);
		tx->setTextureFilter(xdl::XDEVL_TEXTURE_MAG_FILTER, xdl::XDEVL_LINEAR);
		tx->setTextureWrap(xdl::XDEVL_TEXTURE_WRAP_S, xdl::XDEVL_CLAMP_TO_BORDER);
		tx->setTextureWrap(xdl::XDEVL_TEXTURE_WRAP_T, xdl::XDEVL_CLAMP_TO_BORDER);
		tx->unlock();


		// One unit on the screen for x,y [-1 , 1]
		m_unitX 	=  2.0f/(xdl::xdl_float)m_window->getWidth();
		m_unitY		=  2.0f/(xdl::xdl_float)m_window->getHeight();

		std::ifstream infile(fontInfoFilename);
		if(infile.is_open()) {
			std::string tmp;
			std::getline(infile, tmp);
			std::getline(infile, tmp);

			std::getline(infile, tmp);
			std::stringstream ss(tmp);
			ss >> m_numberOfTextures;
			ss.str("");
			ss.clear();
			std::getline(infile, tmp);
			ss << tmp;
			ss >> m_fontSize;

			xdl::xdl_float numCols 	= (xdl::xdl_float)tx->getWidth()/(xdl::xdl_float)m_fontSize;

			// TODO Using maps to handle id of the glyphs? At the moment it is just a hack.
			while(!infile.eof()) {

				XdevLGlyphMetric gp;
				readLine(infile, gp);

				//
				// TODO This part to find the position of the glyph in the texture is some sort of hack.
				// Make it so that all of the information is the the fontInfo.txt file.
				//
				xdl::xdl_uint idx = gp.id - 32;

				xdl::xdl_float pos_x = idx % (xdl::xdl_int)numCols;
				xdl::xdl_float pos_y = idx / (xdl::xdl_int)numCols;

				xdl::xdl_float ds = 1.0/(xdl::xdl_float)tx->getWidth()*gp.width;
				xdl::xdl_float dt = 1.0/(xdl::xdl_float)tx->getWidth()*gp.height;

				xdl::xdl_float s = 1.0/numCols*pos_x ;
				xdl::xdl_float t = 1.0 - 1.0/numCols*pos_y - (1.0/(xdl::xdl_float)tx->getHeight())*((m_fontSize - gp.height - gp.top));

				//
				// Add an offset of x,y pixel offset to the s,t coordinates.
				//
				xdl::xdl_float offset_x = 0.0/(xdl::xdl_float)tx->getWidth();
				xdl::xdl_float offset_y = 0.0/(xdl::xdl_float)tx->getHeight();


				gp.vertices[0].x = gp.brearing_x;
				gp.vertices[0].y = (gp.height - gp.brearing_y);
				gp.vertices[0].s = s - offset_x;
				gp.vertices[0].t = t - dt - offset_y;

				gp.vertices[1].x = gp.brearing_x;
				gp.vertices[1].y = gp.brearing_y;
				gp.vertices[1].s = s - offset_x;
				gp.vertices[1].t = t + offset_y;

				gp.vertices[2].x = gp.brearing_x + gp.width;
				gp.vertices[2].y = gp.brearing_y;
				gp.vertices[2].s = s + ds + offset_x;
				gp.vertices[2].t = t + offset_y;

				gp.vertices[3].x = gp.brearing_x + gp.width;
				gp.vertices[3].y = (gp.height - gp.brearing_y);
				gp.vertices[3].s = s + ds + offset_x;
				gp.vertices[3].t = t - dt - offset_y;

				//
				// Find maximum value for the new line.
				//
				m_newLine = std::max(m_newLine, gp.height);

				m_glyphMap[gp.id] = gp;

			}
			return xdl::ERR_OK;
		}

		return xdl::ERR_ERROR;
	}

	XdevLGlyphMetric& XdevLFontImpl::readLine(std::ifstream& os, XdevLGlyphMetric& gp) {
		os >> gp.tid
		   >> gp.id
		   >> gp.left
		   >> gp.top
		   >> gp.width
		   >> gp.height
		   >> gp.advance_h
		   >> gp.advance_v
		   >> gp.brearing_x
		   >> gp.brearing_y
           >> gp.vertices[0].s
           >> gp.vertices[0].t
           >> gp.vertices[3].s
           >> gp.vertices[3].t;
			
			return gp;
	}

	void XdevLFontImpl::calculateGlyphInformation(std::ifstream& os) {

		xdl::xdl_float numCols 				= (xdl::xdl_float)m_textureList[0]->getWidth()/m_fontSize;
		xdl::xdl_uint numberOfSegments 	= pow(m_textureList[0]->getWidth()/(xdl::xdl_uint)m_fontSize,2);

		xdl::xdl_uint	count = 0;

		// TODO Using maps to handle id of the glyphs? At the moment it is just a hack.
		while(!os.eof()) {

			XdevLGlyphMetric gp;
			readLine(os, gp);

			//
			// Get the info for the glyph.
			//

			xdl::XdevLTexture* tx = m_textureList[gp.tid];

			//
			// Add an offset of x,y pixel offset to the s,t coordinates.
			// TODO This value is a hack. Is done because of the Shadow Effect.
			xdl::xdl_float offset_x = 4.0/(xdl::xdl_float)tx->getWidth();
			xdl::xdl_float offset_y = 4.0/(xdl::xdl_float)tx->getHeight();

			//
			// Add an offset of x,y pixel offset to the x,y coordinates.
			// What this doesn is expanding the size of the quad in the same amound
			//
			xdl::xdl_float offset_sx = 0.0;
			xdl::xdl_float offset_sy = 0.0;

            //
            // Convert the pixel units into texture coordinate units
            // 
			xdl::xdl_float s1 = (1.0f/(xdl::xdl_float)tx->getWidth())*gp.vertices[0].s;
			xdl::xdl_float t1 = (1.0f/(xdl::xdl_float)tx->getWidth())*gp.vertices[0].t;

			xdl::xdl_float s2 = (1.0f/(xdl::xdl_float)tx->getWidth())*gp.vertices[3].s;
			xdl::xdl_float t2 = (1.0f/(xdl::xdl_float)tx->getWidth())*gp.vertices[3].t;

	
			gp.vertices[0].x = gp.brearing_x - offset_sx;
			gp.vertices[0].y = (gp.height - gp.brearing_y) - offset_sy;
			gp.vertices[0].s = s1;
			gp.vertices[0].t = t2;

			gp.vertices[1].x = gp.brearing_x - offset_sx;
			gp.vertices[1].y = gp.brearing_y + offset_sy;
			gp.vertices[1].s = s1;
			gp.vertices[1].t = t1;

			gp.vertices[2].x = gp.brearing_x + gp.width + offset_sx;
			gp.vertices[2].y = gp.brearing_y + offset_sy;
			gp.vertices[2].s = s2;
			gp.vertices[2].t = t1;

			gp.vertices[3].x = gp.brearing_x + gp.width + offset_sx;
			gp.vertices[3].y = (gp.height - gp.brearing_y) - offset_sy;
			gp.vertices[3].s = s2;
			gp.vertices[3].t = t2;


			//
			// Find maximum value for the new line.
			//
			m_newLine = std::max(m_newLine, gp.height);

			//
			// Store that glyph in the map
			//
			m_glyphMap[gp.id] = gp;

			count++;

		}
	}


	XdevLGlyphMetric& XdevLFontImpl::getGlyphMetric(xdl::xdl_uint32 unicode) {

		auto ib = m_glyphMap.find(unicode);
		if(ib == m_glyphMap.end()) {
			return m_dummyGlyph;
		}

		return ib->second;
	}

	xdl::xdl_float  XdevLFontImpl::getFontSize() {
		return m_fontSize;
	}

	xdl::xdl_float XdevLFontImpl::getNewLineSize() {
		return m_newLine;
	}

	xdl::XdevLTexture* XdevLFontImpl::getTexture(xdl::xdl_uint idx) {
		assert( (idx < m_textureList.size()) && "XdevLFontImpl::getTexture: Specified index out of range.");
		return m_textureList[idx];
	}

	xdl::xdl_uint XdevLFontImpl::getNumberOfTextures() {
		return m_textureList.size();
	}

}

//	wchar_t tmp1 [] = {	32, 33, 34, 35, 36, 37, 38, 39, 10,
//											40, 41, 42, 43, 44, 45, 46, 47, 10,
//											48, 49, 50, 51, 52, 53, 54, 55, 10,
//											56, 57, 58, 59, 60, 61, 62, 63, 10,
//											64, 65, 66, 67, 68, 69, 70, 71, 10,
//											72, 73, 74, 75, 76, 77, 78, 79, 10,
//											80, 81, 82, 83, 84, 85, 86, 87, 10,
//											88, 89, 90, 91, 92, 93, 94, 95};
//	message = tmp1;


//	wchar_t tmp2 [] = {	96,   97,  98,  99, 100, 101, 102, 103, 10,
//											104, 105, 106, 107, 108, 109, 110, 111, 10,
//											112, 113, 114, 115, 116, 117, 118, 119, 10,
//											120, 121, 122, 123, 124, 125, 126, 160, 10,
//											161, 162, 163, 164, 165, 166, 167, 168, 10,
//											169, 170, 171, 172, 173, 174, 175, 176, 10,
//											177, 178, 179, 180, 181, 182, 183, 184, 10,
//											185, 186, 187, 188, 189, 190, 191, 192};
//	message = tmp2;


//	wchar_t tmp3 [] = {	193, 194, 195, 196, 197, 198, 199, 200, 10,
//											201, 202, 203, 204, 205, 206, 207, 208, 10,
//											209, 210, 211, 212, 213, 214, 215, 216, 10,
//											217, 218, 219, 220, 221, 222, 223, 224, 10,
//											225, 226, 227, 228, 229, 230, 231, 232, 10,
//											233, 234, 235, 236, 237, 238, 239, 240, 10,
//											241, 242, 243, 244, 245, 246, 247, 248, 10,
//											249, 250, 251, 252, 253, 254, 255, 262};
//	message = tmp3;
