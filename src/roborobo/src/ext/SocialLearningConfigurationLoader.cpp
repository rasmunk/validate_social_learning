#if defined PRJ_SOCIALLEARNING || !defined MODULAR

#include "Config/SocialLearningConfigurationLoader.h"

#include "SocialLearning/include/SocialLearningWorldObserver.h"
#include "SocialLearning/include/SocialLearningAgentObserver.h"
#include "SocialLearning/include/SocialLearningController.h"
#include "SocialLearning/include/SocialLearningRobotEvaluator.h"

SocialLearningConfigurationLoader::SocialLearningConfigurationLoader()
{
	// create the single instance of Agent-World Interface.
}

SocialLearningConfigurationLoader::~SocialLearningConfigurationLoader()
{
	//nothing to do
}

WorldObserver* SocialLearningConfigurationLoader::make_WorldObserver(World* wm)
{
	return new SocialLearningWorldObserver(wm);
}

RobotWorldModel* SocialLearningConfigurationLoader::make_RobotWorldModel()
{
	return new RobotWorldModel();
}

AgentObserver* SocialLearningConfigurationLoader::make_AgentObserver(RobotWorldModel* wm)
{
	return new SocialLearningAgentObserver(wm);
}

Controller* SocialLearningConfigurationLoader::make_Controller(RobotWorldModel* wm)
{
	return new SocialLearningController(wm);
}

Evaluator* SocialLearningConfigurationLoader::make_Evaluator(Controller *controller)
{
	return new SocialLearningRobotEvaluator(controller);
}

Evaluator* SocialLearningConfigurationLoader::make_Evaluator_with_sharedStorage_n_barrier(Controller *controller, std::map<std::string, std::shared_ptr<SharedDataStructure>> &sharedStorage, Barrier *evolutionBarrier)
{
	return new SocialLearningRobotEvaluator(controller, sharedStorage, evolutionBarrier);
}

#endif
