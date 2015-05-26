#include <XdevL.h>
#include <XdevLPlugin.h>
#include <XdevLFileSystem/XdevLFileSystem.h>


int main(int argc, char **argv) {

	xdl::XdevLString recursiveSearchFolder(".");
	std::vector<std::string> argumentsList(argv, argv+argc);
	xdl::xdl_bool recursive = xdl::xdl_false;
	xdl::XdevLString pattern("");

	xdl::xdl_bool searchForFiles = xdl::xdl_false;
	xdl::xdl_bool containsFile = xdl::xdl_false;

	for(auto argument = argumentsList.begin(); argument != argumentsList.end(); ++argument) {
		if(*argument == "-h") {
			std::cout <<
			          "Filo (c) 2015 Cengiz Terzibas\n\n" <<
					  "Usage : [options] foldername\n" <<
					  "Options: \n" << 
			          "  -h    : This help.\n" <<
			          "  -f    : Search for files.\n" <<
					  "  -cf   : Contails file in folders.\n" <<
			          "  -r    : Search recursive.\n" <<
			          "  -p    : Pattern for file search." << std::endl;
			return 0;

		} else if(*argument == "-cf") {
			containsFile = xdl::xdl_true;
		} else if(*argument == "-f") {
			searchForFiles = xdl::xdl_true;
		} else if(*argument == "-p") {
			++argument;
			pattern = xdl::XdevLString(*argument);
		} else if(*argument == "-r") {
			recursive = xdl::xdl_true;
		} else {
			recursiveSearchFolder = xdl::XdevLString(*argument);
		}
	}

	xdl::XdevLPluginDescriptor pluginDescriptor(xdl::XdevLPluginName("XdevLFileSystem"), xdl::XdevLVersion(1, 0, 0));
	xdl::XdevLModuleDescriptor moduleDescriptor(xdl::XdevLModuleName("XdevLDirectory"), xdl::XdevLVersion(1, 0, 0));

	xdl::IPXdevLDirectory directory = xdl::createModule<xdl::IPXdevLDirectory>(pluginDescriptor, moduleDescriptor);
	if(directory == nullptr) {
		return -1;
	}

	std::vector<xdl::XdevLString> tmp;
	if(searchForFiles) {
		tmp = directory->getAllFiles(recursiveSearchFolder, recursive, pattern);
	} else if(containsFile) {
		tmp = directory->getAllDirectoriesContainsFile(recursiveSearchFolder, recursive, pattern);
	} else {
		tmp = directory->getAllDirectories(recursiveSearchFolder, recursive);
	}

	for(auto& items : tmp) {
		std::cout << items << std::endl;
	}


	return 0;
}
