#pragma once

#include <map>
#include "ddd/Types.h"

namespace ddd
{
	
	template< class TEntity >
	class Container
	{
	public:
		
		void addEntity( TEntity& game );
		inline TEntity& getEntity( const unsigned long entityID );
		inline void removeEntity( const unsigned long entityID );
		inline void removeAllEntity();

		inline TEntity* begin();
		inline TEntity* getNextEntity();

		inline typename std::map< unsigned long, TEntity* >::iterator beginContainer();
		inline typename std::map< unsigned long, TEntity* >::iterator endContainer();
	private:
		
		inline const bool hasEntity( const unsigned long entityID );

	private:
		typedef std::map< unsigned long, TEntity* > ContainerMap;

		ContainerMap entityMap_;
		typename std::map< unsigned long, TEntity* >::iterator iterContainer_;
	};

	//-------------------------------------------------------------------------

	template< class TEntity >
	void Container< TEntity >::addEntity( TEntity& game )
	{
		assert( !hasEntity( game.getID() ) );
		entityMap_[ game.getID() ] = &game;
	}

	//-------------------------------------------------------------------------
	
	template< class TEntity >
	inline TEntity& Container< TEntity >::getEntity( const unsigned long entityID )
	{
		assert( hasEntity( entityID ) );
		return *entityMap_[ entityID ];
	}

	//-------------------------------------------------------------------------

	template< class TEntity >
	inline void Container< TEntity >::removeEntity( const unsigned long entityID )
	{
		assert( hasEntity( entityID ) );
		entityMap_.erase( entityID );
	}

	//-------------------------------------------------------------------------

	template< class TEntity >
	inline void Container< TEntity >::removeAllEntity()
	{
		entityMap_.clear();
	}

	//-------------------------------------------------------------------------

	template< class TEntity >
	inline TEntity* Container< TEntity >::begin()
	{
		iterContainer_ = entityMap_.begin();
		return iterContainer_ == entityMap_.end() ? 0 : iterContainer_->second;
	}

	//-------------------------------------------------------------------------

	template< class TEntity >
	inline TEntity* Container< TEntity >::getNextEntity()
	{
		iterContainer_++;
		if(entityMap_.end() == iterContainer_)
		{
			return 0;
		}else
		{
			return iterContainer_->second;
		}
	}

	//-------------------------------------------------------------------------

	template< class TEntity >
	inline const bool Container< TEntity >::hasEntity( const unsigned long entityID )
	{
		return 0 != entityMap_[ entityID ];
	}

	//-------------------------------------------------------------------------

}