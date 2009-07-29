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
 * @condfile
 * Interface for class TLuaParticleSystem
 */


#ifndef LUAPARTICLESYSTEM_H_INCLUDED
#define LUAPARTICLESYSTEM_H_INCLUDED

#include <pf/pftypeinfo.h>
#include <pf/script.h>
#include <pf/luatable.h>
#include <pf/debug.h>

#ifndef TAGGING_IGNORE
class TRandom ;
class TLuaParticleSystem;
class TParticleCollection ;
class TParticleRenderer ;
class TParticleFunction ;
#endif

#include <map>

#ifndef USER_DOCS
/**
 * A caught error. Default is to ASSERT in debug build, but
 * handler is replaceable.
 */
class TFailure
{
public:
	TFailure(){}
	virtual ~TFailure(){}
	virtual void Failure( const char * cause, int line, const char * file )
	{
		ERROR_WRITE( ("%s(%d): %s", file, line, cause) );
#ifdef _DEBUG
		pfDebugBreak();
#endif
	}
	virtual void Status( const char * message )
	{
		ERROR_WRITE( ("%s", message) );
	}
};
#endif

/**
 * A set of parameters or return values, depending on context.
 */
class TParamSet
{
public:
    /// The maximum number of parameters that can be passed.
    static const uint32_t kMaxParams = 255;

	/**
	 * Reload part of a ParamSet with return parameters.
	 *
	 * @param base   New first parameter in set
	 * @param s      A pointer to a local list of parameter sizes (number of floats
	 *               in each parameter).
	 * @param count  Number of parameters.
	 *               Must be less than or equal to original parameter count.
	 */
    void ResetPartial( uint32_t base, uint8_t * s, uint32_t count )
    {
        ASSERT((base+count)<=kMaxParams);

		mCount = base+count;
        if (s!=mSizes)
        {
            memcpy(mSizes+base,s,count*sizeof(float));
        }
        uint32_t offset = mOffsets[base];
        int i=base;
        while (count--)
        {
            offset+= *(s++);
            mOffsets[++i]=offset;
        }
    }

    /**
     * Reload a ParamSet with return parameters.
     *
     * @param s      A pointer to a local list of parameter sizes (number of floats
     *               in each parameter).
     * @param count  Number of parameters.
     *               Must be less than or equal to original parameter count.
     */
    void Reset( uint8_t * s, uint32_t count )
    {
        ResetPartial(0,s,count);
    }

	/**
	 * Create a TParamSet
	 *
	 * @param p      Set of parameter values (must be count*4 in TReals)
	 * @param s      Sizes of parameters (must be count int8_t values)
	 * @param count  Number of parameters.
	 */
    TParamSet( TReal * p, uint8_t * s, uint32_t count ) :
        mParams(p),
        mSizes(s),
        mInitialCount(count)
    {
        mOffsets[0]=0;
        Reset(s,count);
    }

    /**
     * Get the current parameter count
     *
     * @return Number of parameters in the set.
     */
    uint8_t GetCount(){ return (uint8_t)mCount; }

	/**
	 * Get the offset of the nth entry in the set.
	 *
     * @param param  Parameter entry to query.
	 *
	 * @return Offset (in TReals) into the set.
	 */
	uint32_t GetOffset( uint8_t param ) { return mOffsets[param]; 	}

	/**
	 * Get the size of a particular entry.
	 *
	 * @param entry  Entry to query.
	 *
	 * @return Size (in TReals) of entry.
	 */
	uint8_t GetSize( uint8_t entry ) { return mSizes[entry]; }

    /**
     * Point this at a new parameter set (typically a new particle).
     *
     * @param p
     */
    void Redirect( TReal * p )
    {
        mParams = p ;
    }

