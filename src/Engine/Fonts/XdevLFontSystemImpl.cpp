#include "XdevLFontSystemImpl.h"
#include "XdevLFontImpl.h"
#include "lodepng.h"

#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cassert>


namespace soan {

	XdevLFontSystemImpl::XdevLFontSystemImpl() {
	}

	XdevLFontSystemImpl::~XdevLFontSystemImpl() {
	}

	xdl::xdl_int XdevLFontSystemImpl::init(xdl::xdl_uint screenWidth, xdl::xdl_uint screenHeight, xdl::XdevLOpenGL330* opengl) {
		openGL = opengl;
		return xdl::ERR_OK;
	}

	void XdevLFontSystemImpl::setCreateTextureCallback(XdevLFontSystem::createTextureFromFileCallbackFunction function) {
		assert(function && " XdevLFontImpl::setCreateTextureCallback: Parameter not valid.");
		createTextureFromFile = function;
	}

	XdevLFont* XdevLFontSystemImpl::createFromFontFile(const xdl::xdl_char* fontInfoFilename) {
		assert(openGL && " XdevLFontImpl::createFromFontFile: XdevLFontSystem not initialized.");

		XdevLFontImpl* font = new XdevLFontImpl();

		std::vector<std::string> fileNames;
		xdl::xdl_uint numberOfTextures = 1;
		xdl::xdl_uint fontSize = 0;
		xdl::xdl_uint newLineSize = 0;

		std::ifstream infile(fontInfoFilename);
		if(infile.is_open()) {
			std::string tmp;
			std::getline(infile, tmp);
			std::getline(infile, tmp);

			std::stringstream ss(tmp);
			std::getline(infile, tmp);
			ss << tmp;
			ss >> fontSize;
			font->setFontSize(fontSize);

			ss.str("");
			ss.clear();

			std::getline(infile, tmp);
			ss << tmp;
			ss >> numberOfTextures;

			xdl::XdevLTexture* texture = nullptr;

			for(auto id = 0; id < numberOfTextures; id++) {
				std::string filename;
				std::getline(infile, filename);

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

					openGL->createTexture(&texture);

					texture->init(width, height, xdl::XDEVL_RGBA, xdl::XDEVL_RGBA, vflipedimage.data());
					image.clear();
				}
				if(texture == nullptr) {
					std::cout << "XdevLFontImpl::importFromFontFile: Could not create texture: " << filename << std::endl;
					assert(texture && "FontTexture not created.");
				}
				texture->lock();
				texture->setTextureFilter(xdl::XDEVL_TEXTURE_MIN_FILTER, xdl::XDEVL_NEAREST);
				texture->setTextureFilter(xdl::XDEVL_TEXTURE_MAG_FILTER, xdl::XDEVL_NEAREST);
				texture->setTextureWrap(xdl::XDEVL_TEXTURE_WRAP_S, xdl::XDEVL_CLAMP_TO_EDGE);
				texture->setTextureWrap(xdl::XDEVL_TEXTURE_WRAP_T, xdl::XDEVL_CLAMP_TO_EDGE);
				texture->unlock();

				font->addFontTexture(texture);

			}

			calculateGlyphInformation(font, infile);

			return font;
		}

		return nullptr;

	}


	XdevLFont* XdevLFontSystemImpl::createFontFromTexture(const xdl::xdl_char* fontInfoFilename, xdl::XdevLTexture* texture) {
		assert(openGL && " XdevLFontImpl::createFontFromTexture: XdevLFontSystem not initialized.");
		
		XdevLFontImpl* font = new XdevLFontImpl();

		xdl::XdevLTexture* tx = texture;
		font->addFontTexture(texture);

		tx->lock();
		tx->setTextureFilter(xdl::XDEVL_TEXTURE_MIN_FILTER, xdl::XDEVL_NEAREST);
		tx->setTextureFilter(xdl::XDEVL_TEXTURE_MAG_FILTER, xdl::XDEVL_NEAREST);
		tx->setTextureWrap(xdl::XDEVL_TEXTURE_WRAP_S, xdl::XDEVL_CLAMP_TO_EDGE);
		tx->setTextureWrap(xdl::XDEVL_TEXTURE_WRAP_T, xdl::XDEVL_CLAMP_TO_EDGE);
		tx->unlock();


		// One unit on the screen for x,y [-1 , 1]
		font->setUnitX(2.0f/(xdl::xdl_float)screenWidth);
		font->setUnitY(2.0f/(xdl::xdl_float)screenHeight);

		xdl::xdl_uint numberOfTextures = 1;
		xdl::xdl_uint fontSize = 0;
		xdl::xdl_uint newLineSize = 0;

		std::ifstream infile(fontInfoFilename);
		if(infile.is_open()) {
			std::string tmp;
			std::getline(infile, tmp);
			std::getline(infile, tmp);

			std::getline(infile, tmp);
			std::stringstream ss(tmp);
			ss >> numberOfTextures;
			ss.str("");
			ss.clear();

			// Get font size.
			std::getline(infile, tmp);
			ss << tmp;
			ss >> fontSize;

			font->setFontSize(fontSize);

			xdl::xdl_float numCols 	= (xdl::xdl_float)tx->getWidth()/(xdl::xdl_float)fontSize;

			// TODO Using maps to handle id of the glyphs? At the moment it is just a hack.
			while(!infile.eof()) {

				XdevLGlyphMetric gp;
				readLine(infile, gp);

				//
				// TODO This part to find the position of the glyph in the texture is some sort of hack.
				// Make it so that all of the information is the the fontInfo.txt file.
				//
				xdl::xdl_uint idx = gp.character_code - 32;

				xdl::xdl_float pos_x = idx % (xdl::xdl_int)numCols;
				xdl::xdl_float pos_y = idx / (xdl::xdl_int)numCols;

				xdl::xdl_float ds = 1.0/(xdl::xdl_float)tx->getWidth()*gp.width;
				xdl::xdl_float dt = 1.0/(xdl::xdl_float)tx->getWidth()*gp.height;

				xdl::xdl_float s = 1.0/numCols*pos_x ;
				xdl::xdl_float t = 1.0 - 1.0/numCols*pos_y - (1.0/(xdl::xdl_float)tx->getHeight())*((fontSize - gp.height - gp.top));

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
				font->setNewLineSize(std::max(font->getNewLineSize(), gp.height));

				font->addGlyph(gp);

			}
			return font;
		}

		return nullptr;
	}

	XdevLGlyphMetric& XdevLFontSystemImpl::readLine(std::ifstream& os, XdevLGlyphMetric& gp) {
		os >> gp.tid
		   >> gp.character_code
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

	void XdevLFontSystemImpl::calculateGlyphInformation(XdevLFontImpl* font, std::ifstream& os) {

		xdl::xdl_uint	count = 0;

		// TODO Using maps to handle id of the glyphs? At the moment it is just a hack.
		while(!os.eof()) {

			XdevLGlyphMetric gp;
			readLine(os, gp);

			//
			// Get the info for the glyph.
			//

			xdl::XdevLTexture* tx = font->getFontTexture(gp);

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
			font->setNewLineSize(std::max(font->getNewLineSize(), gp.height));

			//
			// Store that glyph in the map
			//
			font->addGlyph(gp);

			count++;

		}
	}

}
