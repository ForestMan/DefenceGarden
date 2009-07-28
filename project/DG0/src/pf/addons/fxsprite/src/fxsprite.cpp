//------------------------------------------------------------------
// Copyright (c) 2004-2008 PlayFirst, Inc. All rights reserved.
// Part of the Playground SDK(tm).
//
// This file contains proprietary information of PlayFirst, Inc.
// It is provided subject to a license agreement. It may not be
// reproduced, used, disclosed, sold or transferred without the
// prior written consent of PlayFirst, Inc.
//------------------------------------------------------------------


/**
 * @file
 * Implementation for class TFxSprite
 */

#include "../fxsprite.h"
#include <pf/animtask.h>
#include <pf/texture.h>

PFTYPEIMPL(TFxSprite);

TFxSpriteAnimTask	*TFxSprite::mAnimTask=NULL;

PFTYPEIMPL_DC(TEmitterLocus);
PFTYPEIMPL_DC(TEmitterUp);

TFxSprite::TFxSprite( int32_t layer ) :
	TSprite(layer),
	mDrawnOnce(false)
{
}

TFxSprite::~TFxSprite()
{
}

TFxSpriteAnimTask * TFxSprite::GetAnimTask()
{
	if (!mAnimTask)
	{
		mAnimTask = new TFxSpriteAnimTask();
		TPlatform::GetInstance()->AdoptTask(mAnimTask);
		mAnimTask->SetDelay(15);
	}
	return mAnimTask;
}

void TFxSprite::Draw(const TDrawSpec &environmentSpec, int32_t depth)
{
	if (!IsVisible())
	{
		return;
	}

	TDrawSpec localSpec = GetDrawSpec().GetRelative(environmentSpec);

	mEmitterLocus.mPosition = TVec2(localSpec.mMatrix[2]);
	mEmitterUp.mUp = TVec2(localSpec.mMatrix[1].x, -localSpec.mMatrix[1].y);

	mDrawnOnce = true;

	SpriteList::iterator s;

	if (depth!=0)
	{
		// Draw the sprites that have negative layer (behind us)
		for (s=mChildren.begin(); s!=mChildren.end(); ++s)
		{
			TSpriteRef sprite = *s;
			if (sprite->GetLayer()>=0)  break ;

			sprite->Draw(localSpec,depth-1);
		}
	}

	// go ahead and draw the texture if we have one; we can use
	// this as a marker in FluidFX, at the very least.
	if (GetTexture())
		GetTexture()->DrawSprite(localSpec);

	mLPS.Draw( mRenderOrigin );

	if (depth!=0)
	{
		// Positive children
		for ( ; s!=mChildren.end(); ++s)
		{
			TSpriteRef sprite = *s;
			sprite->Draw(localSpec,depth-1);
		}
	}
}

TFxSpriteRef TFxSprite::Create(int32_t layer, str particleSystem, TFxSpriteAnimTask * task )
{
	TFxSpriteRef s= TFxSpriteRef( new TFxSprite(layer) );

	ScriptRegisterMemberFunctor(s->GetLPS()->GetScript(),"CreateFx",s.get(),TFxSprite::CreateFx);
	s->mContainerFolder = particleSystem;
	int lastSlash = s->mContainerFolder.find("/");
	if (lastSlash != str::npos)
	{
		s->mContainerFolder.erase(lastSlash);
	}
	lua_pushlightuserdata(s->GetLPS()->GetScript()->GetState(),s.get());
	lua_setglobal(s->GetLPS()->GetScript()->GetState(),"gParent");

	if (particleSystem.length())
	{
		s->Init(particleSystem,task, false);
	}
	return s;
}

bool TFxSprite::Init(str particleSystem, TFxSpriteAnimTask * task, bool reset )
{
	TFxSpriteRef s = GetRef();

	if (reset)
	{
		mDrawnOnce = false;
		s->GetLPS()->NewScript();
	}

	if (particleSystem.length())
	{
		s->GetLPS()->RegisterDataSource("dLocus",&s->mEmitterLocus);
		s->GetLPS()->RegisterDataSource("dUp",&s->mEmitterUp);

		if( !s->GetLPS()->Init(particleSystem) )
		{
			return false;
		}
		s->mName = particleSystem ;
		if (!task)
		{
			task = ((TFxSpriteAnimTask*)GetAnimTask());
		}
		task->mSpriteList.insert( s );
	}
	return true;
}


bool TFxSprite::CreateBatch( TSpriteRef parent, str particlebatch )
{
	bool success = true;
	TScript * s = new TScript();

	lua_pushlightuserdata(s->GetState(),parent.get());
	lua_setglobal(s->GetState(),"gParent");

	ScriptRegisterMemberFunctor(s,"CreateFx",(TFxSprite*)NULL,TFxSprite::CreateFx);

	success = s->RunScript(particlebatch);

	delete s;
	return success;
}

int TFxSprite::CreateFx( lua_State * L )
{
	LuaAutoBlock lab(L);

	TLuaTable t(L);

	lua_getglobal(L,"gParent");

	TSprite * sprite = (TSprite*)lua_touserdata(L,-1);

	TVec3 at( (TReal)t.GetNumber("x"), (TReal)t.GetNumber("y"),1);

	int32_t layer = (int32_t)t.GetNumber("layer");

	str spec = t.GetString("spec");
	if (mContainerFolder.empty() == false)
	{
		spec = mContainerFolder + "/" + spec;
	}

	TFxSpriteRef fxSprite = Create(layer,spec);
	fxSprite->GetDrawSpec().mMatrix[2]=at ;

	sprite->AddChild(fxSprite);

	return 0;
}

#define FX_PARTICLE_MS_PER_FRAME 12
#define FX_PARTICLE_MAX_FRAMES 4

bool TFxSpriteAnimTask::Animate()
{
	uint32_t newms = GetTime();
	uint32_t ms = newms-mLastMS ;
	uint32_t times = ms / FX_PARTICLE_MS_PER_FRAME;

	if (times > FX_PARTICLE_MAX_FRAMES)
		times = FX_PARTICLE_MAX_FRAMES ;

	for (WeakFxSpriteList::iterator i =mSpriteList.begin();
		  i!=mSpriteList.end();
		  /*donothing*/)
	{
		WeakFxSpriteList::iterator thisFx = i++;

		TFxSpriteRef fxSprite = thisFx->lock();
		if ( !fxSprite || fxSprite->GetLPS()->IsDone() )
		{
			mSpriteList.erase(thisFx);
			if ( fxSprite && fxSprite->GetLPS()->IsDone())
			{
//				DEBUG_WRITE(("Time to get rid of child %s from parent", fxSprite->GetSpecName().c_str()));
  				TSprite * parent = fxSprite->GetParent();
				
  				if ( parent )
  				{
   					parent->RemoveChild( fxSprite );
  				}
			}	
			continue;
		}
		
		for (uint32_t i=0; i<times; ++i)
			fxSprite->Update( FX_PARTICLE_MS_PER_FRAME );
	}

	mLastMS=newms;

	if (mSpriteList.empty())
	{
		return false;
	}

	return true;
}

void TFxSprite::Update(int ms)
{
	// No updates until we've been drawn once.
	if (mDrawnOnce)
	{
		GetLPS()->Update( ms );
	}
}
