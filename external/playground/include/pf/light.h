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
 * Definition of struct TLight
 */

#ifndef LIGHT_H_INCLUDED
#define LIGHT_H_INCLUDED

#ifndef PFTYPES_H_INCLUDED
#include "pftypes.h"
#endif

#ifndef VEC_H_INCLUDED
#include "vec.h"
#endif

/// A 3d light.
struct TLight
{
	/// Light types
	enum ELightType
	{
		kDirectional,		///< A directional light source.
		kPointSource,		///< A point source light.
		kSpotLight,			///< A spotlight.
	};                  	

	/// Default constructor, which resets the light parameters to a white directional light.
	TLight() :
		mType( kDirectional ),
		mDiff( TColor(1,1,1,0) ),
		mSpec( TColor(0,0,0,0) ),
		mAmb( TColor(0,0,0,0) ),
		mPos( TVec3(0,0,0) ),
		mDir( TVec3(0,0,1) ),
		mRange( ((TReal)sqrt(FLT_MAX)) ),
		mTheta(0),
		mPhi(0)
	{
		mAttenuation[0]=
			mAttenuation[1]=
			mAttenuation[2]= 0 ;
	}

	ELightType	mType ;			///< Type of light
	TColor 		mDiff;			///< Diffuse value
	TColor 		mSpec;			///< Specular value
	TColor 		mAmb;			///< Ambient value
	TVec3   	mPos;			///< Position of light. No meaning for directional light sources.
	TVec3   	mDir;			///< Direction of light. No meaning for point source light sources.
	TReal		mRange ;		///< Effective range of light. No meaning for directional light sources.

	/**
	 * Attenuation factors: Constant, Linear, and Quadratic.
	 * Attenuation is calculated based on the following equation:
	 * 
	 * \f[
	 * A = \frac{1}{a_0 + a_1 D + a_2 D^2}
	 * \f]
	 * 
	 * Where \f$a_0-a_2\f$ are mAttenuation[0]-mAttenuation[2], and
	 * D is the distance of the light source to the vertex, normalized
	 * to 0,1 over the range of the light.
	 */
	TReal	mAttenuation[3];

	/**
	 * Angle of inner cone of spotlight, in radians.
	 */
	TReal	mTheta ;

	/**
	 * Angle of outer edge of spotlight dropoff, in radians.
	 */
	TReal	mPhi ;
};

#endif // LIGHT_H_INCLUDED// 	 // 
