#pragma once

#include "ddd/ILua.h"

namespace ddd
{

	class Actor 
		: public ILua
	{
	public:
		Actor();
		virtual ~Actor();

		void update();

	protected:
		
		virtual void onInit();
		virtual void onRelease();
		virtual void onUpdate();

	private:
	};
}