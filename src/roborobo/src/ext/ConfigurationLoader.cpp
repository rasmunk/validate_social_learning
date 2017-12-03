#include "Config/ConfigurationLoader.h"
#include <string.h>

#include "Config/SocialLearningConfigurationLoader.h"
//###DO-NOT-DELETE-THIS-LINE###TAG:INCLUDE###//


ConfigurationLoader::ConfigurationLoader()
{
	//nothing to do
}

ConfigurationLoader::~ConfigurationLoader()
{
	//nothing to do
}

ConfigurationLoader* ConfigurationLoader::make_ConfigurationLoader (std::string configurationLoaderObjectName)
{
	if (0)
	{
		// >>> Never reached
	}
#if defined PRJ_SOCIALLEARNING || !defined MODULAR
	else if (configurationLoaderObjectName == "SocialLearningConfigurationLoader" )
	{
		return new SocialLearningConfigurationLoader();
	}
#endif
    //###DO-NOT-DELETE-THIS-LINE###TAG:SWITCH###//
	else
	{
		return NULL;
	}

}
