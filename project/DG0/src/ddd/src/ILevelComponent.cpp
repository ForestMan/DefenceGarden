#include "ddd/ILevelComponent.h"
#include "ddd/Types.h"

namespace ddd
{
	//-------------------------------------------------------------------------

	ILevelComponent::ILevelComponent()
		: create_( false )
		, init_( false )
	{
	}

	//-------------------------------------------------------------------------

	ILevelComponent::~ILevelComponent()
	{
	}

	//-------------------------------------------------------------------------

	void ILevelComponent::create()
	{
		assert( !isCreate() );
		assert( !isInit() );
		onCreate();
		setCreate( true );
	}

	//-------------------------------------------------------------------------

	void ILevelComponent::init()
	{
		assert( isCreate() );
		assert( !isInit() );
		onInit();
		setInit( true );
	}

	//-------------------------------------------------------------------------

	void ILevelComponent::release()
	{
		assert( isCreate() );
		assert( isInit() );
		onRelease();
		setCreate( false );
		setInit( false );
	}

	//-------------------------------------------------------------------------
}