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
#include <sstream>
#include <vector>
#include <cmath>

using namespace std;

int main(int argc, char **argv) {
	vector<string> Arguments(argv, argv+argc);

//	std::string inputFilename = "resources/fonts/Roboto-Regular.ttf";
	std::string inputFilename = "resources/fonts/ProggyClean.ttf";
	std::string outputFilename = "resources/fonts/default.png";
//	unsigned int textureSize 			= 16384;
//	unsigned int fontSize 				= 1600;
	unsigned int textureSize 			= 512;
	unsigned int fontSize 				= 16;
//	unsigned int textureSize 			= 512;
//	unsigned int fontSize 				= 50;
	unsigned int startingLetter		= 32;
	unsigned int numberOfLetters 	= 1<<16;
	unsigned int rescaleWidth 		= 512;
	unsigned int rescaleHeight 		= 512;
	unsigned int gapX							= 0;
	unsigned int gapY							= 0;


//	unsigned int textureSize 			= 512;
//	unsigned int fontSize 				= 16;
//	unsigned int startingLetter		= 32;
//	unsigned int numberOfLetters 	= 1<<16;
//	unsigned int rescaleWidth 		= 512;
//	unsigned int rescaleHeight 		= 512;
//	unsigned int gapX							= 0;
//	unsigned int gapY							= 0;


	auto argument(Arguments.begin());

	while(argument != Arguments.end()) {

		if( *argument == "-if") {
			argument++;
			if(argument != Arguments.end()) {
				inputFilename = *argument;
			} else {
				cerr << "Number of arguments wrong.\n";
				return -1;
			}
		} else if(*argument == "-of") {
			argument++;
			if(argument != Arguments.end()) {
				outputFilename = *argument;
			} else {
				cerr << "Number of arguments wrong.\n";
				return -1;
			}
		} else if( *argument == "-ts") {
			argument++;
			if(argument != Arguments.end()) {
				stringstream ss;
				ss << *argument;
				ss >> textureSize;
			} else {
				cerr << "Number of arguments wrong.\n";
				return -1;
			}
		} else if(*argument == "-fs") {
			argument++;
			if(argument != Arguments.end()) {
				stringstream ss;
				ss << *argument;
				ss >> fontSize;
			} else {
				cerr << "Number of arguments wrong.\n";
				return -1;
			}
		} else if(*argument == "-nl") {
			argument++;
			if(argument != Arguments.end()) {
				stringstream ss;
				ss << *argument;
				ss >> numberOfLetters;
			} else {
				cerr << "Number of arguments wrong.\n";
				return -1;
			}
		} else if(*argument == "-sl") {
			argument++;
			if(argument != Arguments.end()) {
				stringstream ss;
				ss << *argument;
				ss >> startingLetter;
			} else {
				cerr << "Number of arguments wrong.\n";
				return -1;
			}
		} else if(*argument == "-gap_width") {
			argument++;
			if(argument != Arguments.end()) {
				stringstream ss;
				ss << *argument;
				ss >> gapX;
			} else {
				cerr << "Number of arguments wrong.\n";
				return -1;
			}
		}  else if(*argument == "-gap_height") {
			argument++;
			if(argument != Arguments.end()) {
				stringstream ss;
				ss << *argument;
				ss >> gapY;
			} else {
				cerr << "Number of arguments wrong.\n";
				return -1;
			}
		}   else if( *argument == "-rs") {
			argument++;
			if(argument != Arguments.end()) {
				stringstream ss;
				ss << *argument;
				ss >> rescaleWidth;
				ss.str("");
				ss.clear();
				ss << *argument;
				ss >> rescaleHeight;
			} else {
				cerr << "Number of arguments wrong.\n";
				return -1;
			}
		} else if(*argument == "-h") {
			cout << "Texture Font Creator Version 1.0.0 by Cengiz Terzibas\n" 	<< endl;
			cout << " -h                : Print out this help.\n";
			cout << " -ts               : The texture size for the fonts in pixels.\n";
			cout << " -fs               : The font size in pixels.\n";
			cout << " -nl               : The number of letters to write into the texture.\n";
			cout << " -sl               : The starting letter.\n";
			cout << " -rs  width height : Rescaling the image.\n";
			cout << " -gap_width width  : Gab in horizontal direction between glyph bitmaps.\n";
			cout << " -gap_height height: Gab in vertical direction between glyph bitmaps.\n";
			return 0;
		}

		argument++;
	}

	cout << "Texture Font Creator Version 1.0.0 by Cengiz Terzibas\n" 	<< endl;
	cout << "Size              : " << textureSize << endl;
	cout << "Font Size         : " << fontSize << endl;
	cout << "Starting Letter   : " << startingLetter << endl;
	cout << "Number of letters : " << numberOfLetters << endl;
	cout << "Rescale to        : " << rescaleWidth << "x" << rescaleHeight << std::endl;
	soan::utils::TrueTypeFont ttf;
	ttf.setGapBetweenGlyphBitmap(gapX, gapY);
	ttf.create(inputFilename.c_str(),outputFilename.c_str(), startingLetter, numberOfLetters, fontSize, textureSize,rescaleWidth,rescaleHeight);
//	ttf.create(inputFilename.c_str(), outputFilename.c_str(), 16, 512);
	return 0;
}
