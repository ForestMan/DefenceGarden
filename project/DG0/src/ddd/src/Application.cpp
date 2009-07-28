#include "ddd/Application.h"

#include "settings.h"

#include <pf/windowmanager.h>
#include <pf/event.h>
#include <pf/script.h>

#include "ddd/Game.h"
#include "ddd/Factory.h"

namespace ddd
{
	void Application::initApplication( TLuaTable* luaTable )
	{
		ddd::Application::get_mutable_instance().init( luaTable );
	}

	//-------------------------------------------------------------------------

	void Application::initPlayground(TPlatform* pPlatform)
	{
		setFactory( 0 );
		initGameStates();
		initWindows(pPlatform);
	}

	//-------------------------------------------------------------------------

	void Application::run( TPlatform* pPlatform )
	{
		TWindowManager * wm = TWindowManager::GetInstance();
		// Start the Lua GUI script; this script will never exit
		// in a typical Playground application.
		ScriptRegisterMemberDirect( wm->GetScript(),"initApplication", this, Application::initApplication );
		wm->GetScript()->RunScript("scripts/mainloop.lua");

		// The main C++ loop
		TEvent event;
		while(true)
		{
			pPlatform->GetEvent(&event);
			if(event.mType == TEvent::kQuit)
			{
				break;
			}

			if (event.mType == TEvent::kClose)
			{
				TWindowManager::GetInstance()->GetScript()->RunScript("scripts/quitverify.lua");
			}

			if (event.mType == TEvent::kFullScreenToggle)
			{
				TSettings::GetInstance()->UpdateFullScreen();
			}

			// Pass the event to the Window manager for further processing
			TPlatform::GetInstance()->GetWindowManager()->HandleEvent(&event);
		}

		release();
		TSettings::DeleteSettings();
	}

	//-------------------------------------------------------------------------

	void Application::onInit()
	{
		removeAllEntity();
		setFactory( new Factory() );
		getFactory()->init( TWindowManager::GetInstance()->GetScript(), this );
		initLuaFunction( 0, "onInit" );
		executeLuaFunction( 0 );
	}

	//-------------------------------------------------------------------------

	void Application::onRelease()
	{
		releaseLuaFunction( 0 );
		getFactory()->release();
		Factory* factory( getFactory() );
		setFactory( 0 );
		delete factory;
		removeAllEntity();
	}

	//-------------------------------------------------------------------------
}