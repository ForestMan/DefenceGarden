#include "ddd/LogicLevelComponent.h"
#include "ddd/Actor.h"

namespace ddd
{
	//-------------------------------------------------------------------------

	LogicLevelComponent::LogicLevelComponent()
	{
	}
	
	//-------------------------------------------------------------------------

	LogicLevelComponent::~LogicLevelComponent()
	{
	}

	//-------------------------------------------------------------------------

	void LogicLevelComponent::update( LevelWindow* owner )
	{
	}

	//-------------------------------------------------------------------------

	void LogicLevelComponent::onCreate()
	{
		removeAllActors();
	}
	
	//-------------------------------------------------------------------------

	void LogicLevelComponent::onInit()
	{
	}
	
	//-------------------------------------------------------------------------

	void LogicLevelComponent::onRelease()
	{
		removeAllActors();
	}

	//-------------------------------------------------------------------------

	void LogicLevelComponent::destroyActor( const unsigned long actorID )
	{
		Actor* destroy(removeActor(actorID));
		destroy->release();
		delete destroy;
	}
	
	//-------------------------------------------------------------------------

	void LogicLevelComponent::destroyAllActors()
	{
		for (size_t i=0;i<getActorCount();i++)
		{
			assert(0!= actorArray_[i]);
			actorArray_[i]->release();
			delete actorArray_[i];
			actorArray_[i] = 0;
		}
		removeAllActors();
	}

	//-------------------------------------------------------------------------
}