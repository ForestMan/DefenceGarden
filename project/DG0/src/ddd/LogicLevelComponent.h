#pragma once

#include <vector>

#include "ddd/ILevelComponent.h"
#include "ddd/Container.h"

namespace ddd
{
	class LevelWindow;
	class Actor;

	class LogicLevelComponent
		: public ILevelComponent
	{
	public:
		
		LogicLevelComponent();
		virtual ~LogicLevelComponent();

		void update( LevelWindow* owner );

		inline void addActor( Actor& actor );
		inline Actor& getActor( const unsigned long actorID )const;
		inline const Actor& getActorConst( const unsigned long actorID )const;
		inline Actor* removeActor(  const unsigned long actorID  );
		void destroyActor(  const unsigned long actorID  );
		void destroyAllActors();
		inline void removeAllActors();

		inline const size_t getActorCount()const;

	private:
		
		virtual void onCreate();
		virtual void onInit();
		virtual void onRelease();

	private:

		std::vector< Actor* > actorArray_;
	};

	//-------------------------------------------------------------------------

	inline void LogicLevelComponent::addActor( Actor& actor )
	{
		actorArray_.push_back( &actor );
	}

	//-------------------------------------------------------------------------

	inline Actor& LogicLevelComponent::getActor( const unsigned long actorID )const
	{
		assert( actorID < actorArray_.size() );
		assert( 0 != actorArray_[ actorID ] );
		return *actorArray_[ actorID ];
	}

	//-------------------------------------------------------------------------

	inline const Actor& LogicLevelComponent::getActorConst( const unsigned long actorID )const
	{
		assert( actorID < actorArray_.size() );
		assert( 0 != actorArray_[ actorID ] );
		return *actorArray_[ actorID ];
	}

	//-------------------------------------------------------------------------

	inline Actor* LogicLevelComponent::removeActor( const unsigned long actorID )
	{
		assert( actorID < actorArray_.size() );
		assert( 0 != actorArray_[ actorID ] );
		assert( 0 != actorArray_[ actorArray_.size() - 1 ] );
		Actor* rem = actorArray_[ actorID ];
		actorArray_[ actorID ] = actorArray_[ actorArray_.size() - 1 ];
		actorArray_[ actorArray_.size() - 1 ] = 0;
		actorArray_.pop_back();
		return rem;
	}

	//-------------------------------------------------------------------------

	inline void LogicLevelComponent::removeAllActors()
	{
		actorArray_.clear();
	}

	//-------------------------------------------------------------------------

	inline const size_t LogicLevelComponent::getActorCount()const
	{
		return actorArray_.size();
	}

	//-------------------------------------------------------------------------
}