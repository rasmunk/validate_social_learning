/*
 * SocialLearningConfigurationLoader.h
 */

#ifndef SOCIALLEARNINGCONFIGURATIONLOADER_H
#define	SOCIALLEARNINGCONFIGURATIONLOADER_H

#include "Config/ConfigurationLoader.h"
#include "Evaluators/Evaluator.h"

class SocialLearningConfigurationLoader : public ConfigurationLoader
{
	public:
		SocialLearningConfigurationLoader();
		~SocialLearningConfigurationLoader();

		WorldObserver *make_WorldObserver(World* wm) ;
		RobotWorldModel *make_RobotWorldModel();
		AgentObserver *make_AgentObserver(RobotWorldModel* wm) ;
		Controller *make_Controller(RobotWorldModel* wm);
		Evaluator *make_Evaluator(Controller *controller);
		Evaluator *make_Evaluator_with_sharedStorage(Controller *controller, std::map<std::string, std::shared_ptr<SharedDataStructure>> &sharedStorage);
		Evaluator *make_Evaluator_with_sharedStorage_n_barrier(Controller *controller, std::map<std::string, std::shared_ptr<SharedDataStructure>> &sharedStorage, Barrier *evolutionBarrier);
};

#endif