    /**
     * Parameter extractor.
     *
     * @param Type   Type of parameter to extract.
     * @param param  Parameter base (which parameter number this is)
     *
     * @return A reference of type Type to the parameter that can be
     *         read or written to.
     */
    template<typename Type>
    Type & Param(uint8_t param)
    {
//        PARTICLE_ASSERT(mSizes[param]==sizeof(Type)/sizeof(float),"Invalid Parameter Size");
        return *(Type*)(mParams+mOffsets[param]);
    }

	/**
	 * Get a raw real value.
	 *
	 * @param index  Index of real to extract.
	 *
	 * @return A value in the set.
	 */
	TReal & GetReal(uint32_t index)
	{
		return mParams[index];
	}

	/**
	 * Get a raw real value.
	 *
	 * @param index  Index of real to extract.
	 *
	 * @return A value in the set.
	 */
	TReal GetReal(uint32_t index) const
	{
		return mParams[index];
	}

private:
    TReal * mParams ;
    uint8_t * mSizes ;
    uint32_t mOffsets[kMaxParams+1] ;
    uint32_t mCount ;
    uint32_t mInitialCount ;
};

/**
 * The internal state of a TLuaParticleSystem.
 */
class TParticleMachineState
{
public:
    /**
     * A set of function parameters.
     *
     * @param paramSet The stack to initialize function parameters from.
     * @param lps      A pointer to the associated TLuaParticleSystem.
     */
    TParticleMachineState( TParamSet & paramSet, TLuaParticleSystem * lps ):
		mLPS(lps),
        mParamSet(paramSet),
        mBase(0),
		mStackPointer(0),
        mCount(TParamSet::kMaxParams)
    {

    }

/*	void AdjustStack( int8_t size )
	{
		ASSERT((int32_t)mStackPointer>=-size);
		mStackPointer += size;
		ASSERT(mStackPointer<=TParamSet::kMaxParams);
	}*/

	/**
     * Parameter extractor.
     *
     * @param Type   Type of parameter to extract.
     * @param param  Parameter base (which parameter number this is)
     *
     * @return A Type reference that can be read or written to.
     */
    template<typename Type>
    Type & Param(int8_t param)
    {
		ASSERT(param>=0);
		return mParamSet.Param<Type>((uint8_t)mBase+ param) ;
    }

	/**
	 * Get a TReal value directly from the stack.
	 *
	 * @param index  Index of value.
	 *
	 * @return The value from the stack.
	 */
	TReal & GetReal( uint32_t index )
	{
		return mParamSet.GetReal(index+mParamSet.GetOffset(mBase));
	}

	/**
	 * Get the size of one of the parameters.
	 *
	 * @param param  Parameter to query.
	 *
	 * @return Size of parameter.
	 */
	uint8_t GetSize(int8_t param)
	{
		ASSERT(param>=0);
		return mParamSet.GetSize((uint8_t)mBase+ param) ;
	}

    /**
     * Push a value onto the stack.
     *
     * @param r      Pointer to the value array.
     * @param size   Size of the value array.
     */
	void Push(TReal * r, uint8_t size )
	{
		mParamSet.ResetPartial(mBase,&size,1);

		uint32_t offset = mParamSet.GetOffset((uint8_t)mBase);

		do
		{
			mParamSet.GetReal(offset++)= *(r++) ;
		} while (--size);

		mStackPointer = ++mBase;
	}

	/**
     * Push a value onto the stack. Invalidates incoming parameter
     * list.
     *
	 * @param Type
	 * @param value
	 */
	template<typename Type>
	void Push(Type value)
	{
		Push( (TReal*)&value, sizeof(Type)/sizeof(TReal) );
	}

	/**
	 * Exit a function (removes any remaining parameters; a NOP
	 * if the function pushed any return values).
	 */
	void Leave()
	{
		mStackPointer = mBase;
	}

    /**
     * Get the local parameter count
     *
     * @return Number of parameters in the set.
     */
    uint8_t GetCount(){ return static_cast<uint8_t>(mCount); }

