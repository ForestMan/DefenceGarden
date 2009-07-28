#pragma once

#include "ddd/ILua.h"
#include "ddd/Container.h"

namespace ddd
{
	class Actor;

	class Level 
		: public ILua
		, public Container< Actor >
	{
	public:
		Level();
		virtual ~Level();

	protected:
	private:
	};
}