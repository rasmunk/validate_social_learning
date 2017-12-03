/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 */
 
#ifndef SOCIALLEARNINGAGENTOBSERVER_H
#define SOCIALLEARNINGAGENTOBSERVER_H 

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"
#include "WorldModels/RobotWorldModel.h"
#include "Observers/AgentObserver.h"

class SocialLearningAgentObserver : public AgentObserver
{
	public:
		SocialLearningAgentObserver();
		SocialLearningAgentObserver( RobotWorldModel *__wm );
		~SocialLearningAgentObserver();
				
		void reset();
		void step();
};


#endif

