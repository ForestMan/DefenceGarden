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
 * Interface for class TFxSprite
 */


#ifndef FXSPRITE_H_INCLUDED
#define FXSPRITE_H_INCLUDED

#ifndef SPRITE_H_INCLUDED
#include <pf/sprite.h>
#endif

#ifndef LUAPARTICLESYSTEM_H_INCLUDED
#include <pf/luaparticlesystem.h>
#endif

#include <set>

#ifndef TAGGING_IGNORE
class TFxSprite;
class TAnimTask;
class TFxSpriteAnimTask;
#endif

typedef shared_ptr<TFxSprite> TFxSpriteRef ;

class TEmitterLocus : public TParticleFunction
{
	PFTYPEDEF_DC(TEmitterLocus,TParticleFunction);
public:
	TEmitterLocus() : mPosition(400,300) { }

	virtual bool Process(TParticleState &/*particle*/, TParticleMachineState &params)
	{
		params.Push( mPosition );
		return true;
	}

	virtual uint8_t GetReturnSize(int /*paramCount*/, uint8_t * /*sizes*/) { return 2; }

	TVec2 mPosition;
};

class TEmitterUp : public TParticleFunction
{
	PFTYPEDEF_DC(TEmitterUp,TParticleFunction);
public:
	TEmitterUp() : mUp(0,-1) { }

	virtual bool Process(TParticleState &/*particle*/, TParticleMachineState &params)
	{
		params.Push( mUp );
		return true;
	}

	virtual uint8_t GetReturnSize(int /*paramCount*/, uint8_t * /*sizes*/) { return 2; }

	TVec2 mUp;
};

class TFxSprite : public TSprite
{
	PFSHAREDTYPEDEF(TSprite);
protected:
/// Default Constructor
	TFxSprite( int32_t layer );
public:

	/**
	 * Create a single TFxSprite based on a Lua particle specification.
	 *
	 * @param layer  Layer to place the sprite.
	 * @param particleSystem
	 *               Particle system specification: Either a Lua string or a
	 *               .lua filename. Can be an empty string to
	 *               produce an uninitialized TLuaParticleSystem.
     * @param task   The TFxSpriteAnimTask that will update this
     *               sprite. You can pass NULL, in which case a
     *               global (static) TFxSpriteAnimTask will update
     *               this sprite.
	 *
	 * @return True on success.
	 */
	static TFxSpriteRef Create(int32_t layer, str particleSystem, TFxSpriteAnimTask * task=NULL );

	/**
	 * Initialize our TFxSprite with a particleSystem.
	 *
	 * @param particleSystem
	 *               Particle system to load.
	 * @param task   Update task to attach to.
	 *
	 * @return True on success.
	 */
	bool Init(str particleSystem, TFxSpriteAnimTask * task=NULL, bool reset=true );

	/**
	 * Create a batch of TFxSprites.
	 *
	 * @param parent Parent to attach the children to.
	 * @param particlebatch
	 *               The particle batch script. The only command accepted in
	 *               such a script is:
	 *
	 *               CreateFX{ layer=[spritelayer], x=[x position], y=[y position], spec=[particle spec] };
	 *
	 * @return True on success.
	 */
	static bool CreateBatch( TSpriteRef parent, str particlebatch );

	/// Destructor
	virtual ~TFxSprite();

	/**
     * Draw the sprite and its children. Note that TFxSprite::Draw
     * will ignore everything except the position (offset) in its
     * drawspec when rendering the particle system, though it will
     * still respect it as usual when drawing an attached texture
     * (if any).
	 *
	 * @param environmentSpec The 'parent' or envionment
	 *                 drawspec--the frame of reference that this
	 *                 sprite is to be rendered in. In general you
	 *                 shouldn't pass anything in for this parameter
	 *                 and instead should modify the position of the
	 *                 sprite using its GetDrawSpec() member.
	 *
	 *                 Defaults to a default-constructed TDrawSpec.
	 *                 See TDrawSpec for more details on what is
	 *                 inherited.
	 * @param depth     How many generations of children to draw; -1
	 *                  means all children.
	 * @see TDrawSpec
	 */
	virtual void Draw(const TDrawSpec &environmentSpec=TDrawSpec(), int32_t depth=-1);

	/**
	 * Get the encapsulated Lua Particle System.
	 *
	 * @return A pointer to the TLuaParticleSystem associated with this sprite.
	 */
	TLuaParticleSystem * GetLPS() { return &mLPS;}
	/**
	 * Get the name of the spec used to create this particle system.
	 *
	 * @return A Lua filename.
	 */
	str GetSpecName() { return mName; }

	/**
	 * Get the current render origin for the particle system.
	 *
	 * @return The current render origin. This is the value passed in as the
	 *         origin to TLuaParticleSystem::Draw().
	 */
	TVec3 GetRenderOrigin()
	{
		return mRenderOrigin;
	}

	/**
	 * Set the current render origin.
	 *
	 * @param origin Origin to set.
	 */
	void SetRenderOrigin( const TVec3 & origin )
	{
		mRenderOrigin = origin;
	}

	/**
     * Get the TAnimTask that updates all TFxSprite particle systems.
     *
     * The TAnimTask can be changed to run at a different update
     * speed, or to respect a different clock.
	 *
	 * @return A pointer to a TAnimTask.
	 */
	static TFxSpriteAnimTask * GetAnimTask();

	TFxSpriteRef GetRef()
	{
		return shared_from_this()->GetCast<TFxSprite>();
	}
private:
	void Update(int);

	TVec3	mRenderOrigin ;
	TEmitterLocus mEmitterLocus;
	TEmitterUp mEmitterUp;

	int CreateFx( lua_State * L );
	TLuaParticleSystem 	mLPS;
	str					mName;
	str					mContainerFolder;

	bool				mDrawnOnce ;

	friend class TFxSpriteAnimTask;
	static TFxSpriteAnimTask	*mAnimTask;
};

class TFxSpriteAnimTask : public TAnimTask
{
	/**
	 * Define this function to add the actual animation task.
	 *
	 * @return True to continue, false when we're done.
	 */
public:
	typedef std::set< weak_ptr<TFxSprite> > WeakFxSpriteList ;

	TFxSpriteAnimTask()
	{
		mLastMS = GetTime();
	}
	~TFxSpriteAnimTask()
	{
		TFxSprite::mAnimTask = NULL;
	}

	virtual bool Animate();

	WeakFxSpriteList mSpriteList ;
	uint32_t mLastMS ;
};

#endif // FXSPRITE_H_INCLUDED