    /**
     * Get the offset of a parameter on the stack.
     *
     * @param param  The parameter number to retrieve. The stack is an array of
     *               TReal values, but they logically group into parameters, so
     *               if the first parameter is a Vec3(), the second parameter
     *               offset will be 3.
     *
     * @return The offset to the requested parameter.
     */
	uint32_t GetOffset( uint8_t param ) { return mParamSet.GetOffset(mBase+param)-mParamSet.GetOffset(mBase); }

    /**
     * Reload with return parameters.
     *
     * @param s      A pointer to a local list of parameter sizes (number of floats
     *               in each parameter).
     * @param count  Number of return values.
     */
    void InitReturnValues( uint8_t * s, uint32_t count )
    {
        mParamSet.ResetPartial(mBase,s,count);
    }

	/**
     * Set the number of parameters being passed to the current
     * function
	 *
	 * @param num    Number of parameters
	 */
	void SetNumParams(uint8_t num)
	{
		if (mStackPointer<num) return ;
		mBase= mStackPointer-num ;
		mNumParams = num;
	}

    /**
     * Get the number of parameters passed.
     *
     * @return The number of incoming parameters.
     */
	uint8_t GetNumParams()
	{
		return mNumParams;
	}

	/**
	 * Get a registered data source by index.
	 *
	 * @param source   Data source index to read (must be negative).
	 *
	 * @return The data source.
	 */
	TParticleFunction * GetDataSource( int32_t source );
private:

	void Push( uint8_t size )
	{
		ASSERT((uint32_t)mStackPointer+size<=TParamSet::kMaxParams);
		mStackPointer = mStackPointer+ size;
	}

	void Pop( uint8_t size )
	{
		ASSERT(mStackPointer>=size);
		mStackPointer = mStackPointer - size;
	}

	TLuaParticleSystem * mLPS;
	TParticleMachineState & operator=( const TParticleMachineState & );
    TParamSet & mParamSet ;
    uint8_t    mBase;
    uint8_t    mStackPointer;	// Stack pointer AS NUMBER OF PARAMETERS
    uint32_t    mCount ;
	uint8_t		mNumParams;
};


/**
 * A particle state. A particle state is made up of some number
 * of floating point values and an indication of elapsed
 * milliseconds.
 *
 * Note the private inheritance: We don't want to expose
 * TParamSet::Reset on a TParticleState, but we want the
 * TParamSet implementation.
 */
class TParticleState : private TParamSet
{
public:

    /**
     * Constructor.
     *
     * @param p      A pointer to an array of TReals that will be
     *               used as the dynamic particle state. Must be
     *               count*4 TReals.
     * @param s      A pointer to an array of bytes that indicate parameter sizes.
     * @param count  The number of members in the particle.
     * @param ms     The number of milliseconds that have passed in this time
     *               step.
     */
    TParticleState( TReal * p, uint8_t * s, uint32_t count, uint32_t ms ) :
        TParamSet(p,s,count),
        mMS(ms),
		mAlive(true)
    {
    }

    /**
     * Parameter extractor.
     *
     * @param Type   Type of parameter to extract.
     * @param param  Parameter base (which parameter number this is)
     *
     * @return A Type reference that can be read or written to.
     */
    template<typename Type>
    Type & Param(uint8_t param)
    {
        return TParamSet::Param<Type>(param);
    }

	TReal & GetReal( uint32_t index )
	{
		ASSERT(index<GetOffset( TParamSet::GetCount() ) );
		return TParamSet::GetReal(index);
	}

	uint32_t GetOffset( uint8_t param )
	{
		return TParamSet::GetOffset(param);
	}

    void Redirect( TReal * p )
    {
        TParamSet::Redirect(p);
		mAlive = true;
    }

    uint32_t GetCount(){ return TParamSet::GetCount(); }

    /**
     * Get the number of milliseconds being processed.
     *
     * @return Time elapsed in milliseconds.
     */
    uint32_t GetMS() { return mMS; }

