#include <XdevL.h>
#include <XdevLPlugin.h>
#include <XdevLFileSystem/XdevLFileSystem.h>

int main(int argc, char **argv)
{
	xdl::XdevLPluginDescriptor pluginDescriptor(xdl::XdevLPluginName("XdevLFileSystem"), xdl::XdevLVersion(1, 0, 0));
	xdl::XdevLModuleDescriptor moduleDescriptor(xdl::XdevLModuleName("XdevLDirectory"), xdl::XdevLVersion(1, 0, 0));
	
	xdl::IPXdevLDirectory directory = xdl::createModule<xdl::IPXdevLDirectory>(pluginDescriptor, moduleDescriptor);
	if(directory == nullptr) {
		return -1;
	}
	
	if(directory->open(STRING(argv[1])) == xdl::ERR_ERROR) {
		std::cerr << "Could not open folder: " << argv[1] << std::endl;
		return -1;
	}
	
	std::vector<xdl::XdevLDirectoryEntity> tmp = directory->getEntities();
	for(auto& entity : tmp) {
		std::cout << "Type: " << entity.getTypeAsString() << " : " << entity.getName() << std::endl;
		if(entity.getType() == xdl::XdevLDirectoryEntity::Type::DIRECTORY) {

		}
	}
	
	
    return 0;
}
