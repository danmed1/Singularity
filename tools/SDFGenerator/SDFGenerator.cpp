#include <SignedDistanceFieldGenerator.h>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

int main(int argc, char **argv) {

	vector<string> Arguments(argv, argv+argc);

	std::string inputFilename = "resources/fonts/font.png";
	std::string outputFilename = "resources/fonts/font_super.png";
	unsigned int rescaleWidth = 256;
	unsigned int rescaleHeight = 256;

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
		}
		if( *argument == "-rs") {
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
			cout << "  -h               : Print out this help.\n";
			cout << "  -if              : The filename of the input image.\n";
			cout << "  -of              : The filename of the output image.\n";
			cout << "  -rs width height : Rescaling the image.\n";
			return 0;
		}

		argument++;
	}

	cout << "Signed Distance Field Generator Version 1.0.0 by Cengiz Terzibas\n" 	<< endl;


	soan::utils::SignedDistanceFieldGenerator sdfgenerator;
	sdfgenerator.generateFromFile(inputFilename.c_str(), outputFilename.c_str(), rescaleWidth, rescaleHeight);

	cout << "File exported to: " << outputFilename << std::endl;
	
	return 0;
}