	/**
	 * Query whether this particle is still alive.
	 *
	 * @return True if alive.
	 */
	bool GetAlive() { return mAlive; }

	/**
	 * Kill this particle (mark for destruction).
	 */
	void KillParticle() { mAlive = false; }
private:
    uint32_t 	mMS;
	bool		mAlive;

};


/**
 * A user data source.
 */

class PFLIB_API TParticleFunction
{
	PFTYPEDEFBASE_DC(TParticleFunction);
public:
	virtual ~TParticleFunction(){}

	/**
	 * Optional initializer that's called once per particle
	 * render frame.
	 */
	virtual void InitFrame(){};

	/**
	 * Initialize parameters.
	 *
	 * For DataSource functions, incoming parameters will always be
	 *
	 * @param paramCount Number of parameters in array.
	 * @param sizes      Pointer to size array (NULL for registered data sources).
	 *
	 * @return Size of parameter in TReals.
	 */
	virtual uint8_t GetReturnSize( int paramCount, uint8_t * sizes )=0;

	/**
	 * Process function or fetch data
	 *
	 * @param particle Particle being processed.
     * @param params   [in/out] parameters
     *
     * @return True on success; false if parameters are
     *         incorrect.
     */
	virtual bool Process( TParticleState & particle, TParticleMachineState & params )=0;

// Define these so that auto-complete and documentation can know about them
#if DOXYGEN
	/**
	 * Dynamic creation.
	 *
	 * @param id     The PFClassId of the class to create.
	 *
	 * @return A new TParticleFunction-derived class instance.
	 */
	static TParticleFunction * CreateFromId( PFClassId id );

	/// The class id of this class.
	PFClassId ClassId();

	/**
	 * Query whether this class IsKindOf another class.
	 *
	 * @param type   The ClassId of the class to compare to.
	 *
	 * @return True if class is, or is derived from, target class.
	 */
	virtual bool IsKindOf( int type );

	/**
	 * Safely cast this class to another class.
	 *
	 * @param TO     Class to convert to.
	 *
	 * @return A cast pointer, or NULL if this class is unrelated to the
	 *         target.
	 */
	template<class TO> TO * GetCast();
#endif
	/// The name of the function; will be set internally when it's registered.
	str mName;
};

/// A reference to a TParticleFunction.
typedef shared_ptr<TParticleFunction> TParticleFunctionRef ;

/**
 * A particle member value. When additional values are added
 * to a particle (beyond what exists in the base particle values),
 * each is allocated as a ParticleMember.
 */
struct ParticleMember
{
	/**
	 * Constructor
	 *
	 * @param base   Base index (in TReals) of this member.
	 * @param size   Size of this member in TReals.
	 */
	ParticleMember(int16_t base=0, uint16_t size=0) :
		mBase(base), mSize(size)
	{
	}

	/**
	 * Index in TReals into the particle.
	 */
    int16_t		mBase;

	/**
	 * Size of this member.
	 */
	uint16_t	mSize;
};

/**
 * A particle system driven by Lua scripts.
 *
 * Particle system documentation is at @ref particledocs
 */
class PFLIB_API TLuaParticleSystem
{
public:
	/**
	 * Default Constructor
	 *
     * @param r     Particle renderer to use. If NULL, will create a
     *              T2dParticleRenderer.
	 */
	TLuaParticleSystem( TParticleRenderer * r =NULL );

private:
	TLuaParticleSystem( const TLuaParticleSystem & );
	const TLuaParticleSystem & operator=( const TLuaParticleSystem & );
public:

	/// Destructor
	virtual ~TLuaParticleSystem();

    /**
     * Draw the system.
     *
     * @param at     Where to draw it. For 2d particles, the third component just
     *               sets the Z-depth to draw the particles.
     */
    void Draw( const TVec3 & at );

