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

	private:
		
		inline const bool hasEntity( const unsigned long entityID );

	private:
		std::map< unsigned long, TEntity* >entityMap_;
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
	inline const bool Container< TEntity >::hasEntity( const unsigned long entityID )
	{
		return 0 != entityMap_[ entityID ];
	}

	//-------------------------------------------------------------------------

}