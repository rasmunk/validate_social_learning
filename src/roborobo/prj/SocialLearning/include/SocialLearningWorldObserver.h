/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 */


#ifndef SOCIALLEARNINGWORLDOBSERVER_H
#define SOCIALLEARNINGWORLDOBSERVER_H

#include "RoboroboMain/common.h"
#include "RoboroboMain/roborobo.h"

#include "Observers/WorldObserver.h"

class World;

class SocialLearningWorldObserver : public WorldObserver
{
	protected:
		
	public:
		SocialLearningWorldObserver( World *__world );
		~SocialLearningWorldObserver();
				
		void reset();
		void step();
		
};

#endif