	/**
     * An initializer with a client-defined specification. Note that
     * this doesn't call NewScript() for you--it's acting more like
     * a "Load()" call, and will likely have its name changed in 4.1
     * to better reflect its behavior.
	 *
	 * @param spec   Client spec.
	 */
	virtual bool Init(str spec);

	/**
	 * Reset the script to a virgin state.
	 */
	void NewScript();

	/**
     * Update particle system.
	 *
	 * @param ms     Number of milliseconds to advance system.
	 */
	virtual void Update(int ms);

	/**
     * Register a function type with the particle system. This
     * interface allows you to create a new operator that can be
     * used in the particle system.
	 *
	 * @param name      Name of process to use (or to replace).
     * @param processId The PFClassId of the class that provides the
     *                  function. See \ref rtti for more
     *                  information.
	 *
	 * @see AdoptFunctionInstance
	 */
	void RegisterFunction( str name, PFClassId processId );

	/**
	 * Register a function type with the particle system. This
	 * interface allows you to create a new operator that can be
	 * used in the particle system.
	 *
	 * Adopt semantics are used: You need to create a
	 * TParticleFunction-derived class instance and pass it in.
	 * Lifetime management is then handled by the
	 * TLuaParticleSystem.
	 *
	 * For example:
	 *
	 * @code
	 *      AdoptFunctionInstance("fNewFunction", new MyParticleFunction );
	 * @endcode
	 * @param name     Name of process to use (or to replace).
	 * @param function A new instance of the class that provides the function.
	 *                 The TLuaParticleSystem will delete this function on
	 *                 destruction.
	 *
	 * @see RegisterFunction
	 */
	void AdoptFunctionInstance( str name, TParticleFunction * function );

	/**
	 * Register a data source.
	 *
	 * @param name     Name of data source in Lua, e.g., "dSpriteVelocity"
	 * @param function Class that contains the data accessor
	 *
	 * @return Data source index.
	 */
	int32_t RegisterDataSource( str name, TParticleFunction * function );

	/**
     * Get a registered data source by index.
	 *
	 * @param source   Data source index to read (must be negative).
	 *
     * @return The data source.
	 */
	TParticleFunction * GetDataSource( int32_t source );

	/**
     * Allocate a particle variable.
     *
     * This function is typically called from Lua to allocate custom
     * particle variables, and by the renderer to allocate the
     * default particle members.
	 *
	 * @param size   Size of particle variable (1-4) in TReal values.
	 *
     * @return      A ParticleMember struct that refers to the newly
     *              allocated member.
     */
	ParticleMember Allocate( uint8_t size );

    /**
     * Get a new instance of a particle function associated with
     * a named type.
     *
     * @param type   A registered particle function type.
     *
     * @return A reference-counted pointer to a newly created
     *         particle function.
     */
	TParticleFunctionRef GetParticleFunction( str type );

    /**
     * Get a pointer to the attached particle system script.
     *
     * @return A pointer to the current script.
     */
	TScript * GetScript() { return mScript; }

    /**
     * Get a reference to the asset-type TScriptCodeRef. This allows
     * Lua scripts to be pre-loaded once and maintained in the asset
     * system.
     *
     * @return A reference to the TScriptCode object that contains the
     *         (compiled) Lua code.
     */
	TScriptCodeRef GetScriptCode() { return mSCR; }

	/**
	 * Is the particle system done?
	 *
	 * @return True if done; false otherwise.
	 */
	bool IsDone();

	/**
	 * Flag the particle system as done.
	 */
	void SetDone();

	/**
	 * Not done any more!
	 */
	void ResetDone();

	/**
     * Get number of current active particles.
	 *
	 * @return The current number of particles active.
	 */
	uint32_t GetParticleCount();

	/**
     * Set the alpha of this particle system.
     *
     * @param a      Alpha (transparency) of this system. 0 is transparent, 1 opaque.
     */
	void SetAlpha( float a );

