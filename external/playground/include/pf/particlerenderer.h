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
 * Interface for class TParticleRenderer
 */


#ifndef PARTICLERENDERER_H_INCLUDED
#define PARTICLERENDERER_H_INCLUDED

#ifndef FORWARD_H_INCLUDED
#include <pf/forward.h>
#endif

#ifndef VEC_H_INCLUDED
#include "vec.h"
#endif

#ifndef STR_H_INCLUDED
#include "str.h"
#endif

#include <list>

/// A list of particles.
typedef std::list<TReal*> ParticleList ;


/**
 * The abstract particle renderer class: This class is used
 * by TLuaParticleSystem to wrap an actual particle renderer.
 * This way you can use TLuaParticleSystem to drive the default
 * 2d particle system or other more complex systems.
 */
class PFLIB_API TParticleRenderer
{
public:
	virtual ~TParticleRenderer(){}

    /**
     * Render the particles.
     *
     * @param at        Location to render particles.
     * @param alpha     Alpha to render particles with.
     * @param particles The list of particles to render.
     * @param maxParticles
     *                  The maximum number of particles this particle system is
     *                  expecting to render. MUST be greater than the number of
     *                  particles or Bad Things will happen.
     */
    virtual void Draw( const TVec3 & at, TReal alpha, const ParticleList & particles, int maxParticles )=0;

	/**
	 * Set the texture for the particle.
	 *
	 * @param texture Texture to use.
	 */
	virtual void SetTexture( TTextureRef texture )=0;

	/**
	 * Set a renderer-specific option.
	 *
	 * @param option Option to set.
     * @param value  Value to set option to, in the form of an array
     *               of TReals. Not all values in array are relevant for
     *               all options.
	 */
	virtual void SetRendererOption( str option, const TReal (& value)[4] )=0;

    /**
     * Get an initialized particle that will be copied over each
     * particle after creation but before running initializers.
     *
     * @return A pointer to an array of TReals.
     */
    virtual TReal * GetPrototypeParticle()=0;

    /**
     * Size of the array of TReals returned by
     * GetPrototypeParticle.
     *
     * @return Number of reals.
     */
    virtual uint32_t GetPrototypeParticleSize()=0;

	/**
	 * Get a TLuaParticleSystem initialization string.
	 *
	 * @return A valid Lua string that will be executed by a TLuaParticleSystem
	 *         that uses this renderer.
	 */
	virtual str GetLuaInitString()=0;
};

#endif // PARTICLERENDERER_H_INCLUDED
