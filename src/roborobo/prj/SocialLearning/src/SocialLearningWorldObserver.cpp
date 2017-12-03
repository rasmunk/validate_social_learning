/**
 * @author Nicolas Bredeche <nicolas.bredeche@upmc.fr>
 */


#include "SocialLearning/include/SocialLearningWorldObserver.h"

#include "World/World.h"


SocialLearningWorldObserver::SocialLearningWorldObserver( World *__world ) : WorldObserver( __world )
{
	_world = __world;
}

SocialLearningWorldObserver::~SocialLearningWorldObserver()
{
	// nothing to do.
}

void SocialLearningWorldObserver::reset()
{
	// nothing to do.
}

void SocialLearningWorldObserver::step()
{
	// nothing to do.
}
