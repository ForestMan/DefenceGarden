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
 * Interface for class T2dParticleRenderer
 */

#ifndef _2DPARTICLERENDERER_H_INCLUDED
#define _2DPARTICLERENDERER_H_INCLUDED

#ifndef PARTICLERENDERER_H_INCLUDED
#include "particlerenderer.h"
#endif

#ifndef DEBUG_H_INCLUDED
#include <pf/debug.h>
#endif

#ifndef RENDERER_H_INCLUDED
#include <pf/renderer.h>
#endif

#include <vector>

#ifndef TAGGING_IGNORE
struct TTransformedLitVert;
class TScript;
#endif

#if !defined(PFCONFIG_H_INCLUDED)
#include <pf/pfconfig.h>
#endif

#if !defined(VEC_H_INCLUDED)
#include "vec.h"
#endif

static const int k2dParticlePosition = 0; ///< Index of 2d particle position value. (TVec2)
static const int k2dParticleUp = 2;		  ///< Index of 2d particle "up" vector. (TVec2)
static const int k2dParticleScale = 4;	  ///< Index of particle scale (TReal).
static const int k2dParticleColor = 5;	  ///< Index of particle color (TColor).
static const int k2dParticleFrame = 9; 	  ///< Index of particle frame number (TReal).

/**
 * Basic Particle Values.
 *
 * \see T2dParticleRenderer
 */
class T2dParticle
{
public:
	/// Particle position. Referenced as pPosition in a
	/// Lua particle script.
	TVec2				mPosition ;
	/// Current up vector of particle. Referenced as pUp.
    TVec2				mUp ;
	/// Current scale of particle. Referenced as pScale.
    TReal				mScale ;
	/// Current particle color. Referenced as pColor.
	TColor  			mColor ;
	/// Current frame of the particle animation (as int). Referenced as pFrame.
	TReal				mFrame ;
};


/**
 * A particle renderer that expects 2d particles. This is the
 * default particle renderer used by TLuaParticleSystem.
 *
 * T2dParticleRenderer defines five particle registers:
 *
 * - \c pPosition [2] The 2d position of a particle.
 * - \c pUp [2] The 2d "up" vector of a particle.
 * - \c pScale [1] The current particle scale.
 * - \c pColor [4] The current particle color.
 * - \c mFrame [1] The current particle frame (when using a
 *   TAnimatedTexture).
 *
 * @see T2dParticle
 * @see TLuaParticleSystem
 */
class PFLIB_API T2dParticleRenderer : public TParticleRenderer
{
public:
	/// Default Constructor
	T2dParticleRenderer();

private:
	// noncopyable
	T2dParticleRenderer( const T2dParticleRenderer & other );
	const T2dParticleRenderer & operator =( const T2dParticleRenderer & other );
public:
	/// Destructor
	virtual ~T2dParticleRenderer();

    virtual void Draw( const TVec3 & at, TReal alpha, const ParticleList & particles, int maxParticles );

    /**
     * Size of the particle object to render.
     *
     * @param size   Width and height of particle square on screen
     *               in pixels.
     */
	void SetParticleSize( const TVec2 & size )
	{
		mParticleSize = size;
	}

	/**
	 * Set the blend mode for a particular layer.
	 *
	 * @param mode   Blend mode
	 */
	void SetBlendMode( TRenderer::EBlendMode mode ) { mBlendMode = mode; };

	virtual void SetTexture( TTextureRef texture ) ;
	virtual void SetRendererOption( str option, const TReal (& value)[4] );
    virtual uint32_t GetPrototypeParticleSize();
    virtual TReal * GetPrototypeParticle();

	virtual str GetLuaInitString()
	{
		return
			"pPosition = Allocate(2);\n"
			"pUp = Allocate(2);\n"
			"pScale = Allocate(1);\n"
			"pColor = Allocate(4);\n"
			"pFrame = Allocate(1);\n" ;
	}

private:
    /**
     * Update the internal vertex buffer size.
     *
     * @param size   New size of buffer, in TTransformedLitVerts.
     */
	inline void UpdateBuffer( int size );


    /**
     * Create TTransformedLitVerts based on the T2dParticle at p.
     *
     * @param verts  [out] Verts to fill.
     * @param p      Particle to read.
     * @param at     Position to place particle at.
     */
	void MakeQuad2di( TTransformedLitVert * verts, const TReal * p, const TVec3 & at, TReal alpha  );
	void MakeQuad2diuv( TTransformedLitVert * verts, const TReal * p, const TVec3 & at, TVec2 * uv, TReal alpha  );

	void MakeQuad2d( TTransformedLitVert * verts, const TReal * fp, const TVec3 & at, TReal alpha );
	void MakeQuad2duv( TTransformedLitVert * verts, const TReal * fp, const TVec3 & at, TVec2 * uv, TReal alpha );

	TRenderer::EBlendMode		mBlendMode ;		///< Blend mode to draw particles with for each layer

	TVec2			mParticleSize ;		///< Size of a particle

    /**
     * An array of indices into our vertex buffer.
     */
    static unsigned short	    *mIndices ;
	static TTransformedLitVert  *mVertBuffer;
	static int					mVertBufferSize;
	static int					mParticleSystems;

    T2dParticle mInitializedParticle ;
	TTextureRef mTexture;


	bool		mBrokenIndexedVertices ;
};

#endif // _2DPARTICLERENDERER_H_INCLUDED