	/**
	 * Get the particle system random number generator.
	 *
	 * @return A pointer to the particle system random generator.
	 */
	static TRandom * GetRandom();

	/**
	 * Get the embedded particle renderer.
	 *
	 * @return A pointer to the TParticleRenderer associated with this
	 *         particle system.
	 */
	TParticleRenderer * GetParticleRenderer()
	{
		return mParticleRenderer ;
	}

	/**
     * Bind an error handler to all TLuaParticleSystems.
	 *
	 * @param failureHandler
	 *               Handler to add.
	 */
	static void SetErrorHandler( TFailure * failureHandler );

	/**
	 * Get the current error handler.
	 *
	 * @return A pointer to the current error handler, if any.
	 */
	static TFailure * GetErrorHandler() { return mFailureHandler; }

	/**
	 * Get a pointer to the currently attached TParticleRenderer.
	 *
	 * @return A pointer to the particle renderer.
	 */
	TParticleRenderer * GetRenderer() { return mParticleRenderer; }
private:
    void SetRendererOption1( const char * option, TReal value );
    void SetRendererOption2( const char * option, TReal value1, TReal value2 );
    void SetRendererOption3( const char * option, TReal value1, TReal value2, TReal value3 );
    void SetRendererOption4( const char * option, TReal value1, TReal value2, TReal value3, TReal value4 );

    /** @deprecated
     *
     *  I can't figure out what this was supposed to do. Yes, I
     *  know I wrote this code. I obviously should have
     *  documented it sooner...
     */
    void SetLuaProtected( bool protect ) { mProtected = protect ; }

	int GetParticleMemberParts( lua_State * L );
	void AccumulateInitStream();
	void AccumulateAnimStream();

	void CreateParticles( TReal num );
	int CompileParam( lua_State * L );

	void SetTexture( str texture );
	void SetNumParticles( int num );
	void Do( str task, int base, TLuaTable * table );
	void ProcessParticles( int ms );

	bool mCreatedRenderer ;
	TParticleRenderer * mParticleRenderer ;
	TParticleCollection * mParticleCollection ;

	TScript		* mScript ;

	// lpsonescript support
	static TScript	* 	gScript ;
	static uint32_t 	gScriptUseCount;
	static uint32_t 	gUpdateCount;
	uint32_t 	mUpdateNumber;

	TScriptCodeRef mSCR ;
	str			  mPath ;

	TReal		* mParticleData ;
	uint8_t		mParticleSize ;
	std::vector<uint8_t> mSizes ;

	std::vector<TParticleFunction*> mDataSources;

	typedef std::map<str,PFClassId> FunctionMap ;
	typedef std::map<str,TParticleFunctionRef> FunctionInstanceMap ;

	FunctionMap 			mFunctionMap ;
	FunctionInstanceMap 	mFunctionInstanceMap ;

	static const int kNumParams = 3;

	uint8_t mSizePool[1024] ;
	uint8_t mCurSize ;

	struct PInstruction
	{
		PInstruction() { mSizes = mSizes_; }

		TParticleFunctionRef mFunc ;	///< Function to run
		uint8_t		*mSizes;			///< The size array.
		uint8_t		mSizes_[kNumParams];///< Up to kNumParams parameters are just here.
		uint8_t		mCount ;			///< Number of parameters
	};


	typedef std::vector<PInstruction> PInstructionList ;

	/// Code that's executed to initialize a fresh particle
	PInstructionList		mInitCode ;

	/// Code that's executed once per update frame
	PInstructionList		mAnimCode ;

	PInstructionList		mCurList ;

	float				mAlpha ;

	bool				mProtected ;

	bool				mDone ;

	TReal				mParticlesToCreate ;

	static TFailure		mDefaultFailureHandler;
	static TFailure *	mFailureHandler;
};

#endif // LUAPARTICLESYSTEM_H_INCLUDED
